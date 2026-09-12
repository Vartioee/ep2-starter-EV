/*
 * Session 1 — blink a plain LED (the homework, on real hardware).
 * --------------------------------------------------------------------------
 * The blink in src/main.cpp uses the board's addressable RGB LED, which you
 * send a COLOUR to. This one uses an ordinary LED from your box, which you
 * switch ON and OFF. It is the same idea with simpler hardware — and it is
 * the same sketch you ran in the simulator.
 *
 * WIRING (power off while you wire):
 *   GPIO40 -> resistor (220-330 ohm) -> LED long leg (anode)
 *   LED short leg (cathode) -> GND
 *
 *   THE RESISTOR IS NOT OPTIONAL. Straight across a pin, an LED draws more
 *   current than the pin should give. 220 ohm is bright, 330 is comfortable,
 *   1k works but is dim. Long leg = positive.
 *
 * WHY GPIO40: it's free on this board, it's not a strapping pin (avoid
 * GPIO0/3/45/46), and it's clear of the I2C bus (8/9), the session-1 scope
 * signal (GPIO2) and the octal flash/PSRAM pins (33-37). Session 2's homework
 * reuses the same pin, so you can leave the LED where it is.
 *
 * THE HOMEWORK: make this blink YOURS. A rhythm, a pattern, morse code, a
 * heartbeat — something you designed. Then push it as `FINAL: my blink`.
 */
#include <Arduino.h>

#define LED_PIN 40

void setup() {
  Serial.begin(115200);
  delay(300);
  pinMode(LED_PIN, OUTPUT);
  Serial.println();
  Serial.println("Plain LED blinking on GPIO40.");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // on
  delay(500);                    // <-- wait half a second

  digitalWrite(LED_PIN, LOW);    // off
  delay(500);                    // <-- and again
}


/*
 * Embedded Programming 2 — Session 1: your first flash
 * --------------------------------------------------------------------------
 * The smallest thing that proves the WHOLE chain works:
 *   your code -> compiler -> USB -> real silicon -> and back to your screen.
 *
 * About the LED: this board has no ordinary on/off LED. It carries a single
 * addressable RGB LED (a WS2812), so you don't switch it on — you send it a
 * colour. neopixelWrite(pin, red, green, blue) does that for you.
 *
 * Check the silkscreen next to the LED for its pin! Ours says "RGB@IO38".
 * Other ESP32-S3 boards put it on GPIO48 — if yours does, change the line
 * below. (This is why LED_BUILTIN doesn't help here: it assumes GPIO48.)
 *
 * The serial "hello" is the guaranteed proof: if you see it in the monitor,
 * your toolchain, your cable and your board are all fine.
 *
 * SCOPE_PIN is for the PicoScope block later in the session: a plain 1 kHz
 * square wave, always running, that you can find on the scope in seconds.
 * It is deliberately NOT the LED pin — see the note above ledcSetup().
 */
#include <Arduino.h>

#define RGB_LED_PIN 38   // silkscreen: RGB@IO38
#define SCOPE_PIN    2   // J3 pin 5, four pins down from GND — probe this one

void setup() {
  Serial.begin(115200);
  delay(300);                                 // let serial come up before printing
  Serial.println();
  Serial.println("Hello from ESP32-S3 — the whole chain works!");

  /*
   * A test signal for the oscilloscope.
   *
   * Why a separate pin, when the LED is already blinking? Because the LED is
   * a WS2812: its pin sits idle-low and carries a ~30 us burst of pulses only
   * at the instant the colour changes. Twice a second, for 30 microseconds —
   * that is 0.006 % of the time. Finding that on a scope is a genuinely hard
   * first capture. GPIO2 gives you an easy one: a steady square wave that is
   * always there.
   *
   * LEDC is the ESP32's hardware PWM. Once set up it runs on its own, so the
   * delay(500) calls in loop() below don't disturb it at all.
   */
  ledcSetup(0, 1000, 10);        // channel 0, 1 kHz, 10-bit duty resolution
  ledcAttachPin(SCOPE_PIN, 0);
  ledcWrite(0, 512);             // 512 / 1023 = 50 % duty -> a square wave
  Serial.println("Scope test signal: 1 kHz square wave on GPIO2");
}

void loop() {
  neopixelWrite(RGB_LED_PIN, 0, 24, 0);       // dim green (r, g, b)
  Serial.println("blink: on");
  delay(1500);

  neopixelWrite(RGB_LED_PIN, 0, 0, 0);        // off
  Serial.println("blink: off");
  delay(1500);
}

