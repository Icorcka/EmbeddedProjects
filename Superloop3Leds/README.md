# Superloop3Leds — Three Independent Blink Rates in One Loop

ESP32-S3 (ESP-IDF, C) program that blinks three LEDs at three independent periods from a single non-blocking superloop.

## What it does
- LED1 toggles every 200 ms
- LED2 toggles every 500 ms
- LED3 toggles every 1000 ms

All three are driven from the same `app_main` loop with no `delay()` and no inner `while()` — each LED keeps its own `last_ms` timestamp and is toggled when `millis() - last_ms >= period_ms`. `millis()` is implemented on top of `esp_timer_get_time()`.

## Pins
- LED1 — GPIO 42 (200 ms)
- LED2 — GPIO 47 (500 ms)
- LED3 — GPIO 37 (1000 ms)

## Build
PlatformIO + ESP-IDF — open the folder and run `pio run -t upload`.
