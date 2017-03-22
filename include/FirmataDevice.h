/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef FIRMATA_DEVICE_H
#define FIRMATA_DEVICE_H

#include <cstddef>
#include <cstdlib>
#include <mutex>

#include <FirmataMarshaller.h>
#include <FirmataParser.h>
#include <serial_wiring>

#include "RemoteDevice.h"
#include "RwTypes.h"
#include "WiringPin.h"

namespace remote_wiring {

class FirmataDevice : public RemoteDevice {
  public:
    /*!
     * \brief Create a remote device conforming to the Wiring API
     *
     * The default constructor creates an undefined representation of a remote device.
     * "Undefined" meaning the telemorph has no concept of the hardware it is modeling.
     * As such, a best effort attempt to deliver messages will be made. Unfortunately,
     * there will be no ability for error checking, and undefined behavior will occur.
     *
     * \param [in] stream_ The underlying transport layer
     *
     * \note An undefined device does not require any inbound messages, which can help
     *       debug a poor connection or malconfigured remote device.
     * \note Use `FirmataDevice::survey` to promote the telemorph to a defined representation.
     *
     * \sa remote_wiring::telemorph::survey
     */
    FirmataDevice (
        Stream & stream_
    );

    ~FirmataDevice (
        void
    );

  private:
    void * _attach_context;
    char * _firmware_name;
    SemVer _firmware_semantic_version;
    firmata::FirmataMarshaller _marshaller;
    firmata::FirmataParser _parser;
    uint8_t * _parser_buffer;
    size_t _parser_buffer_size;
    size_t _pin_count;
    WiringPinInfo * _pin_info_cache;
    WiringPinState * _pin_state_cache;
    SemVer _protocol_semantic_version;
    void * _refresh_context;
    std::timed_mutex _refresh_mutex;
    Stream & _stream;
    void * _survey_context;
    signal_t _uponAttach;
    signal_t _uponRefresh;
    signal_t _uponSurvey;

    size_t
    _analogRead (
        const size_t pin_
    ) override;

    void
    _analogWrite (
        const size_t pin_,
        const uint8_t value_
    ) override;

    int
    _attach (
        signal_t uponAttach_,
        void * context_
    ) override;

    void
    _detach (
        void
    ) override;

    bool
    _digitalRead (
        const size_t pin_
    ) override;

    void
    _digitalWrite (
        const size_t pin_,
        const bool value_
    ) override;

    const char *
    _firmware (
        void
    ) override;

    void
    _pinMode (
        const size_t pin_,
        const size_t mode_
    ) override;

    int
    _refresh (
        signal_t uponRefresh_,
        void * context_
    ) override;

    int
    _reset (
        signal_t uponSoftReset_,
        void * context_
    ) override;

    int
    _survey (
        signal_t uponSurvey_,
        void * context_
    ) override;

    SemVer
    _version (
        void
    ) override;

    static
    void
    analogReadCallback (
        void * context_,
        uint8_t command_,
        uint16_t value_
    );

    static
    void
    digitalReadCallback (
        void * context_,
        uint8_t command_,
        uint16_t value_
    );

    static
    void
    extendBuffer (
        void * context_
    );

    static
    void
    firmwareVersionCallback (
        void * context_,
        size_t sv_major_,
        size_t sv_minor_,
        const char * firmware_
    );

    static
    void
    protocolVersionCallback (
        void * context_,
        size_t sv_major_,
        size_t sv_minor_,
        const char * description_
    );

    static
    void
    serialEventCallback (
        void * context_
    );

    static
    void
    sysexCallback (
        void * context_,
        uint8_t command_,
        size_t argc_,
        uint8_t * argv_
    );

    inline
    int
    updateStorageForPin (
        const size_t pin_
    ) {
        int error;
        if (pin_ < _pin_count) {
            error = 0;
        } else if ( nullptr == (_pin_state_cache = (WiringPinState *)realloc(_pin_state_cache, ((pin_ + 1) * sizeof(WiringPinState)))) ) {
            error = __LINE__;
        } else {
            error = 0;
            _pin_count = (pin_ + 1);
        }
        return error;
    }
};

} // namespace remote_wiring

#endif // FIRMATA_DEVICE_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

