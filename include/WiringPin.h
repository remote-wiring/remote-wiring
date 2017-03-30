/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef WIRING_PIN_H
#define WIRING_PIN_H

#include <cstddef>
#include <cstdint>

#include "RwConstants.h"
#include "RwTypes.h"

namespace remote_wiring {

/*!
 * \brief Details the hardware configuration of a pin
 *
 * PinInfo is immutable data describing the capabilities of each pin.
 * This information describes the modes of operation supported on each
 * pin, as well as the read and write resolution of analog operations
 * for that pin - if they exist. PinInfo also provides a generic way to
 * test the capabilities of a pin, regardless of the underlying protocol.
 */
class WiringPinInfo {
  public:
    /*!
     * \brief Describes whether a pin has analog read capabilities
     *
     * \return A `bool` signaling `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    inline
    bool
    analogReadAvailable (
        void
    ) const {
        return capabilityPresentInPinInfo(ANALOG_READ);
    }

    /*!
     * \brief Describes the bits of resolution associated with analog read
     *
     * \return An integer indicating the number of bits used to
     *         describe the precision of the analog read value.
     *
     * \note The Arduino Uno has a 10-bit analog to digital converter (ADC),
     *       which allows for a reading range of [0, 1023].
     */
    inline
    size_t
    analogReadBitsOfResolution (
        void
    ) const {
        return analog_read_resolution_bits;
    }

    /*!
     * \brief Describes whether a pin has analog write capabilities
     *
     * \return A `bool` signaling `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    inline
    bool
    analogWriteAvailable (
        void
    ) const {
        return capabilityPresentInPinInfo(ANALOG_WRITE);
    }

    /*!
     * \brief Describes the bits of resolution associated with analog write
     *
     * \return An integer indicating the number of bits used to
     *         describe the precision of the analog write value.
     *
     * \note The Arduino Uno uses 8-bit resolution (default) when invoking
     *       pins with the pulse-width modulation (PWM) capability. 8 bits
     *       allows for a writing range of [0, 255].
     */
    inline
    size_t
    analogWriteBitsOfResolution (
        void
    ) const {
        return analog_write_resolution_bits;
    }

    /*!
     * \brief Describes whether a pin has digital read capabilities
     *
     * \return A `bool` signaling `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    inline
    bool
    digitalReadAvailable (
        void
    ) const {
        return capabilityPresentInPinInfo(DIGITAL_READ);
    }

    /*!
     * \brief Describes whether a pin has a built-in pullup resistor
     *
     * \return A `bool` signaling `true` when a pullup resistor is
     *         available and `false` when a pullup resistor is unavailable
     *
     * \note A pullup resistor is used to insulate digital reads from
     *       environmental noise. When a pullup resistor is employed
     *       the digital pin will be tested with for LOW.
     */
    inline
    bool
    digitalReadPullupAvailable (
        void
    ) const {
        return capabilityPresentInPinInfo(DIGITAL_READ_WITH_PULLUP);
    }

    /*!
     * \brief Describes whether a pin has digital write capabilities
     *
     * \return A `bool` signaling `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    inline
    bool
    digitalWriteAvailable (
        void
    ) const {
        return capabilityPresentInPinInfo(DIGITAL_WRITE);
    }

    pin_data_t supported_modes : 8;
    pin_data_t analog_read_resolution_bits : 8;
    pin_data_t analog_write_resolution_bits : 8;
    pin_data_t reserved : 8;

  private:
    inline
    bool
    capabilityPresentInPinInfo (
        pin_data_t capability_
    ) const {
        return (supported_modes & capability_);
    }
};

/*!
 * \brief Interrupt state
 */
struct WiringPinInterrupt {
    void * context;
    signal_t isr;
    size_t mode;
};

/*!
 * \brief Details the active state of a pin
 *
 * PinState is volatile data describing the active state of each pin.
 * This information describes the current mode of operation on each
 * pin, the current resolution of analog operations on the pin - if
 * applicable - and the value associated with the pin.
 */
class WiringPinState {
  public:
    pin_data_t mode : 8;
    pin_data_t resolution_bits : 8;
    pin_data_t value : 16;
};

} // remote_wiring

#endif  // WIRING_PIN_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
