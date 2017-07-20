/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef WIRE_H
#define WIRE_H

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "RwTypes.h"

namespace remote_wiring {

/*!
 * \brief Callback type supplied to `Wire::onReceive`, which will
 *        be invoked when data is available from the slave device
 *
 * \param [in] context_ Context to use in the callback
 * \param [in] bytes_read_ Number of bytes read from the master
 *
 * \note Use `TwoWire.read()` to capture this data.
 *
 * \sa TwoWire::available
 * \sa TwoWire::onReceive
 * \sa <a href="https://en.wikipedia.org/wiki/I%C2%B2C">I²C</a>
 */
typedef void(*wire_event_t)(void * context_, size_t bytes_read_);

/*
 * \brief This library allows you to communicate with I²C / TWI devices
 *
 * The TWI protocol and interface were developed by Phillips. It is
 * possible to connect multiple TWI devices to the TWI pins creating
 * a network or bus of devices and communicate with them by means of
 * an address. The TWI can be configured to act as a Master or a Slave
 * in a network of devices. There are both 7- and 8-bit versions of
 * I²C addresses. 7 bits identify the device, and the eighth bit
 * determines if it's being written to or read from. The Wire library
 * uses 7 bit addresses throughout. The addresses from 0 to 7 are not
 * used because are reserved so the first address that can be used is 8.
 *
 * \note The SDA/SCL lines must be HIGH when inactive, a pull-up
 *       resistor is needed when connecting SDA/SCL pins.
 *
 * \note If you have a datasheet or sample code that uses 8-bit
 *       address, you'll want to drop the low bit (i.e. shift the
 *       value one bit to the right), yielding an address between
 *       0 and 127.
 */
class TwoWire {
  public:
    /*!
     * \brief Returns the number of bytes available for retrieval
     *
     * Called on a master device after a call to TwoWire::requestFrom
     * or called on a slave device inside the TwoWire::onReceive handler.
     *
     * \return The number of bytes available for reading
     *
     * \sa TwoWire::read
     * \sa <a href="https://www.arduino.cc/en/Reference/WireAvailable">Wire.available (Arduino.cc)</a>
     */
    inline
    size_t
    available (
        void
    ) {
        return _available();
    }

    /*!
     * \brief Join the I²C bus as a master or slave
     *
     * The begin() method is used to initiate the TWI as a Master and
     * the begin(address) method is used to join the TWI bus a slave
     * using the address specified.
     *
     * \param [in] address_ 7-bit slave address of this device; if
     *                      not specified, join the bus as a master
     *
     * \note I²C address 0x77 is reserved for RemoteWiring
     *
     * \sa TwoWire::end
     * \sa <a href="https://www.arduino.cc/en/Reference/WireBegin">Wire.begin (Arduino.cc)</a>
     */
    inline
    void
    begin (
        uint8_t address_ = 0x77
    ) {
        if ( 0x80 & address_ ) {
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (7-bit address required)!");
#endif
        } else switch (address_) {
          case 0x00:
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (address reserved for general call)!");
#endif
            break;
          case 0x01:
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (address reserved for CBUS addresses)!");
#endif
            break;
          case 0x02:
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (address reserved for alternative protocol addresses)!");
#endif
            break;
          case 0x03:
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (address reserved)!");
#endif
            break;
          case 0x04:
          case 0x05:
          case 0x06:
          case 0x07:
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (address reserved for high-speed master code)!");
#endif
            break;
          case 0x78:
          case 0x79:
          case 0x7A:
          case 0x7B:
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (address reserved for 10-bit slave addressing)!");
#endif
            break;
          case 0x7C:
          case 0x7D:
          case 0x7E:
          case 0x7F:
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (address reserved)!");
#endif
            break;
          default:
            {
                int error;
                if ( 0 != (error = _begin(address_)) ) {
                    errno = error;
#ifdef LOG_ERRORS
                    ::perror("ERROR: TwoWire::begin - Underlying implementation encountered error!");
#endif
                }
            }
        }
    }

