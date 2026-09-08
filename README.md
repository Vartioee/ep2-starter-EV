# Embedded Programming 2 — starter project

Everything you need to start each lab. **Fork** it once, clone your fork, and
each session swap the active sketch and (later) drop in your trained model.

This fork is your **individual** work all semester. From session 3 your team
also makes its own separate repository — that is where the project lives.

**Board:** ESP32-S3-DevKitC-1 · **Framework:** Arduino · **Tool:** PlatformIO (in VS Code)

---

## Quick start

1. Install **VS Code** + the **PlatformIO IDE** extension. Wait for it to finish
   downloading its toolchain, then restart.
2. **File → Open Folder →** this project.
3. Bottom blue bar: **✓ Build**, **→ Upload**, **🔌 Serial Monitor**.
   Or in the PlatformIO terminal: `pio run -t upload && pio device monitor`

The board that ships in this repo is **Session 1's blink + hello**. Flash it
first to prove your whole toolchain works.

> **The LED:** this board has no simple on/off LED — it has one addressable RGB
> LED on **GPIO38** (check the silkscreen: ours reads `RGB@IO38`). The blink
> uses it, but the real proof is the **serial "hello"**, which always works.

> **GPIO2 is a test signal, not a mistake.** `setup()` starts a 1 kHz square
> wave on GPIO2 and leaves it running. It's there for the oscilloscope block in
> session 1 — probe it and you'll find a wave in seconds. The LED pin carries
> pulses for only ~30 µs at a time, which is a much harder first capture.

---

## How the project is organised

| Path | What |
|---|---|
| `src/main.cpp` | The **active** sketch that gets built + flashed. |
| `examples/` | Per-session reference sketches — **copy one into `src/main.cpp`** to use it. |
| `include/` | Headers (e.g. `test_images.h` for session 6). |
| `lib/` | Libraries — **drop your Edge Impulse model here** (session 4). |
| `tools/` | Helper scripts (image → C array). |
| `platformio.ini` | The build recipe (board, libraries, flags). **Don't prune it** — see Notes. |
| `report-template.md` | The final project report skeleton — copy it into your team repo (session 10). |

PlatformIO only builds `src/` + `lib/`, so the files in `examples/` sit safely
out of the build until you copy one in.

---

## Session by session

- **Session 1 — first flash:** flash `src/main.cpp` as-is (blink + hello).
  - Homework on real hardware? Copy `examples/session01_plain_led.cpp` → `src/main.cpp` and wire a plain LED on **GPIO40** (long leg → 220–330 Ω resistor → GPIO40, short leg → GND). Session 2 reuses the same pin.
- **Session 2 — sensor + data:**
  - Wire the MPU-6050: **3V3 → VCC, GND → GND, GPIO8 → SDA, GPIO9 → SCL** *(power off while wiring)*.
  - Copy `examples/session02_i2c_scanner.cpp` → `src/main.cpp`, flash → find `0x68`.
  - Copy `examples/session02_mpu_read.cpp` → `src/main.cpp` → read motion, sanity-check gravity on Z.
  - Analog first: copy `examples/session02_pot_read.cpp` → `src/main.cpp` → turn the knob, watch the number sweep.
  - Record CSVs: `pio device monitor --quiet > wave_01.csv` (one file per gesture).
    **Commit and push the CSVs** — you need them in session 3.
  - Homework: `examples/session02_knob_dims_led.cpp` → analog in, PWM out → `FINAL: knob dims LED`.
- **Session 3 — train:** upload your CSVs to Edge Impulse and train. *(We install the Edge Impulse CLI together in the lab if we need the live data forwarder — don't fight with it at home.)*
- **Session 4 — deploy:** export your model as an **Arduino library**, unzip into `lib/`, then work from `examples/session04_deploy.cpp` (edit the `#include` to your project's header).
- **Session 5 — security:** copy `examples/session05_plant_secret.cpp` → `src/main.cpp`, flash, then dump the flash and find your secret (see that file's header).
- **Session 6 — vision:** clone the Edge Impulse project linked in Moodle, train it, deploy it, and feed it a baked-in test image from `include/test_images.h`. Then push the resolution until the board gives up and record where — `ceiling.md`, `FINAL: my ceiling`.
- **Session 7 — platform:** your team's platform sentence goes in your **team** repo's `README`.
- **Sessions 8–10 — project:** the project lives in your **team** repo, not this fork. Copy `report-template.md` across when you write the report (2 pages max).

---

## Wiring: MPU-6050 (GY-521)

| MPU-6050 | ESP32-S3 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 8 |
| SCL | GPIO 9 |

*(If your board's I²C defaults differ, change the pins in the session-2 sketches.)*

---

## Notes

- **Don't "simplify" `platformio.ini`.** The octal-flash lines
  (`board_build.flash_mode = opi`, `board_build.arduino.memory_type = opi_opi`)
  and `-D BOARD_HAS_PSRAM` are **required** on our boards, not optional extras.
  Take them out and the board doesn't just lose PSRAM — it panics in a boot loop
  before your code runs:
  `assert failed: do_core_init startup.c:328 (flash_ret == ESP_OK)`.
  This has already caught us once. The file is commented; read it, don't prune it.
- **Can't upload / can't dump flash?** Hold **BOOT**, tap **RESET**, release,
  then retry — that forces download mode.
