/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include <remote_wiring>
#include <serial_wiring>

#include "Wire.h"

struct WiiNunchuckStatePackedByte {
    const uint8_t button_z:1;
    const uint8_t button_c:1;
    const uint8_t accelerometer_x_lsb:2;
    const uint8_t accelerometer_y_lsb:2;
    const uint8_t accelerometer_z_lsb:2;
};

union WiiNunchuckStateExt {
    const uint8_t data;
    WiiNunchuckStatePackedByte packed_byte;
};

struct WiiNunchuckState {
    WiiNunchuckState (
        void
    ) :
        accelerometer_x(0x0000),
        accelerometer_y(0x0000),
        accelerometer_z(0x0000),
        button_c(false),
        button_z(false),
        joystick_x(0x00),
        joystick_y(0x00)
    { };

    WiiNunchuckState (
        const uint8_t joystick_x_,
        const uint8_t joystick_y_,
        const uint8_t accelerometer_x_,
        const uint8_t accelerometer_y_,
        const uint8_t accelerometer_z_,
        const uint8_t ext_
    ) :
        joystick_x((joystick_x_ ^ 0x17) + 0x17),
        joystick_y((joystick_y_ ^ 0x17) + 0x17)
    {
        // Decrypt packed byte
        const WiiNunchuckStateExt ext{static_cast<uint8_t>((ext_ ^ 0x17) + 0x17)};

        accelerometer_x = ((accelerometer_x_ ^ 0x17) + 0x17) << 2;
        accelerometer_x |= ext.packed_byte.accelerometer_x_lsb;
        accelerometer_y = ((accelerometer_y_ ^ 0x17) + 0x17) << 2;
        accelerometer_y |= ext.packed_byte.accelerometer_y_lsb;
        accelerometer_z = ((accelerometer_z_ ^ 0x17) + 0x17) << 2;
        accelerometer_z |= ext.packed_byte.accelerometer_z_lsb;
        button_c = !ext.packed_byte.button_c;
        button_z = !ext.packed_byte.button_z;
    }

    WiiNunchuckState (
      const uint8_t raw_data_[6]
    ) :
        joystick_x((raw_data_[0] ^ 0x17) + 0x17),
        joystick_y((raw_data_[1] ^ 0x17) + 0x17)
    {
        // Decrypt packed byte
        const WiiNunchuckStateExt ext{static_cast<uint8_t>((raw_data_[5] ^ 0x17) + 0x17)};

        accelerometer_x = ((raw_data_[2] ^ 0x17) + 0x17) << 2;
        accelerometer_x |= ext.packed_byte.accelerometer_x_lsb;
        accelerometer_y = ((raw_data_[3] ^ 0x17) + 0x17) << 2;
        accelerometer_y |= ext.packed_byte.accelerometer_y_lsb;
        accelerometer_z = ((raw_data_[4] ^ 0x17) + 0x17) << 2;
        accelerometer_z |= ext.packed_byte.accelerometer_z_lsb;
        button_c = !ext.packed_byte.button_c;
        button_z = !ext.packed_byte.button_z;
    }

    void
    print(
        void
    ) {
        std::cout << "Joystick: " << std::dec << std::setw(3) << std::setfill(' ') << static_cast<size_t>(joystick_x) << "," << std::dec << std::setw(3) << std::setfill(' ') << static_cast<size_t>(joystick_y)
          << "\t" << "Accelerometer: " << std::dec << std::setw(4) << std::setfill(' ') << accelerometer_x << "," << std::dec << std::setw(4) << std::setfill(' ') << accelerometer_y << "," << std::dec << std::setw(4) << std::setfill(' ') << accelerometer_z
          << "\t" << "Button: " << button_c << "," << button_z << std::flush << std::endl;
    }

