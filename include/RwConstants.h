/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef RW_CONSTANTS_H
#define RW_CONSTANTS_H

#include <cstdint>

#include "RwTypes.h"

namespace remote_wiring {

// Definitions
static const pin_data_t ANALOG_READ = 0x01;
static const pin_data_t ANALOG_WRITE = 0x02;
static const pin_data_t DIGITAL_READ = 0x04;
static const pin_data_t DIGITAL_READ_WITH_PULLUP = 0x08;
static const pin_data_t DIGITAL_WRITE = 0x10;

#ifdef TEST
  static const size_t REMOTE_DEVICE_TIMEOUT_MS = 1;
#else
  static const size_t REMOTE_DEVICE_TIMEOUT_MS = 5000;
#endif

namespace wiring {

static const size_t LOW = 0;
static const size_t HIGH = 1;
static const size_t CHANGE = 2;
static const size_t RISING = 3;
static const size_t FALLING = 4;

static const size_t OUTPUT = 0;
static const size_t INPUT = 1;
static const size_t INPUT_PULLUP = 2;

}  // wiring
}  // remote_wiring

#endif // RW_CONSTANTS_H
