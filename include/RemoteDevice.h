/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef REMOTE_DEVICE_H
#define REMOTE_DEVICE_H

#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include "DeviceContract.h"

namespace remote_wiring {

const bool LOW = false;
const bool HIGH = true;

const size_t OUTPUT = 0;
const size_t INPUT = 1;
const size_t INPUT_PULLUP = 2;

namespace hardware {

typedef void (*signal_t)(void);

/*!
 * \brief A class representing a remote device capable of supporting the Wiring API
 *
 * \sa <a href="https://www.wiring.org.co/reference/">Wiring API</a>
 */
class RemoteDevice {
  public:
    /*!
     * \brief Create an unbounded device
     *
     * The default constructor creates an unbounded representation of a remote device.
     * A best effort attempt to deliver messages will be made, with no error checking.
     *
     * \note This mode does not require any inbound messages, which can help debug
     *       a poor connection or malconfigured remote device.
     */
    RemoteDevice (
        void
    );

    /*!
     * \brief Create a device bound by contract
     *
     * The contract constructor creates a bounded representation of a remote device.
     * The local machine will not attempt to send messages that will break the device
     * contract. For example, if the local machine were to attempt an analog write on
     * pin 6 of an Arduino Uno, the call would fail without sending malformed bytes
     * over the serial connection.
     *
     * \param [in] device_contract_ The contract detailing the limitations of the device
     *
     * \sa remote_wiring::protocol::DeviceContract
     * \sa remote_wiring::protocol::DeviceQuery
     */
    RemoteDevice (
        const protocol::DeviceContract * const device_contract_
    );

    /*!
     * \brief Reads the value from the specified analog pin
     *
     * The Arduino board contains a 6 channel (8 channels on the Mini
     * and Nano, 16 on the Mega), 10-bit analog to digital converter.
     * This yields a resolution between readings of: 5 volts / 1024
     * units or, .0049 volts (4.9 mV) per unit.
     *
     * \param pin_ The number of the analog input pin to read from
     *             (0 to 5 on most boards, 0 to 7 on the Mini and
     *             Nano, 0 to 15 on the Mega)
     *
     * \return Input voltages between 0 and 5 volts, mapped to integer
     *         values between 0 and 1023.
     *
     * \note Resolution between readings of: 5 volts / 1024 units,
     *       approximately .0049 volts (4.9 mV) per unit.
     * \note If the analog input pin is not connected to anything, the
     *       value returned by analogRead() will fluctuate based on a
     *       number of factors (e.g. the values of the other analog
     *       inputs, how close your hand is to the board, etc.).
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/AnalogRead">::analogRead (Arduino.cc)</a>
     */
    size_t
    analogRead (
        size_t pin_
    );

    /*!
     * \brief Writes an analog value (PWM wave) to a pin
     *
     * Generates a steady square wave of the specified duty cycle until
     * the next call to analogWrite() (or a call to digitalRead() or
     * digitalWrite() on the same pin). The frequency of the PWM signal
     * on most pins is approximately 490 Hz (see exceptions below).
     * This functionality can be used to light a LED at varying
     * brightnesses or drive a motor at various speeds.
     *
     * \param pin_ The number of the pin to write to
     * \param value_ The duty cycle: between 0 (always off) and 255 (always on)
     *
     * \note The analogWrite function has nothing to do with the analog pins
     *       or the `::analogRead` function.
     * \note On most Arduino boards (those with the ATmega168 or ATmega328),
     *       this function works on pins 3, 5, 6, 9, 10, and 11. On the
     *       Arduino Mega, it works on pins 2 - 13 and 44 - 46. Older
     *       Arduino boards with an ATmega8 only support analogWrite() on
     *       pins 9, 10, and 11.
     * \note The Arduino Due supports analogWrite() on pins 2 through 13,
     *       plus pins DAC0 and DAC1. Unlike the PWM pins, DAC0 and DAC1
     *       are Digital to Analog converters and act as true analog outputs.
     *
     * \warning On the Uno and similar boards, the PWM outputs generated
     *          on pins 5 and 6 (additionally pins 3 and 11 on the Leonardo)
     *          will have higher-than-expected duty cycles with a frequency
     *          of approximately 980 Hz. This is because of interactions
     *          with the millis() and delay() functions, which share the
     *          same internal timer used to generate those PWM outputs. This
     *          will be noticed mostly on low duty-cycle settings (e.g 0 -
     *          10) and may result in a value of 0 not fully turning off the
     *          output on pins 5 and 6.
     * \sa <a href="https://www.arduino.cc/en/Reference/AnalogWrite">::analogWrite (Arduino.cc)</a>
     */
    void
    analogWrite (
        size_t pin_,
        uint8_t value_
    );

