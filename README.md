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

#### Remote Wiring Usage

```c++
#include <remote_wiring>
#include <serial_wiring>

using namespace remote_wiring::boards::arduino::uno;
using namespace remote_wiring::wiring;

int main (int argc, char * argv[]) {
    UartSerial usb = new UartSerial("/dev/ttyACM0");
    RemoteDevice arduino_uno(usb);

    // Connect to the remote device
    usb.begin();
    arduino_uno.attach();

    // use the Wiring interface on the RemoteDevice

    arduino_uno.detach();
    usb.end();
    return 0;
}
```
