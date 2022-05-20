// Uses two orange LEDS plus resistors on pins 4 and 7 (on the Nano 33 IoT; change for other boards).
// Don't pull more than 7mA from any pin.

#include <SAMD21turboPWM.h>

TurboPWM pwm;

void setup() {
  pwm.setClockDivider(9, false);
  pwm.timer(1, 1, 1000, true);
}

void loop() {
  // pin out and % of PWM 200 min 20 %
  pwm.analogWrite(4, 1000-430);
  delay(25);
  //pwm.analogWrite(7, random(300, 1000));
  //delay(25);
}
