/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "RwArduino.h"

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

using namespace remote_wiring::boards::arduino::uno;  // change to your board

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

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
