/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "RwArduino.h"

/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

using namespace remote_wiring::boards::arduino::uno;  // change to your board

// the setup routine runs once when you press reset:
void setup(remote_wiring::RemoteDevice & board) {
  // communication is enabled by default via stdin, stdout and stderr
  (void)board;
}

// the loop routine runs over and over again forever:
void loop(remote_wiring::RemoteDevice & board) {
  // read the input on analog pin 0:
  int sensorValue = board.analogRead(A0);
  // print out the value you read:
  std::cout << sensorValue << std::endl;
  delay(1);        // delay in between reads for stability
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
