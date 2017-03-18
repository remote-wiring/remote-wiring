#ifndef RW_BOARDS_H
#define RW_BOARDS_H

#include <cstddef>

namespace remote_wiring {
namespace boards {
namespace arduino {
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
}  // namespace particle
}  // namespace boards
}  // namespace remote_wiring

#endif // RW_BOARDS_H
