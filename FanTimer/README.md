# FanTimer — Cyclic Extraction Fan Controller

ESP32-S3 (ESP-IDF) program that runs an extraction fan (motor or relay) on a fixed duty cycle — every period, the fan turns on for a set duration, then back off automatically. The whole thing runs from hardware-backed `esp_timer` callbacks, so the cycle keeps ticking independent of `loop()` / any task code.

## What it does
- A periodic `esp_timer` fires once per cycle and calls `fan_on_cb`, which switches the motor + indicator LED on, logs the event, and arms a one-shot off-timer.
- The one-shot off-timer fires after `ON_DURATION_US` and runs `fan_off_cb`, which switches everything off and logs.
- Re-entry guard: `fan_on_cb` skips with a warning if the fan is already running, so re-arming can't double-start the motor.
- No `delay()`, no busy-wait — `app_main()` just configures the GPIOs, creates the timers, fires the first cycle, and returns. The cycle continues from timer callbacks.
- An LED on a separate GPIO mirrors the motor state for at-a-glance debugging.

## Timing (debug values currently in the code)
`ON_DURATION_US` and `OFF_DURATION_US` are `constexpr` at the top of `main.cpp`. The committed values are short for bench testing — ON = 60 s, OFF = 300 s (so the full cycle = 6 minutes). For production (15 min ON every hour), set:

```
ON_DURATION_US  = 15ULL * 60 * 1000 * 1000;   // 15 min
OFF_DURATION_US = 45ULL * 60 * 1000 * 1000;   // 45 min — gives 1 h period
```

## Pins
- Motor / relay control — GPIO 42
- Status LED — GPIO 46

## Build
PlatformIO + ESP-IDF — open the folder and run `pio run -t upload`.
