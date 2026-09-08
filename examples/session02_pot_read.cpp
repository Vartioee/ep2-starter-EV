/*
 * Session 2 — the analog way: read a potentiometer.
 * --------------------------------------------------------------------------
 * Copy this into src/main.cpp, then build + upload.
 *
 * WIRING (three wires, power off while you wire):
 *   pot outer leg 1 -> 3V3
 *   pot outer leg 2 -> GND
 *   pot middle leg  -> GPIO4          <- the wiper: this is the one you read
 *
 * Turn the knob and watch the number sweep. That is the whole exercise: a
 * continuously variable voltage becoming a number your program can use.
 *
 * WHAT THE NUMBER MEANS
 *   analogRead() gives a raw count from a 12-bit converter: 0..4095.
 *   0 = the wiper is at GND, 4095 = the wiper is near 3V3.
 *
 *   Deliberately NOT printing volts here. "raw * 3.3 / 4095" is the idealised
 *   conversion you will see all over the internet, but the ESP32's converter
 *   is not perfectly linear and the real transfer function depends on the
 *   attenuation setting. If you actually need volts, ask the core for a
 *   calibrated reading instead:  analogReadMilliVolts(POT_PIN)
 *   For dimming an LED next, the raw count is all you need.
 *
 * WHY GPIO4: it is an ADC1 pin, it is free on this board, and it is not one of
 * the strapping pins (avoid GPIO0/3/45/46) or the octal flash/PSRAM pins
 * (GPIO33-37). GPIO8 and GPIO9 are the I2C bus. GPIO2 carries the scope test
 * signal from session 1.
 */
#include <Arduino.h>

#define POT_PIN 4

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println();
  Serial.println("Potentiometer — turn the knob.");
}

void loop() {
  int raw = analogRead(POT_PIN);          // 0 .. 4095
  Serial.println(raw);
  delay(100);                             // 10 readings a second is plenty to watch
}
