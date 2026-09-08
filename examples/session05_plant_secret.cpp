/*
 * Session 5 — plant a secret, then read it back off the flash.
 * --------------------------------------------------------------------------
 * Copy into src/main.cpp and flash. Then, on your laptop:
 *
 *   esptool.py read_flash 0x0 0x800000 dump.bin
 *   strings dump.bin | grep -i -E "savonia|secret"
 *
 * Both planted strings come back out of your own board:
 *   SuperSecret123
 *   Savonia-IoT
 *
 * WHY THAT SEARCH AND NOT grep -i pass
 *   strings prints the text that is IN the binary — the VALUES, not the names
 *   of your variables. There is no "password" in there to match, and no "ssid"
 *   either; there is "Savonia-IoT" and "SuperSecret123". Search for the word
 *   you are thinking of instead of the text that is actually there and you get
 *   nothing — or worse, you get ESP_ERR_WIFI_PASSWORD out of the framework and
 *   think you found something. That is the single most common way this
 *   exercise appears to "fail".
 *
 *   Note that a real attacker does not know your values in advance. They would
 *   just scroll:  strings dump.bin | less  — and read what looks interesting.
 *   We grep only because we planted it and know what we are looking for.
 *
 *   Prefer to see the raw truth? Drop the pipe and scroll:
 *       strings dump.bin | less
 *   Then hunt for your model's labels too — "wave", "shake", "idle" — if your
 *   session-4 model is still on the chip.
 *
 * WHY BOTH Serial.println CALLS ARE REQUIRED, NOT DECORATION
 *   With optimisation on, a string that is never used gets stripped out of the
 *   binary, and then strings|grep finds nothing. Actually using each string
 *   forces the compiler to keep it. (And printing a secret to the serial port
 *   is itself a real vulnerability — that is part of the point.)
 *
 * KEEPING YOUR MODEL IN THE DUMP
 *   The lab is better if the dump also contains your own model's labels, which
 *   means the interesting build is your session-4 project WITH these three
 *   lines pasted into its setup(), rather than this file replacing everything.
 *   If you just want the credential part to work, this file on its own is fine.
 *
 * If esptool can't connect (native-USB boards are fussy):
 *   hold BOOT, tap RESET, release — then run the command again.
 */
#include <Arduino.h>

const char* ssid     = "Savonia-IoT";
const char* password = "SuperSecret123";   // "temporary"... ships forever

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println(ssid);                     // forces the string into flash — and leaks it
  Serial.println(password);                 // same again, for the one that matters
}

void loop() {}
