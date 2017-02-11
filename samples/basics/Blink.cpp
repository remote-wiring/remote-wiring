/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include <chrono>
#include <iostream>
#include <thread>

#include <remote_wiring>
#include <serial_wiring>

#define delay(ms_) std::this_thread::sleep_for(std::chrono::milliseconds(ms_))

using namespace remote_wiring::boards::arduino::uno;
using namespace remote_wiring::wiring;

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 //%% Aduino Sketch Environment %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald

  modified 2 Sep 2016
  by Arturo Guadalupi

  modified 8 Sep 2016
  by Colby Newman
*/

// the setup function runs once when you press reset or power the board
void setup(remote_wiring::RemoteDevice & uno) {
  // initialize digital pin LED_BUILTIN as an output.
  uno.pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop(remote_wiring::RemoteDevice & uno) {
  uno.digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                           // wait for a second
  uno.digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                           // wait for a second
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::atomic_bool run;

void sketch_thread(remote_wiring::RemoteDevice & uno) {
  setup(uno);
  for (run = true ; run ; ) { loop(uno); }
}

int main (int argc, char * argv []) {
  if ( argc < 2 ) { std::cout << "Usage: " << argv[0] << " <serial device descriptor>" << std::endl; return -1; }

  serial_wiring::UartSerial usb(argv[1]);
  remote_wiring::FirmataDevice uno(usb);

  usb.begin(57600);
  uno.attach();
  uno.survey();

  // Launch Arduino sketch environment
  std::thread sketch(sketch_thread, std::ref(uno));

  // Prompt for exit
  std::cout << std::endl << "Processing sketch on remote device." << std::endl;
  if ( uno.firmware() ) {
    std::cout << "(detected firmware as: " << uno.firmware() << " - v" << uno.version().sv_major << "." << uno.version().sv_minor << ")" << std::endl;
  } else {
    std::cout << "(unable to detect firmware)" << std::endl;
  }

  std::cout << std::endl << "Press any key to exit sketch..." << std::endl;
  ::getchar();
  run = false;
  sketch.join();
  std::cout << "Sketch environment has exited." << std::endl;

  uno.detach();
  usb.end();
  return 0;
}