    /*!
     * \brief Begin a transmission to the I²C slave device
     *
     * Once a transmission has been initiated using `TwoWire::beginTransmission`,
     * bytes will be queued for delivery using `TwoWire::write` and transmitted
     * with a call to `TwoWire::endTransmission`.
     *
     * \param [in] address_ 7-bit address of the slave device
     *
     * \sa TwoWire::endTransmission
     * \sa TwoWire::write
     * \sa <a href="https://www.arduino.cc/en/Reference/WireBeginTransmission">Wire.beginTransmission (Arduino.cc)</a>
     */
    inline
    void
    beginTransmission (
        uint8_t address_
    ) {
        int error;

        if ( 0x80 & address_ ) {
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (7-bit address required)!");
#endif
        } else if ( 0 != (error = _beginTransmission(address_)) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Close connection to the I²C bus
     *
     * \sa TwoWire::begin
     */
    inline
    void
    end (
        void
    ) {
        int error;

        if ( 0 != (error = _end()) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::end - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief End transmission to a slave device
     *
     * By invoking `TwoWire::endTransmission` all bytes queued by
     * `TwoWire::write` will be transmitted to the slave device.
     *
     * \param [in] stop_ Release bus after transmission (default is `true`)
     * \n - `true` - Send a stop message after transmission, releasing the I²C bus.
     * \n - `false` - Send a restart message after transmission, preventing
     *                another master device from transmitting between messages.
     *                This allows a single master to send multiple transmissions
     *                while in control.
     *
     * \return A byte indicating the status of the transmission
     * \n - 0 - success
     * \n - 1 - data too long to fit in transmit buffer
     * \n - 2 - received NACK on transmit of address
     * \n - 3 - received NACK on transmit of data
     * \n - 4 - other error
     *
     * \sa TwoWire::beginTransmission
     * \sa <a href="https://www.arduino.cc/en/Reference/WireEndTransmission">Wire.endTransmission (Arduino.cc)</a>
     */
    inline
    int
    endTransmission (
        bool stop_ = true
    ) {
        int result = _endTransmission(stop_);

        if ( result < 0 || result > 4 ) {
            errno = ERANGE;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::endTransmission - Underlying implementation result out of range!");
#endif
        }

        return result;
    }

    /*!
     * \brief Flush queued bytes
     *
     * Enables a slave device to push a complete response onto the wire.
     * The typical use case would entail calling `TwoWire::flush` after
     * multiple calls to `TwoWire::write` have enqueued a response to a
     * `TwoWire::onRequest` callback.
     *
     * \sa TwoWire::onRequest
     * \sa <a href="https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/libraries/Wire/src/Wire.h#L71">GitHub: Arduino/.../libraries/Wire.h</a>
     */
    inline
    void
    flush (
        void
    ) {
        int error;

        if ( 0 != (error = _flush()) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::flush - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Register a function to be called when a slave device
     *        receives a write request from a master
     *
     * \param [in] handler_ Callback invoked when the slave
     *                      receives a read request
     * \param [in] context_ Optional context provided to `handler_`
     *                      via the `context_` parameter
     *
     * \sa TwoWire::flush
     * \sa TwoWire::requestFrom
     * \sa TwoWire::write
     * \sa <a href="https://www.arduino.cc/en/Reference/WireOnReceive">Wire.onReceive (Arduino.cc)</a>
     */
    inline
    void
    onReceive (
        wire_event_t handler_,
        void * context_ = nullptr
    ) {
        int error;

        if ( !handler_ ) { context_ = nullptr; }

        if ( 0 != (error = _onReceive(handler_, context_)) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::onReceive - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Register a function to be called when a slave device
     *        receives a read request from a master
     *
     * \param [in] handler_ Callback invoked when the slave
     *                      receives a write request
     * \param [in] context_ Optional context provided to `handler_`
     *                      via the `context_` parameter
     *
     * \sa TwoWire::beginTransmission
     * \sa TwoWire::flush
     * \sa TwoWire::write
     * \sa <a href="https://www.arduino.cc/en/Reference/WireOnRequest">Wire.onRequest (Arduino.cc)</a>
     */
    inline
    void
    onRequest (
        signal_t handler_,
        void * context_ = nullptr
    ) {
        int error;

        if ( !handler_ ) { context_ = nullptr; }

        if ( 0 != (error = _onRequest(handler_, context_)) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::onRequest - Underlying implementation encountered error!");
#endif
        }
    }

    /*!
     * \brief Fetches the next byte from the I²C buffer
     *
     * Reads a byte that was transmitted from a slave device to a
     * master after a call to `TwoWire::requestFrom` or was transmitted
     * from a master to a slave.
     *
     * \return The first byte of incoming data available
     *
     * \note Returns -1 if there is no byte, although this should
     *       be avoided by first checking `available()` to see if
     *       data is available.
     *
     * \sa TwoWire::available
     * \sa <a href="https://www.arduino.cc/en/Reference/WireRead">Wire.read (Arduino.cc)</a>
     */
    inline
    int
    read (
        void
    ) {
        int result = _read();

        if ( result < -1 || result > 255 ) {
            errno = ERANGE;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::read - Underlying implementation result out of range!");
#endif
        }

        return result;
    }

    /*!
     * \brief Request bytes from a slave device
     *
     * The request initiates a transaction and when complete,
     * the master can consume the bytes using `TwoWire::available`
     * and `TwoWire::read`.
     *
     * \param [in] address_ 7-bit address of the slave device
     * \param [in] quantity_ Number of bytes requested
     * \param [in] stop_ Release bus after transmission (default is `true`)
     * \n - `true` - Send a stop message after transmission, releasing the I²C bus.
     * \n - `false` - Send a restart message after transmission, preventing
     *                another master device from transmitting between messages.
     *                This allows a single master to send multiple transmissions
     *                while in control.
     *
     * \return The number of bytes returned from the slave device
     *
     * \sa TwoWire::available
     * \sa TwoWire::read
     * \sa <a href="https://www.arduino.cc/en/Reference/WireRequestFrom">Wire.requestFrom (Arduino.cc)</a>
     */
    inline
    size_t
    requestFrom (
        uint8_t address_,
        size_t quantity_,
        bool stop_ = true
    ) {
        size_t result;

        if ( 0x80 & address_ ) {
            result = 0;
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::begin - Invalid address (7-bit address required)!");
#endif
        } else {
            result = _requestFrom(address_, quantity_, stop_);
        }

        return result;
    }

    /*!
     * \brief Modify the clock frequency for I²C communication
     *
     * \param [in] freq_Hz_ The desired clock frequency in Hertz
     * \n Standard frequencies:
     * \n - 10000 (low speed mode)
     * \n - 100000 (standard mode)
     * \n - 400000 (fast mode)
     * \n - 1000000 (fast mode plus)
     * \n - 3400000 (high speed mode)
     *
     * \note I²C slave devices have no minimum working clock
     *       frequency, however 100KHz is usually the baseline.
     *
     * \warning Please refer to the specific processor documentation
     *          to make sure the desired mode is supported.
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/WireSetClock">Wire.setClock (Arduino.cc)</a>
     */
    inline
    void
    setClock (
        size_t freq_Hz_
    ) {
        int error;

        if ( !freq_Hz_ ) {
            errno = EDOM;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::setClock - Frequency cannot be zero!");
#endif
        } else if ( 0 != (error = _setClock(freq_Hz_)) ) {
            errno = error;
#ifdef LOG_ERRORS
            ::perror("ERROR: TwoWire::setClock - Underlying implementation result out of range!");
#endif
        }
    }

    /*!
     * \brief Queues bytes for transmission on I²C bus
     *
     * \param [in] byte_ The byte of data to enqueue
     *
     * \return Number of bytes enqueued
     *
     * \sa TwoWire::endTransmission
     * \sa TwoWire::flush
     * \sa <a href="https://www.arduino.cc/en/Reference/WireWrite">Wire.write (Arduino.cc)</a>
     */
    inline
    size_t
    write (
        uint8_t byte_
    ) {
        return _write(byte_);
    }

  protected:
    ~TwoWire (void) {}

  private:
    virtual
    size_t
    _available (
        void
    ) = 0;

    virtual
    int
    _begin (
        uint8_t address_
    ) = 0;

    virtual
    int
    _beginTransmission (
        uint8_t address_
    ) = 0;

    virtual
    int
    _end (
        void
    ) = 0;

    virtual
    int
    _endTransmission (
        bool stop_
    ) = 0;

    virtual
    int
    _flush (
        void
    ) = 0;

    virtual
    int
    _onReceive (
        wire_event_t handler_,
        void * context_
    ) = 0;

    virtual
    int
    _onRequest (
        signal_t handler_,
        void * context_
    ) = 0;

    virtual
    int
    _read (
        void
    ) = 0;

    virtual
    size_t
    _requestFrom (
        uint8_t address_,
        size_t quantity_,
        bool stop_
    ) = 0;

    virtual
    int
    _setClock (
        size_t freq_Hz_
    ) = 0;

    virtual
    size_t
    _write (
        uint8_t byte_
    ) = 0;
};

} // namespace remote_wiring

#endif // WIRE_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