    /*!
     * \brief Reads the value from a specified digital pin
     *
     * \param pin_ The number of the digital pin you want to read
     *
     * \return `HIGH` (`true`) or `LOW` (`false`)
     *
     * \note If the pin isn't connected to anything, `::digitalRead` can
     *       return either `HIGH` or `LOW` (and this can change randomly)
     * \note The analog input pins can be used as digital pins, referred
     *       to as A0, A1, etc.
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/DigitalRead">::digitalRead (Arduino.cc)</a>
     */
    bool
    digitalRead (
        size_t pin_
    );

    /*!
     * \brief Write a HIGH or a LOW value to a digital pin
     *
     * If the pin has been configured as an OUTPUT with pinMode(), its
     * voltage will be set to the corresponding value: 5V (or 3.3V on
     * 3.3V boards) for HIGH, 0V (ground) for LOW.
     *
     * \param pin_ The number of the digital pin you want to write to
     * \param value_ `HIGH` (`true`) or `LOW` (`false`)
     *
     * \note If the pin is configured as an `INPUT`, `digitalWrite()` will
     *       enable (`HIGH`) or disable (`LOW`) the internal pullup on the
     *       input pin. Instead, it is recommended to set the `pinMode()`
     *       to `INPUT_PULLUP` to enable the internal pull-up resistor.
     * \note The analog input pins can be used as digital pins, referred
     *       to as A0, A1, etc.
     *
     * \sa <a href="https://www.arduino.cc/en/Reference/DigitalWrite">::digitalWrite (Arduino.cc)</a>
     */
    void
    digitalWrite (
        size_t pin_,
        bool value_
    );

    /*!
     * \brief Configures the specified pin to behave either as
     *        an input or an output
     *
     * \param pin_ The number of the pin whose mode you wish to set
     * \param mode_ The mode of operation (valid values: INPUT,
     *              OUTPUT, or INPUT_PULLUP)
     *
     * \note The analog input pins can be used as digital pins,
     *       referred to as A0, A1, etc.
     *
     * \sa RemoteDevice::analogWrite
     * \sa RemoteDevice::digitalRead
     * \sa RemoteDevice::digitalWrite
     * \sa <a href="https://www.arduino.cc/en/Reference/PinMode">::pinMode (Arduino.cc)</a>
     */
    void
    pinMode (
        size_t pin_,
        size_t mode_
    );

    /*!
     * \brief Updates local cache based on the current state of the remote device
     *
     * \param [in] A function pointer to a callback to be fired once the
     *             refresh operation has completed.
     * \param [in] A context to be passed to the callback
     *
     * \note The signature of the signalling callback is `void(*)(void)`
     */
    int
    refresh (
        signal_t refreshComplete_,
        void * context_
    );

  private:
    const protocol::DeviceContract * const _device_contract;
    size_t _pin_count;
    protocol::pin_config_t * _pin_mode;
    unsigned int * _pin_value;
    signal_t _refreshComplete;
    void * _refresh_context;

    inline
    int
    udpateStorageForPin (
        const size_t pin_
    ) {
        int error;
        if (pin_ <= _pin_count) {
            error = 0;
        } else if ( NULL == (_pin_mode = (protocol::pin_config_t *)realloc(_pin_mode, (pin_ * sizeof(protocol::pin_config_t)))) ) {
            error = __LINE__;
        } else if ( NULL == (_pin_value = (unsigned int *)realloc(_pin_value, (pin_ * sizeof(unsigned int)))) ) {
            free(_pin_mode);
            error = __LINE__;
        } else {
            error = 0;
        }
        return error;
    }
};

} // namespace hardware
} // namespace remote_wiring

#endif // REMOTE_DEVICE_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

