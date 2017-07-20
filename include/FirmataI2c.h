/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef FIRMATA_I2C_H
#define FIRMATA_I2C_H

#include <cstddef>
#include <cstdint>
#include <deque>
#include <future>
#include <mutex>
#include <vector>

#include "Stream.h"
#include "Wire.h"

namespace remote_wiring {

typedef struct i2c_config_t {
    uint8_t address_ext : 3;
    uint8_t read_mode : 2;
    uint8_t address_mode_10_bit : 1;
    uint8_t restart : 1;
    uint8_t reserved : 1;
} i2c_header_t;

union i2c_header_u {
    i2c_config_t config;
    uint8_t data;
};

typedef struct i2c_transaction_t {
    uint8_t address_base : 7;
    uint8_t reserved : 1;
    i2c_header_u header;
} i2c_transaction_t;

/*
 * \brief Firmata implementation of I²C / TWI devices
 *
 * \warning Firmata only supports I²C master mode
 *
 * \sa <a href="https://github.com/firmata/protocol/blob/master/i2c.md">Firmata I²C Protocol</a>
 */
class FirmataI2c : public TwoWire {
    friend FirmataDevice;
  public:
    virtual
    ~FirmataI2c (
        void
    );

  private:
    uint8_t _i2c_slave_address;
    std::promise<size_t> * _i2c_slave_response_result;
    i2c_transaction_t _i2c_transaction;
    wire_event_t _onReceiveHandler;
    void * _onReceiveHandler_context;
    signal_t _onRequestHandler;
    void * _onRequestHandler_context;
    std::deque<uint8_t> _rx;
    std::mutex _rx_mutex;
    Stream & _stream;
    std::vector<uint8_t> _tx;

    FirmataI2c (
        Stream & stream_
    );

    size_t
    _available (
        void
    ) override;

    int
    _begin (
        uint8_t address_
    ) override;

    int
    _beginTransmission (
        uint8_t address_
    ) override;

    int
    _end (
        void
    ) override;

    int
    _endTransmission (
        bool stop_
    ) override;

    int
    _flush (
        void
    ) override;

    int
    _onReceive (
        wire_event_t handler_,
        void * context_
    ) override;

    int
    _onRequest (
        signal_t handler_,
        void * context_
    ) override;

    int
    _read (
        void
    ) override;

    size_t
    _requestFrom (
        uint8_t address_,
        size_t quantity_,
        bool stop_
    ) override;

    int
    _setClock (
        size_t freq_Hz_
    ) override;

    size_t
    _write (
        uint8_t byte_
    ) override;
};

} // namespace remote_wiring

#endif

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
