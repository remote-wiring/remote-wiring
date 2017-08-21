/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "FirmataDevice.h"

#include <cerrno>
#include <cstdio>
#include <cstring>

#include <FirmataConstants.h>

#include "RwConstants.h"
#include "Wire.h"
#include "WiringPin.h"

#define DATA_NOT_AVAILABLE_ANALOG 0x4000
#define PIN_MODE_UNSPECIFIED 0x00

using namespace remote_wiring;

FirmataDevice::FirmataDevice (
    Stream & stream_,
    bool jit_input_
) :
    RemoteDevice(_firmata_i2c),
    _attach_context(nullptr),
    _firmata_i2c(stream_),
    _firmware_name(nullptr),
    _firmware_semantic_version{0,0,0},
    _jit_input(jit_input_),
    _parser_buffer(nullptr),
    _parser_buffer_size(0),
    _pin_count(0),
    _pin_info_cache(nullptr),
    _pin_isr_cache(nullptr),
    _pin_isr_count(0),
    _pin_state_cache(nullptr),
    _protocol_semantic_version{0,0,0},
    _refresh_context(nullptr),
    _report_on_query(!jit_input_),
    _stream(stream_),
    _survey_context(nullptr),
    _uponAttach(nullptr),
    _uponRefresh(nullptr),
    _uponSurvey(nullptr)
{
    _marshaller.begin(stream_);

    // Configure the Firmata parser
    if ( nullptr == (_parser_buffer = new uint8_t[firmata::MAX_DATA_BYTES]) ) {
        errno = ENOMEM;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::FirmataDevice - Unable to allocate parser buffer!");
#endif
    } else if ( 0 != _parser.setDataBufferOfSize(_parser_buffer, firmata::MAX_DATA_BYTES) ) {
        errno = EACCES;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::FirmataDevice - Unable to update parser buffer!");
#endif
    } else {
        // Update state
        _parser_buffer_size = firmata::MAX_DATA_BYTES;

        // Attach callbacks
        _parser.attach(FirmataDevice::extendBuffer, this);
        _parser.attach(firmata::ANALOG_MESSAGE, analogReadCallback, this);
        _parser.attach(firmata::DIGITAL_MESSAGE, digitalReadCallback, this);
        _parser.attach(firmata::REPORT_FIRMWARE, firmwareVersionCallback, this);
        //TODO:[Firmata v3.0.0] _parser.attach(firmata::REPORT_VERSION, protocolVersionCallback, this);
        _parser.attach(firmata::START_SYSEX, FirmataDevice::sysexCallback, this);
    }
}

FirmataDevice::~FirmataDevice (
    void
) {
    delete[](_firmware_name);
    delete[](_pin_isr_cache);
    delete[](_parser_buffer);
    delete[](_pin_info_cache);
    delete[](_pin_state_cache);
}

size_t
FirmataDevice::_analogRead (
    const size_t pin_
) {
    size_t result;

    if ( !_pin_info_cache && updateStorageForPin(pin_) ) {
        result = static_cast<size_t>(UINT_MAX);
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::analogRead - Unable to allocate storage for pin!");
#endif
    } else if ( pin_ >= _pin_count ) {
        result = static_cast<size_t>(UINT_MAX);
        errno = ENXIO;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::analogRead - Pin out of bounds!");
#endif
    } else if ( _pin_info_cache && ! _pin_info_cache[pin_].analogReadAvailable() ) {
        result = static_cast<size_t>(UINT_MAX);
        errno = EOPNOTSUPP;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::analogRead - Pin incapable of analog read functionality!");
#endif
    } else if ( ANALOG_READ != _pin_state_cache[pin_].mode ) {
        //TODO: Block until callback is fired for pin_, and return valid result
        result = DATA_NOT_AVAILABLE_ANALOG;
        _pin_state_cache[pin_].value = DATA_NOT_AVAILABLE_ANALOG;
        _pin_state_cache[pin_].mode = ANALOG_READ;  // set cache to reporting
        _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata::PIN_MODE_ANALOG);  // Arduino will report pin number as 4-bit transform
        _stream.flush();
    } else {
        result = _pin_state_cache[pin_].value;
    }

    return result;
}

