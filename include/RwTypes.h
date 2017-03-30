/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef RW_TYPES_H
#define RW_TYPES_H

#include <cstdint>

namespace remote_wiring {

// Forward declarations
class FirmataDevice;
struct RemoteDevice;
struct SemVer;
class Telemorph;
class Wiring;
class WiringPinInfo;
struct WiringPinInterrupt;
class WiringPinState;

// Definitions
/*!
 * \brief The base type for both immutable and volatile pin data
 */
typedef uint_fast32_t pin_data_t;

/*!
 * \brief A basic signal from the Remote Wiring library
 *
 * The signal callback is a simple callback implementation that
 * provides context via a parameter to a delegate, which in-turn
 * allows for a mechanism of stateful callbacks (psuedo non-static).
 */
typedef void (*signal_t)(void * context_);

}  // remote_wiring

#endif  // RW_TYPES_H
