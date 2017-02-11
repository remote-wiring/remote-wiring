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
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 The analogWrite() function uses PWM, so if
 you want to change the pin you're using, be
 sure to use another PWM capable pin. On most
 Arduino, the PWM pins are identified with
 a "~" sign, like ~3, ~5, ~6, ~9, ~10 and ~11.

 This example code is in the public domain.
*/

int led = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup function runs once when you press reset or power the board
void setup(remote_wiring::RemoteDevice & uno) {
  // declare pin 9 to be an output:
  uno.pinMode(led, OUTPUT);
}

// the loop function runs over and over again forever
void loop(remote_wiring::RemoteDevice & uno) {
  // set the brightness of pin 9:
  uno.analogWrite(led, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  // wait for 30 milliseconds to see the dimming effect
  delay(30);
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