int
FirmataDevice::_analogWrite (
    const size_t pin_,
    const size_t value_
) {
    int error;

    if ( pin_ >= _pin_count ) {
        error = errno = ENXIO;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::analogWrite - Pin out of bounds!");
#endif
    } else if ( _pin_info_cache && !_pin_info_cache[pin_].analogWriteAvailable() ) {
        error = errno = EOPNOTSUPP;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::analogWrite - Pin incapable of analog write functionality!");
#endif
    } else if ( ANALOG_WRITE != _pin_state_cache[pin_].mode ) {
        error = 0;
        _pin_state_cache[pin_].mode = ANALOG_WRITE;
        _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata::PIN_MODE_PWM);
        _marshaller.sendAnalog(static_cast<uint8_t>(pin_), value_);
        _stream.flush();
    } else {
        error = 0;
        _marshaller.sendAnalog(static_cast<uint8_t>(pin_), value_);
        _stream.flush();
    }

    return error;
}

int
FirmataDevice::_attach (
    signal_t uponAttach_,
    void * context_
) {
    // Store callback
    _uponAttach = uponAttach_;
    _attach_context = context_;

    // Subscribe to serial data
    _stream.registerSerialEventCallback(FirmataDevice::serialEventCallback, this);

    // Search for remote device on serial channel
    _marshaller.queryFirmwareVersion();
    _stream.flush();

    return 0;
}

int
FirmataDevice::_attachInterrupt (
    size_t pin_,
    signal_t isr_,
    size_t mode_,
    void * context_
) {
    int error;

    if ( pin_ >= _pin_count ) {
        error = errno = ENXIO;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::attachInterrupt - Pin out of bounds!");
#endif
    } else if ( _pin_info_cache && !_pin_info_cache[pin_].digitalReadAvailable() ) {
        error = errno = EOPNOTSUPP;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::attachInterrupt - Pin incapable of digital read functionality!");
#endif
    } else if ( 0 != (error = updateStorageForPinInterrupt(pin_)) ) {
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::attachInterrupt - Unable to allocate storage for interrupt!");
#endif
    } else {
        // Store the interrupt parameters
        _pin_isr_cache[pin_].context = context_;
        _pin_isr_cache[pin_].isr = isr_;
        _pin_isr_cache[pin_].mode = mode_;
    }

    return error;
}

int
FirmataDevice::_detach (
    void
) {
    // Unsubscribe from serial data
    _stream.registerSerialEventCallback(nullptr, nullptr);

    return 0;
}

int
FirmataDevice::_detachInterrupt (
    size_t pin_
) {
    int error;

    if ( pin_ >= _pin_isr_count ) {
        error = errno = ENXIO;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::detachInterrupt - Pin out of bounds!");
#endif
    } else {
        error = 0;

        // Clear the interrupt parameters
        _pin_isr_cache[pin_].context = nullptr;
        _pin_isr_cache[pin_].isr = nullptr;
        _pin_isr_cache[pin_].mode = wiring::LOW;
    }

    return error;
}

bool
FirmataDevice::_digitalRead (
    const size_t pin_
) {
    bool result;

    if ( pin_ >= _pin_count ) {
        result = false;
        errno = ENXIO;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::digitalRead - Pin out of bounds!");
#endif
    } else if ( _pin_info_cache && !_pin_info_cache[pin_].digitalReadAvailable() ) {
        result = false;
        errno = EOPNOTSUPP;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::digitalRead - Pin incapable of digital read functionality!");
#endif
    } else if ( DIGITAL_READ != _pin_state_cache[pin_].mode
      && DIGITAL_READ_WITH_PULLUP != _pin_state_cache[pin_].mode ) {
        result = false;
        errno = EPERM;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::digitalRead - Pin mode not set for digital read!");
#endif
    } else {
        result = _pin_state_cache[pin_].value;
    }

    return result;
}