    void
    update (
        const uint8_t joystick_x_,
        const uint8_t joystick_y_,
        const uint8_t accelerometer_x_,
        const uint8_t accelerometer_y_,
        const uint8_t accelerometer_z_,
        const uint8_t ext_
    ) {
        // Decrypt packed byte
        const WiiNunchuckStateExt ext{static_cast<uint8_t>((ext_ ^ 0x17) + 0x17)};

        accelerometer_x = ((accelerometer_x_ ^ 0x17) + 0x17) << 2;
        accelerometer_x |= ext.packed_byte.accelerometer_x_lsb;
        accelerometer_y = ((accelerometer_y_ ^ 0x17) + 0x17) << 2;
        accelerometer_y |= ext.packed_byte.accelerometer_y_lsb;
        accelerometer_z = ((accelerometer_z_ ^ 0x17) + 0x17) << 2;
        accelerometer_z |= ext.packed_byte.accelerometer_z_lsb;
        button_c = !ext.packed_byte.button_c;
        button_z = !ext.packed_byte.button_z;
        joystick_x = ((joystick_x_ ^ 0x17) + 0x17);
        joystick_y = ((joystick_y_ ^ 0x17) + 0x17);
    }

    void
    update (
        const uint8_t raw_data_[6]
    ) {
        // Decrypt packed byte
        const WiiNunchuckStateExt ext{static_cast<uint8_t>((raw_data_[5] ^ 0x17) + 0x17)};

        accelerometer_x = ((raw_data_[2] ^ 0x17) + 0x17) << 2;
        accelerometer_x |= ext.packed_byte.accelerometer_x_lsb;
        accelerometer_y = ((raw_data_[3] ^ 0x17) + 0x17) << 2;
        accelerometer_y |= ext.packed_byte.accelerometer_y_lsb;
        accelerometer_z = ((raw_data_[4] ^ 0x17) + 0x17) << 2;
        accelerometer_z |= ext.packed_byte.accelerometer_z_lsb;
        button_c = !ext.packed_byte.button_c;
        button_z = !ext.packed_byte.button_z;
        joystick_x = ((raw_data_[0] ^ 0x17) + 0x17);
        joystick_y = ((raw_data_[1] ^ 0x17) + 0x17);
    }

    uint16_t accelerometer_x;
    uint16_t accelerometer_y;
    uint16_t accelerometer_z;
    bool button_c;
    bool button_z;
    uint8_t joystick_x;
    uint8_t joystick_y;
};

using namespace remote_wiring::boards::arduino::uno;  // change to your board
using namespace remote_wiring::wiring;

int main (void) {
    uint8_t raw_data_buffer[6] = {0};
    WiiNunchuckState nunchuck;

    serial_wiring::UartSerial usb("/dev/ttyACM0");
    remote_wiring::FirmataDevice board(usb);

    // Establish a communication channel
    usb.begin();

    // Attach to the remote device
    board.attach();

    // Survey the board's capabilities
    // (not necessary but allows for error checking)
    board.survey();

    // Set analog pins to OUTPUT
    board.pinMode(A2, OUTPUT);
    board.pinMode(A3, OUTPUT);

    // Emulate power and ground
    board.digitalWrite(A2, LOW);
    board.digitalWrite(A3, HIGH);

    // Allow the Wii Nunchuck MCU to wake
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Join I²C bus (as master)
    board.Wire.begin();

    // Handshake with the Wii Nunchuck
    board.Wire.beginTransmission(0x52); // transmit to device 0x52
    board.Wire.write(0x40);    // sends memory address
    board.Wire.write(0x00);    // sends register address
    board.Wire.endTransmission(); // stop transmitting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Instruct Wii Nunchuck to sample sensors
    board.Wire.beginTransmission(0x52); // transmit to device 0x52
    board.Wire.write(0x00);    // sends one byte
    board.Wire.endTransmission(); // stop transmitting
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Request sensor data from Wii Nunchuck
    board.Wire.requestFrom (0x52, 6); // request data from nunchuck
    for ( int i = 0 ; i < 6 && board.Wire.available() ; ++i ) {
      raw_data_buffer[i] = board.Wire.read();
    }

    nunchuck.update(raw_data_buffer);
    nunchuck.print();

    // Clean-up and exit
    board.Wire.end();
    board.detach();
    usb.end();
    return 0;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
