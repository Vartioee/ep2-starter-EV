// 1. LED turns ON only when a_y >= 5.0 m/s^2.
// 2. Readings:
//    - Flat (1.59, -0.48, 9.14)   => a_y = -0.48 m/s^2 -> LED OFF
//    - Up (3.80, 8.65, 1.77) => a_y =  8.65 m/s^2 -> LED ON
//    - Down (-0.34, -9.80, 2.35) => a_y = -9.80 m/s^2 -> LED OFF
// 3. Tilt on z and x axis doesnt matter and only y axis is used to determine if the LED should be ON or OFF, since only up is ON.
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>

#define POT_PIN 4
#define LED_PIN 40

constexpr float TILT_THRESHOLD_M_S2 = 5.0f;

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  delay(300);
  Wire.begin(8, 9);  // SDA 8, SCL 9
  pinMode(LED_PIN, OUTPUT);

  if (!mpu.begin()) {
    Serial.println("MPU-6050 not found — run the I2C scanner first.");
    while (true) delay(1000);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

void loop() {
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  int raw = analogRead(POT_PIN);  // 0 .. 4095
  int duty = raw / 16;            // 0 .. 255
  if (duty > 255) duty = 255;

  const bool tiltEnabled = a.acceleration.y >= TILT_THRESHOLD_M_S2;
  analogWrite(LED_PIN, tiltEnabled ? duty : 0);

  // Keep serial output as x,y,z CSV so the recording tool can consume it.
  Serial.printf("%.2f,%.2f,%.2f\n",
                a.acceleration.x, a.acceleration.y, a.acceleration.z);
  
  Serial.printf("raw %4d -> duty %3d\n", raw, duty);

  delay(2000);  // Wait 2 seconds before the next reading to avoid flooding the serial output and to give time for the LED to be visible. Made getting samples easier.
}
