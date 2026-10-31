/*
* Session 4 — run YOUR trained model on the board.
* --------------------------------------------------------------------------
* PREREQUISITE (do this first, or it will NOT compile — that's expected):
*   1. In Edge Impulse: Deployment -> "Arduino library" -> EON + int8 -> Build.
*   2. Unzip the downloaded .zip into the lib/ folder of this project.
*   3. Change the #include below to match the header the zip created
*      (look in lib/<your-project>/  -> it ends in _inferencing.h).
*
* This is the honest shape of on-device ML:
*   read the sensor -> fill a window -> run the model -> act on the result.
* No web server, no network — just the sensor and the model.
*/
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>

// 👇 CHANGE THIS to your Edge Impulse project's header:
#include <vartioee-project-1_inferencing.h>

const char* ssid = "Savonia-IoT";
const char* password = "SuperSecret123"; // "temporary"... ships forever

Adafruit_MPU6050 mpu;

float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];   // the window — allocated once
int   i = 0;                                        // fill position

void act_on(ei_impulse_result_t &result) {
  // Pick the highest-confidence class.
  int   best = 0;
  float best_val = 0.0f;
  for (int k = 0; k < EI_CLASSIFIER_LABEL_COUNT; k++) {
    if (result.classification[k].value > best_val) {
      best_val = result.classification[k].value;
      best = k;
    }
  }
  // Your threshold is a DECISION, not a default. Pick it for the error that
  // hurts your project more (a missed event vs a false trigger).
  if (best_val > 0.67f) {
    Serial.printf(">> %s (%.0f%%)\n", result.classification[best].label, best_val * 100.0f);
    // TODO: your action here — light the RGB LED, move a servo, send a message.

    const char *label = result.classification[best].label;

    Serial.printf(">> Detected: %s (%.1f%%)\n", label, best_val * 100.0f);

    if (strcmp(label, "shake") == 0) {
      // Trigger action for "shake"
      delay(200);
      
    } else if (strcmp(label, "wave") == 0) {
      // Trigger action for "wave"
      Serial.println("Action: Wave detected! Performing task...");
      
    } else if (strcmp(label, "idle") == 0) {
      // Keep outputs in safe default state
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Wire.begin(8, 9);
  if (!mpu.begin()) { Serial.println("MPU not found"); while (true) delay(1000); }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

void loop() {
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);
  buffer[i++] = a.acceleration.x;
  buffer[i++] = a.acceleration.y;
  buffer[i++] = a.acceleration.z;

  if (i >= EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
    signal_t signal;
    numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);

    ei_impulse_result_t result;
    run_classifier(&signal, &result, false);
    act_on(result);
    i = 0;
  }

  delay(20);   // 50 Hz — the rate you trained at! (drop this and the model sees garbage)
}
