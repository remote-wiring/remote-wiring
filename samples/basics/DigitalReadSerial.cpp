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
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

// digital pin 2 has a pushbutton attached to it. Give it a name:
int pushButton = 2;

// the setup routine runs once when you press reset:
void setup(remote_wiring::RemoteDevice & uno) {
  // communication is enabled by default via stdin, stdout and stderr
  // make the pushbutton's pin an input:
  uno.pinMode(pushButton, INPUT);
}

// the loop routine runs over and over again forever:
void loop(remote_wiring::RemoteDevice & uno) {
  // read the input pin:
  int buttonState = uno.digitalRead(pushButton);
  // print out the state of the button:
  std::cout << buttonState << std::endl;
  delay(1);        // delay in between reads for stability
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

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
