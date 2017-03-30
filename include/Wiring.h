/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef WIRING_H
#define WIRING_H

#include <cstddef>
#include <cstdint>

namespace remote_wiring {

/*!
 * \brief An interface to ensure the availability of the Wiring API
 *
 * \sa <a href="https://www.wiring.org.co/reference/">Wiring API</a>
 */
class Wiring {
  public:
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
    inline
    size_t
    analogRead (
        size_t pin_
    ) {
        return _analogRead(pin_);
    }

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
    inline
    void
    analogWrite (
        size_t pin_,
        uint8_t value_
    ) {
        _analogWrite(pin_, value_);
    }

    /*!
     * \brief Attach an ISR to a psuedo-interrupt for a given pin
     *
     * Generally, an ISR (interrupt service routine) should be as short
     * and fast as possible. If your sketch uses multiple ISRs, only one
     * can run at a time, other interrupts will be executed after the
     * current one finishes in an order determined by the system.
     *
     * \param pin_ The pin number
     * \param isr_ The interrupt service routine to call when the
     *             interrupt condition occurs
     * \param mode_ Defines when the interrupt should be triggered
     *              (valid values: LOW, HIGH, CHANGE, RISING, FALLING)
     *
     * \note The user provided ISR will be invoked as a result of a
     *       polling interval on the remote device.
     *
     * \warning This is considered a psuedo-interrupt and cannot be
     *          substitued for true hardware interrupts. As such, precise
     *          timing operations cannot be preformed. This limitation
     *          manifest due to the latency involved in the remote aspect
     *          of the device.
     */
    inline
    void
    attachInterrupt (
        size_t pin_,
        signal_t isr_,
        size_t mode_,
        void * context_ = nullptr
    ) {
        _attachInterrupt(pin_, isr_, mode_, context_);
    }

    /*!
     * \brief  Detach the ISR for a given pin
     */
    inline
    void
    detachInterrupt (
        size_t pin_
    ) {
        _detachInterrupt(pin_);
    }

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
    inline
    bool
    digitalRead (
        size_t pin_
    ) {
        return _digitalRead(pin_);
    }

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
    inline
    void
    digitalWrite (
        size_t pin_,
        bool value_
    ) {
        _digitalWrite(pin_, value_);
    }

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
    inline
    void
    pinMode (
        size_t pin_,
        size_t mode_
    ) {
        _pinMode(pin_, mode_);
    }

  protected:
    ~Wiring (void) {}

    virtual
    size_t
    _analogRead (
        size_t pin_
    ) = 0;

    virtual
    void
    _analogWrite (
        size_t pin_,
        uint8_t value_
    ) = 0;

    virtual
    void
    _attachInterrupt (
        size_t pin_,
        signal_t isr_,
        size_t mode_,
        void * context_
    ) = 0;

    virtual
    void
    _detachInterrupt (
        size_t pin_
    ) = 0;

    virtual
    bool
    _digitalRead (
        size_t pin_
    ) = 0;

    virtual
    void
    _digitalWrite (
        size_t pin_,
        bool value_
    ) = 0;

    virtual
    void
    _pinMode (
        size_t pin_,
        size_t mode_
    ) = 0;
};

} // namespace remote_wiring

#endif // WIRING_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

