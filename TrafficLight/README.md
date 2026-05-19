# Traffic Light — ESP32-S3

Mini-project: a traffic light following Ukrainian road rules, built on **ESP-IDF + FreeRTOS**. No Arduino, no `delay()`.

## Framework

- ESP-IDF (PlatformIO, board `esp32s3usbotg`).
- FreeRTOS (two tasks + a hardware interrupt).
- C++17, native `driver/gpio.h`, timing via `esp_timer_get_time()`.

## Pinout

| Signal | GPIO |
|--------|------|
| Green  | 6    |
| Yellow | 17   |
| Red    | 8    |
| Button | 45   |

The button is wired to GND (active-low), with the internal `GPIO_PULLUP_ENABLE` pull-up.

## Cycle

`Green (5s) → Green blinking (3s) → Yellow (2s) → Red (5s) → Red+Yellow (1s) → Green`.

All durations and pin numbers are `constexpr` in [`include/Config.h`](include/Config.h) — no magic numbers in the rest of the code.

## Button — night mode

A press switches the traffic light into **yellow blinking** (night) mode. Pressing again returns to the normal cycle and safely restarts from **Red**.

## Completed parts

- [x] **Core logic (FSM)** — `enum class TrafficLight::State`, standard Ukrainian cycle.
- [x] **Non-blocking** — `esp_timer_get_time()` for state timing; no `delay()` and no blocking waits inside the FSM.
- [x] **Architecture** — `Led` class, `TrafficLight` class, all parameters as `constexpr` in `Config.h`.
- [x] **Optional 1: interrupts & button** — `gpio_isr_handler_add`; the ISR only sends a task notification; debouncing happens in a task.
- [x] **Optional 2: ESP-IDF + FreeRTOS** — `app_main`, two tasks (`tl`, `btn`), synchronization via **Task Notifications** (no global `volatile`).

## Layout

```
include/
  Config.h          // pins, timings, task priorities — all constexpr
  Led.h             // Led class declaration
  TrafficLight.h    // TrafficLight class with nested enum class State
src/
  Led.cpp           // Led implementation
  TrafficLight.cpp  // FSM driven by esp_timer_get_time()
  main.cpp          // app_main, two FreeRTOS tasks, ISR, notifications
  CMakeLists.txt    // registers ../include as INCLUDE_DIRS
```

## Build & flash

```bash
pio run                  # build
pio run -t upload        # flash
pio device monitor       # serial log
```
