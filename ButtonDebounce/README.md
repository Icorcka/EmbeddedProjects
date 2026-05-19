# ButtonDebounce — Polling-based GPIO Button Debounce

ESP32-S3 (ESP-IDF, C++) implementation of a software debouncer for a GPIO button. This is the final variant of an exercise that worked through five approaches — raw interrupt, time-based debounce in ISR, state-based debounce in `loop()`, polling with a debounce state machine, and an RC-filter hardware-debounce comparison — and the code here is the polling + state-machine version, which gave the most stable behavior.

## What it does
- Polls `GPIO 37` every 10 ms inside a FreeRTOS task (no interrupts).
- Tracks a small `RELEASED → PRESSED → RELEASED` state machine; a transition is only accepted after 4 consecutive samples (≈40 ms) of the same level — that's the debounce.
- Counts each accepted press and logs it via `ESP_LOGI` (`press #N`). Release does not trigger an event.
- Active-low button (`BUTTON_LEVEL_PRESSED = 0`) with internal pull-up enabled.

## Pins
- Button — GPIO 37 (to GND through the button; internal pull-up enabled)
- Optional hardware debounce on the RC test: 100 Ω series + 100 nF to GND, 10 kΩ pull-up

## Build
PlatformIO + ESP-IDF — open the folder and run `pio run -t upload`.
