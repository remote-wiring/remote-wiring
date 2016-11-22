wiring
==============

## Library details:

The `wiring` library provides an object-oriented [Wiring](http://wiring.org.co/) interface to any* remote embedded device connected over a [supported serial connection](https://github.com/remote-wiring/serial-wiring).

Technical documentation, requirements and diagrams will be stored in the `docs/` folder. Sample programs are located in the `samples/` folder. Google Mock unit-tests are located in the `tests/` folder.

**The target device must have a C++ compiler and be running a firmata host program (i.e. [StandardFirmata.ino](https://github.com/firmata/arduino/tree/master/examples/StandardFirmata)).*

## Software License:
This project uses the ***MIT license***, please consult the license file for details.

## References:

[Wiring API - Wiring.org.co](http://wiring.org.co/reference/)  
[Wiring API - Arduino.cc](https://www.arduino.cc/en/Reference/HomePage)  
[Wiring API - Particle.io](https://docs.particle.io/reference/firmware/photon/#input-output)  

## Example code:

#### Arduino Blink via Remote Wiring

```c++
#include <chrono>
#include <thread>

#include <RemoteDevice.h>
#include <UartSerial.h>

#define delay(ms_) std::this_thread::sleep(std::chrono::milliseconds(ms_))

using namespace remote_wiring;
using namespace remote_wiring::wiring;
using namespace remote_wiring::transport;

void setup (void);
void loop (void);

int main (int argc, char * argv[]) {
    Stream * stream = new UartSerial("/dev/ttyO0");
    RemoteDevice arduino_uno(stream);

    // Connect to the remote device
    stream->begin();

    // Arduino Processing Loop
    setup();
    for (;;) { loop(); }

    stream->end();
    delete stream;
    return 0;
}

// the setup function runs once when you press reset or power the board
void setup (void) {
    // initialize digital pin 13 as an output.
    arduino_uno.pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop (void) {
    arduino_uno.digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);  // wait for a second
    arduino_uno.digitalWrite(13, LOW);  // turn the LED off by making the voltage LOW
    delay(250);  // wait for a second
}
```