int
FirmataDevice::_digitalWrite (
    const size_t pin_,
    const bool value_
) {
    int error;

    if ( pin_ >= _pin_count ) {
        error = errno = ENXIO;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::digitalWrite - Pin out of bounds!");
#endif
    } else if ( _pin_info_cache && !_pin_info_cache[pin_].digitalWriteAvailable() ) {
        error = errno = EOPNOTSUPP;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::digitalWrite - Pin incapable of digital write functionality!");
#endif
    } else if ( PIN_MODE_UNSPECIFIED == _pin_state_cache[pin_].mode ) {
        error = 0;
        _pin_state_cache[pin_].mode = DIGITAL_WRITE;
        _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata::PIN_MODE_OUTPUT);
        _marshaller.sendDigital(static_cast<uint8_t>(pin_), value_);
        _stream.flush();
    } else if ( DIGITAL_WRITE != _pin_state_cache[pin_].mode ) {
        error = errno = EPERM;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::digitalWrite - Pin not set to output!");
#endif
    } else {
        error = 0;
        _marshaller.sendDigital(static_cast<uint8_t>(pin_), value_);
        _stream.flush();
    }

    return error;
}

const char *
FirmataDevice::_firmware (
    void
) {
    return _firmware_name;
}

int
FirmataDevice::_pinMode (
    const size_t pin_,
    const size_t mode_
) {
    int error;
    int firmata_mode;

    if ( _pin_info_cache ) {
        if ( pin_ >= _pin_count ) {
            firmata_mode = firmata::PIN_MODE_IGNORE;
            error = errno = ENXIO;
#ifdef LOG_ERRORS
            ::perror("FirmataDevice::pinMode - Pin out of bounds!");
#endif
        } else {
            switch (mode_) {
              case wiring::OUTPUT:
                if ( _pin_info_cache[pin_].analogWriteAvailable()
                  && _pin_info_cache[pin_].digitalWriteAvailable() ) {
                    // Store a general output mode and do nothing. Then determine
                    // if the intent is either digital or analog output when the
                    // corresponding API is called, and update mode at that time.
                    _pin_state_cache[pin_].mode = PIN_MODE_UNSPECIFIED;
                    firmata_mode = firmata::PIN_MODE_IGNORE;
                    error = 0;
                } else if ( _pin_info_cache[pin_].analogWriteAvailable() ) {
                    _pin_state_cache[pin_].mode = ANALOG_WRITE;
                    firmata_mode = firmata::PIN_MODE_PWM;
                    error = 0;
                } else if ( _pin_info_cache[pin_].digitalWriteAvailable() ) {
                    _pin_state_cache[pin_].mode = DIGITAL_WRITE;
                    firmata_mode = firmata::PIN_MODE_OUTPUT;
                    error = 0;
                } else {
                    firmata_mode = firmata::PIN_MODE_IGNORE;
                    error = errno = EPERM;
#ifdef LOG_ERRORS
                    ::perror("FirmataDevice::pinMode - Pin incapable of output.");
#endif
                }
                break;
              case wiring::INPUT:
                if ( _pin_info_cache[pin_].digitalReadAvailable() ) {
                    _pin_state_cache[pin_].mode = DIGITAL_READ;
                    _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata::PIN_MODE_INPUT);
                    _marshaller.reportDigitalPortEnable(pin_ / 8);
                    _stream.flush();
                    error = 0;
                } else {
                    error = errno = EPERM;
#ifdef LOG_ERRORS
                    ::perror("FirmataDevice::pinMode - Pin incapable of input.");
#endif
                }
                firmata_mode = firmata::PIN_MODE_IGNORE;
                break;
              case wiring::INPUT_PULLUP:
                if ( _pin_info_cache[pin_].digitalReadPullupAvailable() ) {
                    _pin_state_cache[pin_].mode = DIGITAL_READ_WITH_PULLUP;
                    _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata::PIN_MODE_PULLUP);
                    _marshaller.reportDigitalPortEnable(pin_ / 8);
                    _stream.flush();
                    error = 0;
                } else {
                    error = errno = EPERM;
#ifdef LOG_ERRORS
                    ::perror("FirmataDevice::pinMode - Pin not configured with pull-up resistor.");
#endif
                }
                firmata_mode = firmata::PIN_MODE_IGNORE;
                break;
              default:
                firmata_mode = firmata::PIN_MODE_IGNORE;
                error = errno = EINVAL;
#ifdef LOG_ERRORS
                ::perror("FirmataDevice::pinMode - Unrecognized mode sent to device!");
#endif
            }
        }
    } else {
        if ( 0 != updateStorageForPin(pin_) ) {
            firmata_mode = firmata::PIN_MODE_IGNORE;
            error = errno = ENOMEM;
#ifdef LOG_ERRORS
            ::perror("FirmataDevice::pinMode - Unable to allocate storage for pin!");
#endif
        } else {
            switch (mode_) {
              case wiring::OUTPUT:
                // Store a general output mode and do nothing. Then determine
                // if the intent is either digital or analog output when the
                // corresponding API is called, and update mode at that time.
                _pin_state_cache[pin_].mode = PIN_MODE_UNSPECIFIED;
                firmata_mode = firmata::PIN_MODE_IGNORE;
                error = 0;
                break;
              case wiring::INPUT:
                _pin_state_cache[pin_].mode = DIGITAL_READ;
                    _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata::PIN_MODE_INPUT);
                    _marshaller.reportDigitalPortEnable(pin_ / 8);
                    _stream.flush();
                    firmata_mode = firmata::PIN_MODE_IGNORE;
                    error = 0;
                break;
              case wiring::INPUT_PULLUP:
                _pin_state_cache[pin_].mode = DIGITAL_READ_WITH_PULLUP;
                    _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata::PIN_MODE_PULLUP);
                    _marshaller.reportDigitalPortEnable(pin_ / 8);
                    _stream.flush();
                    firmata_mode = firmata::PIN_MODE_IGNORE;
                    error = 0;
                break;
              default:
                firmata_mode = firmata::PIN_MODE_IGNORE;
                error = errno = EINVAL;
#ifdef LOG_ERRORS
                ::perror("FirmataDevice::pinMode - Unrecognized mode sent to device!");
#endif
            }
        }
    }

    // Always send, even when desired mode equals the cached mode, to account for transmission failure
    if ( firmata::PIN_MODE_IGNORE != firmata_mode ) { _marshaller.sendPinMode(static_cast<uint8_t>(pin_), firmata_mode); _stream.flush(); }
    return error;
}

