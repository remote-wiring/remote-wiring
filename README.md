# Remote Wiring

Remote Wiring allows you to control an [Arduino](https://www.arduino.cc/) clone\* from a Linux or MacOS machine using the [Wiring interface](http://wiring.org.co/reference).

Imagine, an application developer now has the ability to interact with the real world from a program that was previously constrained to a computer. While the embedded developer can now develop on a platform with rich displays, toolsets and relatively unlimited power and resources. Both types of developer get to program against the same well-documented and well understood Wiring interface that has become the de facto standard due to the Arduino development community.

_\*The target device must be capable of running a firmata host program \(i.e. _[_StandardFirmata.ino_](https://github.com/firmata/arduino/tree/master/examples/StandardFirmata)_\) and reachable via a _[_supported serial connection_](https://github.com/remote-wiring/serial-wiring)_._

### Remote Device API \[v0.1.0\]:

###### Wiring interface

```cpp
      size_t RemoteDevice::analogRead (size_t pin);
        void RemoteDevice::analogWrite (size_t pin, uint8_t value);
        bool RemoteDevice::digitalRead (size_t pin);
        void RemoteDevice::digitalWrite (size_t pin, bool value);
        void RemoteDevice::pinMode (size_t pin, size_t mode);
```

###### Telemorph interface

```cpp
         int RemoteDevice::attach (signal_t uponAttach, void * context);
        void RemoteDevice::detach (void);
const char * RemoteDevice::firmware (void);
         int RemoteDevice::refresh (signal_t uponRefresh, void * context);
         int RemoteDevice::reset (signal_t uponReset, void * context);
         int RemoteDevice::survey (signal_t uponSurvey, void * context);
      SemVer RemoteDevice::version (void);
```

> See also: [Wiring interface](http://wiring.org.co/reference)

## Quick Start Guide

### Prerequisites:

* **CMake ** `v2.8.2` \(_minimum version_\)
* **Git ** `v1.9.1` \(_minimum version_\)

### Install Instructions:

```bash
$ git clone https://github.com/remote-wiring/remote-wiring.git --recursive
$ cd remote-wiring/
$ mkdir build
$ cd build/
$ cmake ..
$ make
```

### Run the sample:

1. Run `readme_example` with no parameters to print out usage instructions.

   ```bash
   $ ./samples/readme_example

   ************************************************
   ** The "Examples > Firmata > StandardFirmata" **
   ** sketch must be deployed to the Arduino in  **
   ** order for the sample to work correctly.    **
   ************************************************
   Usage: ./samples/readme_sample <serial device descriptor>
   ```

2. Install StandardFirmata.ino \(v2.5.6\) on an Arduino clone.

3. Take note of the _**Tools &gt; Port**_ setting, because this is the `<serial device descriptor>` required by `readme_example`.

4. Run `readme_example` \(_linux example shown below_\).
   ```bash
   ./samples/readme_sample /dev/ttyACM0/
   ```

###### Expected Behavior

1. StandardFirmata will blink its major and minor version using a series of quick blinks.
2. A single slow blink \(lasting one second\) will occur \(as shown in the example code below\).

### Example Code:

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

    // Issue commands to the remote device via the Wiring API
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

### Code Breakdown:

```c++
#include <chrono>
#include <iostream>
#include <thread>
```

These lines are from the standard template library. They give us the ability to pause the program, so our slow human reflexes can witness the programs execution on the hardware as well as through console logging.

```c++
#include <serial_wiring>
#include <remote_wiring>
```

These lines pull in the Remote Wiring libraries. Serial Wiring allows us to communicate with the remote device over a serial connection via an Arduino `Stream` compatible interface. Remote Wiring provides the `FirmataDevice`, which adheres to the `RemoteDevice` interface.

```c++
using namespace remote_wiring::boards::arduino::uno;  // change to your board
```

The `remote_wiring::boards` namespace defines several keywords with board specific values that are available in an Arduino sketch \(i.e. `LED_BUILTIN`, `A0`, `A1`, ..., `An`, etc...\).

```c++
using namespace remote_wiring::wiring;
```

The `remote_wiring::wiring` namespace defines several keywords with universal values that are available in an Arduino sketch \(i.e. `INPUT`, `OUTPUT`, `HIGH`, `LOW`, etc...\)

```c++
int main (int argc, char * argv []) {
    std::cout << "************************************************" << std::endl;
    std::cout << "** The \"Examples > Firmata > StandardFirmata\" **" << std::endl;
    std::cout << "** sketch must be deployed to the Arduino in  **" << std::endl;
    std::cout << "** order for the sample to work correctly.    **" << std::endl;
    std::cout << "************************************************" << std::endl;

    if ( argc < 2 ) { std::cout << "Usage: " << argv[0] << " <serial device descriptor>" << std::endl; return -1; }
```

`main()` is the default entrypoint for any C/C++ program. The parameters `argc` and `argv` describe an array of user specified command line parameters. The next lines generate a banner reminding the user to deploy `StandardFirmata.ino` on the target device, which is necessary to receive the instructions generated by the `RemoteDevice`.

```c++
    serial_wiring::UartSerial usb(argv[1]);
```

`UartSerial` is the physical communication channel with the remote device. It adheres to the `Stream` interface and provides a well-defined serial connection to the `RemoteDevice` implementation. Here `argv[1]` is passed as the construction parameter to the `UartSerial` object. It is expected the user provided the path to the serial device descriptor as a parameter on the command line \(i.e. `/dev/ttyACM0` on Linux\).

```c++
    remote_wiring::FirmataDevice board(usb);
```

The `FirmataDevice` object is a telemorphic representation of the target device. The constructor requires a `Stream` object, so it will be able to communicate with the remote device.

```c++
    // Establish a communication channel
    usb.begin(57600);
```

`Stream::begin` starts the UART connection at 57600 baud, which is the default baud rate for `StandardFirmata.ino`.

```c++
    // Attach to the remote device
    board.attach();
```

`RemoteDevice::attach` queries the firmware of the remote device to ensure the communication channel is properly configured and the protocol version is compatible.

```c++
    // Survey the board's capabilities
    // (not necessary but allows for error checking)
    board.survey();
```

A survey is not necessary, but is good practice. `RemoteDevice::survey` gets as much information from the board as possible, which in turn allows it to enforce error checking and generate better diagnostic messages.

```c++
    // Initialize digital pin LED_BUILTIN as an output
    board.pinMode(LED_BUILTIN, OUTPUT);
```

The call to `RemoteDevice::pinMode` tells the specfied pin whether it should be in input or output mode. Here it is set to `OUTPUT` in order to be able to drive power and ground to the pin.

```c++
    // Issue commands to the remote device via the Wiring API
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    board.digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    board.digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // allow time for the serial to send
```

The `RemoteDevice::digitalWrite` calls drive power \(`HIGH`\) or ground \(`LOW`\) to the GPIO \(general purpose input/output\) pins, which turns on or off the LED. The first sleep call allows the initial firmware version blinking time to finish, before we begin the deliberate blink being controlling by the program. The second sleep will allow the LED to stay on for one second before being turned off. The final sleep ensures the last message will be transmitted to the remote device before the connection is closed.

```c++
    // Clean-up and exit
    board.detach();
    usb.end();
    return 0;
}
```

`RemoteDevice::detach` will disconnect remote device from the underlying serial channel. `Stream::end` will close the communication channel and return those resources to the operating system. Finally, `return 0;` exits the program cleanly and returns control to the terminal.

### References:

* [Wiring API - Wiring.org.co](http://wiring.org.co/reference/)
* [Wiring API - Arduino.cc](https://www.arduino.cc/en/Reference/HomePage)
* [Wiring API - Particle.io](https://docs.particle.io/reference/firmware/photon/#input-output)

### Project Organization:

Technical documentation, requirements and diagrams will be stored in the `docs/` folder. Sample programs are located in the `samples/` folder. Google Mock unit-tests are located in the `tests/` folder.

### Software License:

This project uses the _**MIT license**_, please consult the license file for details.

