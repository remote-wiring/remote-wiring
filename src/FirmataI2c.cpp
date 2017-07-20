/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "FirmataI2c.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <FirmataConstants.h>

#include "RwConstants.h"

using namespace remote_wiring;

FirmataI2c::FirmataI2c (
    Stream & stream_
) :
    _i2c_slave_address(0x00),
    _i2c_slave_response_result(nullptr),
    _i2c_transaction{0, 0, {0, 0, 0, 0, 0}},
    _onReceiveHandler(nullptr),
    _onReceiveHandler_context(nullptr),
    _onRequestHandler(nullptr),
    _onRequestHandler_context(nullptr),
    _stream(stream_)
{ }

FirmataI2c::~FirmataI2c (
    void
) { }

size_t
FirmataI2c::_available (
    void
) {
    std::lock_guard<std::mutex> lock(_rx_mutex);
    return _rx.size();
}

int
FirmataI2c::_begin (
    uint8_t address_
) {
    _i2c_slave_address = address_;

    _rx_mutex.lock();
    _rx.clear();
    _rx_mutex.unlock();

    _tx.clear();

    // Send message to configure Firmata for I²C
    _stream.write(firmata::START_SYSEX);
    _stream.write(firmata::I2C_CONFIG);
    _stream.write(0x00);
    _stream.write(0x00);
    _stream.write(firmata::END_SYSEX);
    _stream.flush();

    return 0;
}

int
FirmataI2c::_beginTransmission (
    uint8_t address_
) {
    // Configure transaction
    _i2c_transaction.header.config.read_mode = 0;
    _i2c_transaction.address_base = address_;

    // Clear transmit buffer
    _tx.clear();

    return 0;
}

int
FirmataI2c::_end (
    void
) {
    // Preserve outstanding bytes, but minimize space requirement
    _rx_mutex.lock();
    _rx.shrink_to_fit();
    _rx_mutex.unlock();

    _tx.shrink_to_fit();

    // Detach any callbacks from `TwoWire` interface
    _onReceiveHandler = nullptr;
    _onReceiveHandler_context = nullptr;
    _onRequestHandler = nullptr;
    _onRequestHandler_context = nullptr;

    return 0;
}

int
FirmataI2c::_endTransmission (
    bool stop_
) {
    // Finalize transaction
    _i2c_transaction.header.config.restart = !stop_;

    // Transmit data
    _stream.write(firmata::START_SYSEX);
    _stream.write(firmata::I2C_REQUEST);
    _stream.write(_i2c_transaction.address_base);
    _stream.write(_i2c_transaction.header.data);
    std::for_each(_tx.cbegin(), _tx.cend(), [&](const uint8_t & data_byte_) {
        _stream.write(data_byte_ & 0x7F);
        _stream.write((data_byte_ >> 7) & 0x01);
    });
    _stream.write(firmata::END_SYSEX);
    _stream.flush();

    return 0;
}

int
FirmataI2c::_flush (
    void
) {
    int result; 

    // Check for slave mode
    if ( 0x77 == _i2c_slave_address ) {
        result = errno = EBADMSG;
#ifdef LOG_ERRORS
        ::perror("ERROR: FirmataI2c::flush - Function not available in master mode!");
#endif
    } else {
        // Transmit data
        _stream.write(firmata::START_SYSEX);
        _stream.write(firmata::I2C_REPLY);
        _stream.write(_i2c_slave_address & 0x7F);
        _stream.write((_i2c_slave_address >> 7) & 0x01);
        std::for_each(_tx.cbegin(), _tx.cend(), [&](const uint8_t & data_byte_) {
            _stream.write(data_byte_ & 0x7F);
            _stream.write((data_byte_ >> 7) & 0x01);
        });
        _stream.write(firmata::END_SYSEX);
        _stream.flush();
        result = 0;
    }

    return result;
}

int
FirmataI2c::_onReceive (
    wire_event_t handler_,
    void * context_
) {
    _onReceiveHandler = handler_;
    _onReceiveHandler_context = context_;

    return 0;
}

int
FirmataI2c::_onRequest (
    signal_t handler_,
    void * context_
) {
    _onRequestHandler = handler_;
    _onRequestHandler_context = context_;

    return 0;
}

int
FirmataI2c::_read (
    void
) {
    int result = -1;

    std::lock_guard<std::mutex> lock(_rx_mutex);
    if ( !_rx.empty() ) {
        result = _rx.front();
        _rx.pop_front();
    }

    return result;
}

size_t
FirmataI2c::_requestFrom (
    uint8_t address_,
    size_t quantity_,
    bool stop_
) {
    size_t result;

    // Enable blocking behavior
    std::promise<size_t> callback_signal;
    std::future<size_t> callback_gate(callback_signal.get_future());

    // Configure transaction
    _i2c_transaction.header.config.restart = !stop_;
    _i2c_transaction.header.config.read_mode = 1;
    _i2c_transaction.address_base = address_;

    // Ready the receive buffer
    _rx_mutex.lock();
    _rx.clear();
    _i2c_slave_response_result = &callback_signal;
    _rx_mutex.unlock();

    // Transmit request
    _stream.write(firmata::START_SYSEX);
    _stream.write(firmata::I2C_REQUEST);
    _stream.write(_i2c_transaction.address_base);
    _stream.write(_i2c_transaction.header.data);
    _stream.write(quantity_ & 0x7F);
    _stream.write((quantity_ >> 7) & 0x01);
    _stream.write(firmata::END_SYSEX);
    _stream.flush();

    // Await the returned bytes
    switch (callback_gate.wait_for(std::chrono::milliseconds(REMOTE_DEVICE_TIMEOUT_MS))) {
      case std::future_status::ready:
        result = callback_gate.get();
        break;
      case std::future_status::timeout:
        errno = ETIME;
#ifdef LOG_ERRORS
        ::perror("ERROR: invoke_blockable_async_method - Block on future timed out!");
#endif
        result = 0;
        break;
      case std::future_status::deferred:
      default:
        errno = EWOULDBLOCK;
#ifdef LOG_ERRORS
        ::perror("ERROR: invoke_blockable_async_method - Block on future returned unhandled result!");
#endif
        result = 0;
        break;
    }

    // Disable slave response signal
    _rx_mutex.lock();
    _i2c_slave_response_result = nullptr;
    _rx_mutex.unlock();

    return result;
}

int
FirmataI2c::_setClock (
    size_t freq_Hz_
) {
    (void)freq_Hz_;
    int result = errno = ENOTSUP;

#ifdef LOG_ERRORS
        ::perror("ERROR: FirmataI2c::setClock - Not supported in the Firmata protocol!");
#endif
    return result;
}

size_t
FirmataI2c::_write (
    uint8_t byte_
) {
    _tx.push_back(byte_);
    return 1;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
