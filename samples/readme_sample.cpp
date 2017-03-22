/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include <chrono>
#include <iostream>
#include <thread>

#include <remote_wiring>
#include <serial_wiring>

using namespace remote_wiring::boards::arduino::uno;  // change to your board
using namespace remote_wiring::wiring;

int main (int argc, char * argv []) {
    std::cout << "************************************************" << std::endl;
    std::cout << "** The \"Examples > Firmata > StandardFirmata\" **" << std::endl;
    std::cout << "** sketch must be deployed to the Arduino in  **" << std::endl;
    std::cout << "** order for the sample to work correctly.    **" << std::endl;
    std::cout << "************************************************" << std::endl;

    if ( argc < 2 ) { std::cout << "Usage: " << argv[0] << " <serial device descriptor>" << std::endl; return -1; }

    serial_wiring::UartSerial usb(argv[1]);
    remote_wiring::FirmataDevice board(usb);

    // Establish a communication channel
    usb.begin(57600);

    // Attach to the remote device
    board.attach();

    // Survey the board's capabilities
    // (not necessary but allows for error checking)
    board.survey();

    // Initialize digital pin LED_BUILTIN as an output
    board.pinMode(LED_BUILTIN, OUTPUT);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    // Issue commands to the remote device via the Wiring API
    board.digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    board.digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // allow time for the serial to send

    // Clean-up and exit
    board.detach();
    usb.end();
    return 0;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
