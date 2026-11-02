# My ceiling — person detection on the ESP32-S3

<!--
Session 6. Fill this in as you go, then commit it as "FINAL: my ceiling".
- 96 × 96: GUESS the time before you flash, then write the measured time.
- 160 × 160 and 48 × 48: PREDICT from your measured 96 px time
  (× 2.8 and ÷ 4 — slide "More pixels, more stops"), then measure.
- Accuracy: the validation accuracy on Edge Impulse's "Transfer learning" page
  after training (for 96 px it is already there in the cloned project).
- The three questions at the bottom are the homework: answer each with
  "time" or "memory" and one reason — from your measurements or the slides.
-->

| Image size | My prediction             | Measured | FPS | Accuracy |
|------------|---------------------------|----------|-----|----------|
| 96 × 96    | guess:  560 ms            | 4362 ms  | 0.2 |  82 %    |
| 160 × 160  | 96 px time × 2.8 = 13000ms| 12078 ms | 0.1 |  98 %    |
| 48 × 48    | 96 px time ÷ 4 = 980 ms   | 1021 ms  | 1.0 |  51 %    |

## Which budget breaks first — time or memory?

1. A doorbell: decide "person or not" within 1 second of the button press, at 96 × 96.
   → Time, when measured, only does 0.2fps
2. A people counter: keeps one 320 × 240 colour photo in memory and checks it once a minute.
   → Memory, exceeds boards capacity
3. Your team's project: which budget is tighter for your model, and why?
   → Memory, with 5 sensors.