int
FirmataDevice::_refresh (
    signal_t uponRefresh_,
    void * const context_
) {
    _refresh_mutex.try_lock_for(std::chrono::milliseconds(REMOTE_DEVICE_TIMEOUT_MS));
    _uponRefresh = uponRefresh_;
    _refresh_context = context_;
    for (size_t pin = 0 ; pin < _pin_count ; ++pin) {
        _marshaller.sendPinStateQuery(pin);
    }
    _stream.flush();
    return 0;
}

int
FirmataDevice::_reset (
    signal_t uponReset_,
    void * context_
) {
    _report_on_query = !_jit_input;
    _marshaller.systemReset();
    _stream.flush();

    // Update local volatile state to reflect device reset state
    for (size_t pin = 0 ; pin < _pin_count ; ++pin ) {
        if ( _pin_info_cache[pin].analogReadAvailable() ) {
            _pin_state_cache[pin].mode = ANALOG_READ;
        } else if ( _pin_info_cache[pin].digitalWriteAvailable() ) {
            _pin_state_cache[pin].mode = DIGITAL_WRITE;
        } else {
            _pin_state_cache[pin].mode = PIN_MODE_UNSPECIFIED;
        }
        _pin_state_cache[pin].value = 0;
    }

    if ( uponReset_ ) { uponReset_(context_); }
    return 0;
}

int
FirmataDevice::_samplingInterval (
    size_t interval_ms_
) {
    _marshaller.setSamplingInterval(static_cast<uint16_t>(interval_ms_));
    _stream.flush();
    return 0;
}

int
FirmataDevice::_survey (
    signal_t uponSurvey_,
    void * context_
) {
    _uponSurvey = uponSurvey_;
    _survey_context = context_;
    _marshaller.sendCapabilityQuery();
    _stream.flush();
    return 0;
}

