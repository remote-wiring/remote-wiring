/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "RwArduino.h"

/*
  DigitalReadSerial
  Reads a digital input on pin 2, prints the result to the serial monitor

  This example code is in the public domain.
 */

using namespace remote_wiring::boards::arduino::uno;  // change to your board

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

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
