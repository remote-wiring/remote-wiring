#ifndef RW_BOARDS_H
#define RW_BOARDS_H

#include <cstddef>

namespace remote_wiring {
namespace boards {
namespace arduino {
namespace adafruit {
namespace circuit_playground {

static const size_t A0 = 18;
static const size_t A1 = 19;
static const size_t A2 = 20;
static const size_t A3 = 21;
static const size_t A4 = 22;
static const size_t A5 = 23;
static const size_t A6 = 24;
static const size_t A7 = 25;
static const size_t A8 = 26;
static const size_t A9 = 27;
static const size_t A10 = 28;
static const size_t A11 = 29;

static const size_t SS = 17;
static const size_t MOSI = 16;
static const size_t MISO = 14;
static const size_t SCK = 15;

static const size_t SDA = 2;
static const size_t SCL = 3;

static const size_t LED_BUILTIN = 13;

}  // namespace circuit_playground

namespace gemma {

static const size_t A0 = 6;
static const size_t A1 = 7;
static const size_t A2 = 8;
static const size_t A3 = 9;

static const size_t LED_BUILTIN = 1;

}  // namespace gemma
}  // namespace adafruit

namespace arduino101 {

static const size_t A0 = 14;
static const size_t A1 = 15;
static const size_t A2 = 16;
static const size_t A3 = 17;
static const size_t A4 = 18;
static const size_t A5 = 19;

static const size_t SS = 10;
static const size_t MOSI = 11;
static const size_t MISO = 12;
static const size_t SCK = 13;

static const size_t SDA = 18;
static const size_t SCL = 19;

static const size_t LED_BUILTIN = 13;

}

namespace leonardo {

static const size_t A0 = 18;
static const size_t A1 = 19;
static const size_t A2 = 20;
static const size_t A3 = 21;
static const size_t A4 = 22;
static const size_t A5 = 23;
static const size_t A6 = 24;
static const size_t A7 = 25;
static const size_t A8 = 26;
static const size_t A9 = 27;
static const size_t A10 = 28;
static const size_t A11 = 29;

static const size_t SS = 17;
static const size_t MOSI = 16;
static const size_t MISO = 14;
static const size_t SCK = 15;

static const size_t SDA = 2;
static const size_t SCL = 3;

static const size_t LED_BUILTIN = 13;

}  // namespace leonardo

namespace mega {

static const size_t A0 = 54;
static const size_t A1 = 55;
static const size_t A2 = 56;
static const size_t A3 = 57;
static const size_t A4 = 58;
static const size_t A5 = 59;
static const size_t A6 = 60;
static const size_t A7 = 61;
static const size_t A8 = 62;
static const size_t A9 = 63;
static const size_t A10 = 64;
static const size_t A11 = 65;
static const size_t A12 = 66;
static const size_t A13 = 67;
static const size_t A14 = 68;
static const size_t A15 = 69;

static const size_t SS = 53;
static const size_t MOSI = 51;
static const size_t MISO = 50;
static const size_t SCK = 52;

static const size_t SDA = 20;
static const size_t SCL = 21;

static const size_t LED_BUILTIN = 13;

}  // mega

namespace mini {

static const size_t A0 = 18;
static const size_t A1 = 19;
static const size_t A2 = 20;
static const size_t A3 = 21;
static const size_t A4 = 22;
static const size_t A5 = 23;
static const size_t A6 = 24;
static const size_t A7 = 25;
static const size_t A8 = 26;
static const size_t A9 = 27;
static const size_t A10 = 28;
static const size_t A11 = 29;

static const size_t SS = 17;
static const size_t MOSI = 16;
static const size_t MISO = 14;
static const size_t SCK = 15;

static const size_t SDA = 2;
static const size_t SCL = 3;

static const size_t LED_BUILTIN = 13;

}  // namespace mini

namespace mkrzero {

static const size_t A0 = 15;
static const size_t A1 = 16;
static const size_t A2 = 17;
static const size_t A3 = 18;
static const size_t A4 = 19;
static const size_t A5 = 20;
static const size_t A6 = 21;

static const size_t SS = 4;
static const size_t MOSI = 8;
static const size_t MISO = 10;
static const size_t SCK = 9;

static const size_t SDA = 11;
static const size_t SCL = 12;

static const size_t LED_BUILTIN = 32;

}  // namespace mkrzero

namespace mkr1000 {

static const size_t A0 = 15;
static const size_t A1 = 16;
static const size_t A2 = 17;
static const size_t A3 = 18;
static const size_t A4 = 19;
static const size_t A5 = 20;
static const size_t A6 = 21;

static const size_t SS = 24;
static const size_t MOSI = 8;
static const size_t MISO = 10;
static const size_t SCK = 9;

static const size_t SDA = 11;
static const size_t SCL = 12;

static const size_t LED_BUILTIN = 6;

}  // namespace mkr1000

namespace m0 {

static const size_t A0 = 24;
static const size_t A1 = 25;
static const size_t A2 = 26;
static const size_t A3 = 27;
static const size_t A4 = 28;
static const size_t A5 = 29;

static const size_t SS = 14;
static const size_t MOSI = 21;
static const size_t MISO = 18;
static const size_t SCK = 20;

static const size_t SDA = 16;
static const size_t SCL = 17;

static const size_t LED_BUILTIN = 13;

}  // namespace m0

namespace standard {

static const size_t A0 = 14;
static const size_t A1 = 15;
static const size_t A2 = 16;
static const size_t A3 = 17;
static const size_t A4 = 18;
static const size_t A5 = 19;
static const size_t A6 = 20;
static const size_t A7 = 21;

static const size_t SS = 10;
static const size_t MOSI = 11;
static const size_t MISO = 12;
static const size_t SCK = 13;

static const size_t SDA = 18;
static const size_t SCL = 19;

static const size_t LED_BUILTIN = 13;

}  // namespace standard

namespace uno {

// ATMEL ATMEGA8 & 168 / ARDUINO
//
//                  +-\/-+
//            PC6  1|    |28  PC5 (AI 5)
//      (D 0) PD0  2|    |27  PC4 (AI 4)
//      (D 1) PD1  3|    |26  PC3 (AI 3)
//      (D 2) PD2  4|    |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|    |24  PC1 (AI 1)
//      (D 4) PD4  6|    |23  PC0 (AI 0)
//            VCC  7|    |22  GND
//            GND  8|    |21  AREF
//            PB6  9|    |20  AVCC
//            PB7 10|    |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|    |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|    |17  PB3 (D 11) PWM
//      (D 7) PD7 13|    |16  PB2 (D 10) PWM
//      (D 8) PB0 14|    |15  PB1 (D 9) PWM
//                  +----+
// (PWM+ indicates the additional PWM pins on the ATmega168.)

static const size_t A0 = 14;
static const size_t A1 = 15;
static const size_t A2 = 16;
static const size_t A3 = 17;
static const size_t A4 = 18;
static const size_t A5 = 19;
static const size_t A6 = 20;
static const size_t A7 = 21;

static const size_t SS = 10;
static const size_t MOSI = 11;
static const size_t MISO = 12;
static const size_t SCK = 13;

static const size_t SDA = 18;
static const size_t SCL = 19;

static const size_t LED_BUILTIN = 13;

}  // namespace uno

namespace yun {

static const size_t A0 = 18;
static const size_t A1 = 19;
static const size_t A2 = 20;
static const size_t A3 = 21;
static const size_t A4 = 22;
static const size_t A5 = 23;
static const size_t A6 = 24;
static const size_t A7 = 25;
static const size_t A8 = 26;
static const size_t A9 = 27;
static const size_t A10 = 28;
static const size_t A11 = 29;

static const size_t SS = 17;
static const size_t MOSI = 16;
static const size_t MISO = 14;
static const size_t SCK = 15;

static const size_t SDA = 2;
static const size_t SCL = 3;

static const size_t LED_BUILTIN = 13;

}  // namespace yun

namespace zero {

static const size_t A0 = 14;
static const size_t A1 = 15;
static const size_t A2 = 16;
static const size_t A3 = 17;
static const size_t A4 = 18;
static const size_t A5 = 19;

static const size_t SS = 16;
static const size_t MOSI = 23;
static const size_t MISO = 22;
static const size_t SCK = 24;

static const size_t SDA = 20;
static const size_t SCL = 21;

static const size_t LED_BUILTIN = 13;

}  // namespace mzero
}  // namespace arduino

namespace esp8266 {
namespace adafruit {

// ESP8266 / ADAFRUIT HUZZAH
//                       _____
//                  o----|___|----o
//            RST  1|         ____|
//            3V3  2|        |    |
//             NC  3|  O     |    |
//            GND  4|        |____|
//     (AI 0) ADC  5|   ___       |28  BAT
//             NC  6|  |   |      |27  REG/EN
//             NC  7|  |___|      |26  USB
//             NC  8|             |25  (D 14)
//             NC  9|             |24  (D 12)
//             NC 10|   ________  |23  (D 13)
//    (D 14)  SCK 11|  |       |  |22  SS (D 15)
//    (D 13) MOSI 12|  |       |  |21  (D 0)
//    (D 12) MISO 13|  |       |  |20  (D 16)
//             RX 14|  |       |  |19  (D 2)
//             TX 15|  |_______|  |18  SCL (D 5)
//             EN 16| **  -----|  |17  SDA (D 4)
//                  o--___||||||--o
// (PWM+ indicates the additional PWM pins on the ATmega168.)

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 0;

}  // namespace adafruit

namespace d1 {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 2;

}  // namespace d1

namespace d1_mini {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 2;

}  // namespace d1_mini

namespace espduino {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 16;

}  // namespace espduino

namespace espino {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 2;

}  // namespace espino

namespace espinotee {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 16;

}  // namespace espinotee

namespace espresso_lite_v1 {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 16;

}  // namespace espresso_lite_v1

namespace espresso_lite_v2 {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 2;

}  // namespace espresso_lite_v2

namespace generic {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 1;

}  // namespace generic

namespace nodemcu {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 16;

}  // namespace nodemcu

namespace phoenix_v1 {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 16;

}  // namespace phoenix_v1

namespace phoenix_v2 {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 2;

}  // namespace phoenix_v2

namespace thing {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 2;
static const size_t SCL = 14;

static const size_t LED_BUILTIN = 5;

}  // namespace thing

namespace wifinfo {

static const size_t A0 = 17;

static const size_t SS    = 15;
static const size_t MOSI  = 13;
static const size_t MISO  = 12;
static const size_t SCK   = 14;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 12;

}  // namespace wifinfo

namespace wifio {

static const size_t A0 = 14;
static const size_t A1 = 15;
static const size_t A2 = 16;
static const size_t A3 = 17;
static const size_t A4 = 18;
static const size_t A5 = 19;
static const size_t A6 = 20;
static const size_t A7 = 21;

static const size_t SS   = 12;
static const size_t MOSI = 13;
static const size_t MISO = 14;
static const size_t SCK  = 15;

static const size_t SDA = 4;
static const size_t SCL = 5;

static const size_t LED_BUILTIN = 2;

}  // namespace wifio
}  // namespace esp8266

namespace particle {
namespace bluz {

static const size_t A0 = 10;
static const size_t A1 = 11;
static const size_t A2 = 12;
static const size_t A3 = 13;
static const size_t A4 = 14;
static const size_t A5 = 15;
static const size_t A6 = 16;
static const size_t A7 = 17;

static const size_t SS = 12;
static const size_t MOSI = 15;
static const size_t MISO = 14;
static const size_t SCK = 13;

static const size_t SDA = 0;
static const size_t SCL = 1;

static const size_t LED_BUILTIN = 7;

}  // namespace bluz

namespace core {

static const size_t A0 = 10;
static const size_t A1 = 11;
static const size_t A2 = 12;
static const size_t A3 = 13;
static const size_t A4 = 14;
static const size_t A5 = 15;
static const size_t A6 = 16;
static const size_t A7 = 17;

static const size_t SS = 12;
static const size_t MOSI = 15;
static const size_t MISO = 14;
static const size_t SCK = 13;

static const size_t SDA = 0;
static const size_t SCL = 1;

static const size_t LED_BUILTIN = 7;

}  // namespace core

namespace electron {

static const size_t A0 = 10;
static const size_t A1 = 11;
static const size_t A2 = 12;
static const size_t A3 = 13;
static const size_t A4 = 14;
static const size_t A5 = 15;
static const size_t A6 = 16;
static const size_t A7 = 17;

static const size_t SS = 12;
static const size_t MOSI = 15;
static const size_t MISO = 14;
static const size_t SCK = 13;

static const size_t SDA = 0;
static const size_t SCL = 1;

static const size_t LED_BUILTIN = 7;

}  // namespace electron

namespace photon {

static const size_t A0 = 10;
static const size_t A1 = 11;
static const size_t A2 = 12;
static const size_t A3 = 13;
static const size_t A4 = 14;
static const size_t A5 = 15;
static const size_t A6 = 16;
static const size_t A7 = 17;

static const size_t SS = 12;
static const size_t MOSI = 15;
static const size_t MISO = 14;
static const size_t SCK = 13;

static const size_t SDA = 0;
static const size_t SCL = 1;

static const size_t LED_BUILTIN = 7;

}  // namespace photon

namespace redbear {
namespace duo {

static const size_t A0 = 10;
static const size_t A1 = 11;
static const size_t A2 = 12;
static const size_t A3 = 13;
static const size_t A4 = 14;
static const size_t A5 = 15;
static const size_t A6 = 16;
static const size_t A7 = 17;

static const size_t SS = 12;
static const size_t MOSI = 15;
static const size_t MISO = 14;
static const size_t SCK = 13;

static const size_t SDA = 0;
static const size_t SCL = 1;

static const size_t LED_BUILTIN = 7;

}  // namespace duo
}  // namespace redbear
}  // namespace particle
}  // namespace boards
}  // namespace remote_wiring

#endif // RW_BOARDS_H
