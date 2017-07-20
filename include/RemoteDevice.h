/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef REMOTE_DEVICE_H
#define REMOTE_DEVICE_H

#include "Telemorph.h"
#include "Wiring.h"

namespace remote_wiring {

/*!
 * \brief A telemorph conforming to the Wiring API
 */
struct RemoteDevice : public Telemorph, public Wiring {
    RemoteDevice (
        TwoWire & Wire_
    ) :
        Wiring(Wire_)
    { }
};

} // remote_wiring

#endif  // REMOTE_DEVICE_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