const SemVer *
FirmataDevice::_version (
    void
) {
    return &_firmware_semantic_version;
}

void
FirmataDevice::analogReadCallback (
    void * context_,
    uint8_t command_,
    uint16_t value_
) {
    FirmataDevice * device = reinterpret_cast<FirmataDevice *>(context_);
    const size_t pin_transform = (0x0F & command_);
    size_t pin;

    if ( device->_pin_info_cache ) {
        for (pin = 0 ; pin < device->_pin_count ; ++pin) {
            if ( pin_transform == device->_pin_info_cache[pin].reserved ) {
                break;
            }
        }
    } else {
        pin = pin_transform;
    }

    if (pin < device->_pin_count) { device->_pin_state_cache[pin].value = value_; }
}

void
FirmataDevice::digitalReadCallback (
    void * context_,
    uint8_t command_,
    uint16_t value_
) {
    FirmataDevice * device = reinterpret_cast<FirmataDevice *>(context_);
    const uint8_t port = (0x0F & command_);

    const size_t pin_offset = (port << 3);
    const int capture_count = ((device->_pin_count >= (pin_offset + 8)) ? 8 : (device->_pin_count - pin_offset));

    for (int i = 0 ; i < capture_count ; ++i) {
        device->_pin_state_cache[(pin_offset + i)].value = ((1 << i) & value_);

        // Invoke user-defined ISR (if registered)
        if ( (device->_pin_isr_count > (pin_offset + i)) && (nullptr != device->_pin_isr_cache[(pin_offset + i)].isr) ) {
            if ( (wiring::CHANGE == device->_pin_isr_cache[(pin_offset + i)].mode)
              || (value_ && ((wiring::HIGH == device->_pin_isr_cache[(pin_offset + i)].mode) || (wiring::RISING == device->_pin_isr_cache[(pin_offset + i)].mode)))
              || (!value_ && ((wiring::LOW == device->_pin_isr_cache[(pin_offset + i)].mode) || (wiring::FALLING == device->_pin_isr_cache[(pin_offset + i)].mode)))
            ) {
                device->_pin_isr_cache[(pin_offset + i)].isr(device->_pin_isr_cache[(pin_offset + i)].context);
            }
        }
    }
}

void
FirmataDevice::extendBuffer (
    void * context_
) {
    FirmataDevice * device = reinterpret_cast<FirmataDevice *>(context_);
#ifdef LOG
    printf("\n%lu-byte buffer exhausted!\n", device->_parser_buffer_size);
#endif
    uint8_t * temp_buffer;
    size_t temp_buffer_size = (device->_parser_buffer_size * 2);

    // Double parser buffer allocation
    temp_buffer = reinterpret_cast<uint8_t *>(realloc(device->_parser_buffer, temp_buffer_size));
    if ( nullptr != temp_buffer ) {
        device->_parser_buffer = temp_buffer;
        device->_parser_buffer_size = temp_buffer_size;
        (void)device->_parser.setDataBufferOfSize(device->_parser_buffer, device->_parser_buffer_size);
#ifdef LOG
        printf("Buffer increased to %lu-byte buffer.\n", device->_parser_buffer_size);
#endif
    }
}

void
FirmataDevice::firmwareVersionCallback (
    void * context_,
    size_t sv_major_,
    size_t sv_minor_,
    const char * firmware_
) {
    FirmataDevice * device = static_cast<FirmataDevice *>(context_);
    device->_firmware_semantic_version = SemVer(sv_major_, sv_minor_, 0);

    // Duplicate description
    device->_firmware_name = new char[::strlen(firmware_)];
    ::strcpy(device->_firmware_name, firmware_);

    device->_uponAttach(device->_attach_context);
}

void
FirmataDevice::protocolVersionCallback (
    void * context_,
    size_t sv_major_,
    size_t sv_minor_,
    const char * description_
) {
    FirmataDevice * device = static_cast<FirmataDevice *>(context_);
    (void)description_;

    device->_protocol_semantic_version = SemVer(sv_major_, sv_minor_, 0);
}

