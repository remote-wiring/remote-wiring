Remote Wiring
==============

## Library details:

The `remote_wiring` library provides an object-oriented [Wiring](http://wiring.org.co/) interface to any* remote embedded device connected over a [supported serial connection](https://github.com/remote-wiring/serial-wiring).

Technical documentation, requirements and diagrams will be stored in the `docs/` folder. Sample programs are located in the `samples/` folder. Google Mock unit-tests are located in the `tests/` folder.

**The target device must have a C++ compiler and be running a firmata host program (i.e. [StandardFirmata.ino](https://github.com/firmata/arduino/tree/master/examples/StandardFirmata)).*

## Software License:
This project uses the ***MIT license***, please consult the license file for details.

## References:

[Wiring API - Wiring.org.co](http://wiring.org.co/reference/)
[Wiring API - Arduino.cc](https://www.arduino.cc/en/Reference/HomePage)
[Wiring API - Particle.io](https://docs.particle.io/reference/firmware/photon/#input-output)

## Install instructions:

```
git clone https://github.com/remote-wiring/remote-wiring.git --recursive
cd remote-wiring
mkdir build
cd build
cmake ..
make
```

## Example code:

#### Blink the onboard LED

```c++
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
```
