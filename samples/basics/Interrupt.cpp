/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "RwArduino.h"

/*
  Interrupt
  Toggle LED with button press, powered by interrupts.

  This sample uses an interrupt service routine (ISR) named `blink` to toggle
  a global state variable and then uses the built-in LED to display the value
  of the cached state to the user. The work of updating the cached state happens
  asychronously in the interrupt service routine.

  This example could be easily modified to update the built-in LED in the ISR,
  which would allow the loop to be empty but still allow actions and reactions
  to occur.

  This example code is in the public domain.
  (https://www.arduino.cc/en/Reference/attachInterrupt)

  adapted 1 APR 2017
  by Zachary J. Fields
*/

using namespace remote_wiring::boards::arduino::uno;  // change to your board

const byte interruptPin = 2;
volatile byte state = LOW;

void blink (void * context) {
  (void)context;
  state = !state;  // toggle the cached state
}

// the setup function runs once when you press reset or power the board
void setup (remote_wiring::RemoteDevice & board) {
  // initialize digital pin LED_BUILTIN as an output.
  board.pinMode(LED_BUILTIN, OUTPUT);
  // initialize digital pin as input with pull-up resistor (i.e. always HIGH, unless deliberately pulled LOW)
  board.pinMode(interruptPin, INPUT_PULLUP);
  // attach an interrupt service routine to be executed when pin state changes
  board.attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

// the loop function runs over and over again forever
void loop (remote_wiring::RemoteDevice & board) {
  // update the LED to reflect the cached state updated with interrupts
  board.digitalWrite(LED_BUILTIN, state);
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
