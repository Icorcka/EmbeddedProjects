# SpeedLights — ESP32 Blink Speed Switcher

ESP32 program that blinks LEDs and lets you switch between several blink speeds using an external button and the on-board BOOT button.

## What it does
- Blinks red, blue and green LEDs in sync via `delay()`-style timing.
- Cycles through 5 blink speeds (1000 / 500 / 250 / 100 / 50 ms).
- External button (GPIO 39) — step forward to the next (faster) speed.
- BOOT button (GPIO 0) — step backward to the previous (slower) speed; a long press (≥1 s) jumps back two steps.
- Logs the active speed to Serial Monitor at 115200 baud.

## Pins
- Red LED — GPIO 3
- Blue LED — GPIO 18
- Green LED — GPIO 6
- External button — GPIO 39 (uses `INPUT_PULLUP`, button to GND)
- BOOT button — GPIO 0 (built-in)

## Build
PlatformIO project — open the folder and run `pio run -t upload`.
