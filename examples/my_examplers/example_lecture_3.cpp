// Tilt: use Z; enable at >= 5.0 m/s^2, below the upright +9.3 m/s^2 samples.
// Readings: upright idle is about +9.2 to +9.7 m/s^2 in idle_01.csv; sideways/down were not recorded separately.
// Testing surprise: shake_01.csv has acceleration spikes above 20 m/s^2, so judge the tilt while still.
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

  const bool tiltEnabled = a.acceleration.z >= TILT_THRESHOLD_M_S2;
  analogWrite(LED_PIN, tiltEnabled ? duty : 0);

  // Keep serial output as x,y,z CSV so the recording tool can consume it.
  Serial.printf("%.2f,%.2f,%.2f\n",
                a.acceleration.x, a.acceleration.y, a.acceleration.z);

  delay(20);  // ~50 Hz
}


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

  const bool tiltEnabled = a.acceleration.z >= TILT_THRESHOLD_M_S2;
  analogWrite(LED_PIN, tiltEnabled ? duty : 0);

  // Keep serial output as x,y,z CSV so the recording tool can consume it.
  Serial.printf("%.2f,%.2f,%.2f\n",
                a.acceleration.x, a.acceleration.y, a.acceleration.z);

  delay(20);  // ~50 Hz
}