void
FirmataDevice::serialEventCallback (
    void * context_
) {
    FirmataDevice * device = reinterpret_cast<FirmataDevice *>(context_);

    uint8_t incoming_byte;
    for (; device->_stream.available() ; device->_parser.parse(incoming_byte)) {
        incoming_byte = device->_stream.read();
#ifdef LOG
        printf("0x%02x ", incoming_byte);
#endif
    }
}

void
FirmataDevice::sysexCallback (
    void * context_,
    uint8_t command_,
    size_t argc_,
    uint8_t * argv_
) {
    FirmataDevice * device = static_cast<FirmataDevice *>(context_);

    switch (command_) {
      case firmata::ANALOG_MAPPING_RESPONSE:
      {
        // Parse analog mapping response into WiringPinInfo
        for (size_t i = 0 ; i < argc_ ; ++i) {
            device->_pin_info_cache[i].reserved = argv_[i];
#ifdef LOG
            printf("\nWiringPinInfo %u:\n\tsupported modes: 0x%02x\n\tanalog read resolution bits: %u\n\tanalog write resolution bits: %u\n\tfirmata analog mapping: %u\n\n", static_cast<unsigned int>(i), static_cast<uint8_t>(device->_pin_info_cache[i].supported_modes), static_cast<uint8_t>(device->_pin_info_cache[i].analog_read_resolution_bits), static_cast<uint8_t>(device->_pin_info_cache[i].analog_write_resolution_bits), static_cast<uint8_t>(device->_pin_info_cache[i].reserved));
#endif
        }

        device->_refresh(device->_uponSurvey, device->_survey_context);
        break;
      }
      case firmata::CAPABILITY_RESPONSE:
      {
        bool analog_resolution = false;
        bool mode_byte = true;
        bool pwm_resolution = false;
        device->_pin_count = 0;
        WiringPinInfo pin_data = { 0, 0, 0, 0 };

        // Parse capability response into WiringPinInfo
        for (size_t i = 0 ; i < argc_ ; ++i, mode_byte = !mode_byte) {
            if ( mode_byte ) {
                switch (argv_[i]) {
                  case firmata::PIN_MODE_ANALOG:
                    pin_data.supported_modes |= ANALOG_READ;
                    analog_resolution = true;
                    break;
                  case firmata::PIN_MODE_IGNORE:
                    ++device->_pin_count;
                    device->_pin_info_cache = (WiringPinInfo *)realloc(device->_pin_info_cache, (sizeof(WiringPinInfo) * device->_pin_count));
                    device->_pin_info_cache[(device->_pin_count - 1)] = pin_data;
                    pin_data.supported_modes = 0;
                    pin_data.analog_read_resolution_bits = 0;
                    pin_data.analog_write_resolution_bits = 0;
                    pin_data.reserved = 0;
                    mode_byte = !mode_byte;
                    break;
                  case firmata::PIN_MODE_INPUT:
                    pin_data.supported_modes |= DIGITAL_READ;
                    break;
                  case firmata::PIN_MODE_OUTPUT:
                    pin_data.supported_modes |= DIGITAL_WRITE;
                    break;
                  case firmata::PIN_MODE_PULLUP:
                    pin_data.supported_modes |= DIGITAL_READ_WITH_PULLUP;
                    break;
                  case firmata::PIN_MODE_PWM:
                    pin_data.supported_modes |= ANALOG_WRITE;
                    pwm_resolution = true;
                    break;
                }
            } else if ( analog_resolution ) {
                pin_data.analog_read_resolution_bits = argv_[i];
                analog_resolution = false;
            } else if ( pwm_resolution ) {
                pin_data.analog_write_resolution_bits = argv_[i];
                pwm_resolution = false;
            }
        }
        // Allocate storage for volatile pin status
        device->_pin_state_cache = new WiringPinState[device->_pin_count];

        // Query analog pin mapping
        device->_marshaller.sendAnalogMappingQuery();
        device->_stream.flush();
        break;
      }
      case firmata::I2C_REPLY:
      {
        // Handle response from I²C slave device
        i2c_transaction_t i2c_transaction;

        // Reassemble transaction details
        i2c_transaction.address_base = argv_[0];
        i2c_transaction.header.data = argv_[1];

        // Check for 10-bit address
        if ( i2c_transaction.header.config.address_mode_10_bit ) {
            errno = EFAULT;
#ifdef LOG_ERRORS
            ::perror("ERROR: Wire::requestFrom - 10-bit addresses not supported!");
#endif
            reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx.push_back(argv_[0]);
            reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx.push_back(argv_[1]);
        } else {
            /**
             * Cache data bytes
             *
             * Address: argv_[0] and argv_[1]
             * Register: argv_[2] and argv_[3]
             *
             * NOTE: The Wiring API only returns the data portion
             *       of the payload, which begins at index 4
             */
            for ( size_t i = 4 ; (i + 1) < argc_ ; i += 2 ) {
                uint8_t i2c_byte = argv_[i];
                i2c_byte |= static_cast<uint8_t>((argv_[i + 1] << 7) & 0x80);
                reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx.push_back(i2c_byte);
            }
        }

        // Signal master with response size
        reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx_mutex.lock();
        if ( reinterpret_cast<FirmataI2c * const>(&device->Wire)->_i2c_slave_response_result ) {
            reinterpret_cast<FirmataI2c * const>(&device->Wire)->_i2c_slave_response_result->set_value(reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx.size());
        }
        reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx_mutex.unlock();

        break;
      }
      case firmata::I2C_REQUEST:
      {
        // Handle request from I²C master device
        i2c_transaction_t i2c_transaction;

        // Reassemble transaction details
        i2c_transaction.address_base = argv_[0];
        i2c_transaction.header.data = argv_[1];

        if ( reinterpret_cast<FirmataI2c * const>(&device->Wire)->_i2c_slave_address == i2c_transaction.address_base ) {
            // Cache data bytes
            for ( size_t i = 2 ; (i + 1) < argc_ ; i += 2 ) {
                uint8_t i2c_byte = argv_[i];
                i2c_byte |= (argv_[i + 1] << 7);
                reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx.push_back(i2c_byte);
            }

            // Call corresponding callback
            if ( i2c_transaction.header.config.read_mode && reinterpret_cast<FirmataI2c * const>(&device->Wire)->_onRequestHandler ) {
                reinterpret_cast<FirmataI2c * const>(&device->Wire)->_onRequestHandler(reinterpret_cast<FirmataI2c * const>(&device->Wire)->_onRequestHandler_context);
            } else if ( !i2c_transaction.header.config.read_mode && reinterpret_cast<FirmataI2c * const>(&device->Wire)->_onReceiveHandler ) {
                reinterpret_cast<FirmataI2c * const>(&device->Wire)->_onReceiveHandler(reinterpret_cast<FirmataI2c * const>(&device->Wire)->_onReceiveHandler_context, (reinterpret_cast<FirmataI2c * const>(&device->Wire)->_rx.size()));
            }
        } else {
            errno = ENXIO;
#ifdef LOG_ERRORS
            ::perror("ERROR: Wire::request - Unrecognized I2C address!");
#endif
        }
        break;
      }
      case firmata::PIN_STATE_RESPONSE:
      {
        // Parse pin state response into WiringPinState
        if ( 3 > argc_ ) {
            errno = EILSEQ;
#ifdef LOG_ERRORS
            ::perror("FirmataDevice::sysexCallback - PIN_STATE_RESPONSE - Parsed data is malformed!");
#endif
        } else if ( argv_[0] > device->_pin_count ) {
            errno = ENXIO;
#ifdef LOG_ERRORS
            ::perror("FirmataDevice::sysexCallback - PIN_STATE_RESPONSE - Pin number is out of bounds!");
#endif
        } else {
            switch (argv_[1]) {
              case firmata::PIN_MODE_ANALOG:
                if ( device->_report_on_query ) {
                    device->_pin_state_cache[argv_[0]].mode = ANALOG_READ;
                    device->_marshaller.reportAnalogEnable(argv_[0]);  // Arduino will report pin number as 4-bit transform
                } else {
                    device->_pin_state_cache[argv_[0]].mode = PIN_MODE_UNSPECIFIED;
                    device->_marshaller.reportAnalogDisable(argv_[0]);
                }
                device->_stream.flush();
                break;
              case firmata::PIN_MODE_INPUT:
                if ( device->_report_on_query ) {
                    device->_pin_state_cache[argv_[0]].mode = DIGITAL_READ;
                    device->_marshaller.sendPinMode(argv_[0], firmata::PIN_MODE_INPUT);
                } else {
                    device->_pin_state_cache[argv_[0]].mode = PIN_MODE_UNSPECIFIED;
                    device->_marshaller.reportDigitalPortDisable(argv_[0] / 8);
                }
                device->_stream.flush();
                break;
              case firmata::PIN_MODE_OUTPUT:
                device->_pin_state_cache[argv_[0]].mode = DIGITAL_WRITE;
                break;
              case firmata::PIN_MODE_PULLUP:
                if ( device->_report_on_query ) {
                    device->_pin_state_cache[argv_[0]].mode = DIGITAL_READ_WITH_PULLUP;
                    device->_marshaller.sendPinMode(argv_[0], firmata::PIN_MODE_PULLUP);
                } else {
                    device->_pin_state_cache[argv_[0]].mode = PIN_MODE_UNSPECIFIED;
                    device->_marshaller.reportDigitalPortDisable(argv_[0] / 8);
                }
                device->_stream.flush();
                break;
              case firmata::PIN_MODE_PWM:
                device->_pin_state_cache[argv_[0]].mode = ANALOG_WRITE;
                break;
              default:
                errno = EINVAL;
#ifdef LOG_ERRORS
                ::perror("FirmataDevice::sysexCallback - PIN_STATE_RESPONSE - Unexpected mode encountered!");
#endif
                device->_pin_state_cache[argv_[0]].mode = PIN_MODE_UNSPECIFIED;
            }

            // WiringPinState only supports two-byte values
            if (1 == (argc_ - 2)) {
                device->_pin_state_cache[argv_[0]].value = argv_[2];
            } else {
                device->_pin_state_cache[argv_[0]].value = ((argv_[2] << 7) | argv_[3]);
            }
#ifdef LOG
            printf("\nWiringPinState %u:\n\tcurrent mode: 0x%02x\n\tpin value: %u\n\n", static_cast<unsigned int>(argv_[0]), static_cast<uint8_t>(device->_pin_state_cache[argv_[0]].mode), static_cast<uint16_t>(device->_pin_state_cache[argv_[0]].value));
#endif
            // Call Wiring::refresh() callback on last pin
            if ( argv_[0] == (device->_pin_count - 1) ) {
                device->_report_on_query = false;
                if ( device->_uponRefresh ) {
                    device->_refresh_mutex.unlock();
                    device->_uponRefresh(device->_refresh_context);
                }
            }
        }
        break;
      }
      default: break;
    }
}

int
FirmataDevice::updateStorageForPin (
    const size_t pin_
) {
    int error;

    if (pin_ < _pin_count) {
        error = 0;
    } else if ( nullptr == (_pin_state_cache = (WiringPinState *)realloc(_pin_state_cache, ((pin_ + 1) * sizeof(WiringPinState)))) ) {
        error = errno = ENOMEM;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::updateStorageForPinInterrupt - Unable to allocate pin cache!");
#endif
    } else {
        error = 0;
        _pin_count = (pin_ + 1);
    }

    return error;
}

int
FirmataDevice::updateStorageForPinInterrupt (
    const size_t pin_
) {
    int error;

    if (pin_ < _pin_isr_count) {
        error = 0;
    } else if ( nullptr == (_pin_isr_cache = (WiringPinInterrupt *)realloc(_pin_isr_cache, ((pin_ + 1) * sizeof(WiringPinInterrupt)))) ) {
        error = errno = ENOMEM;
#ifdef LOG_ERRORS
        ::perror("FirmataDevice::updateStorageForPinInterrupt - Unable to allocate interrupt pin cache!");
#endif
    } else {
        error = 0;
        _pin_isr_count = (pin_ + 1);
    }
    
    return error;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
