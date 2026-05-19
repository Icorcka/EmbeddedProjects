# BlinkCpp — Embedded C++ Blink on ESP32

Classic Arduino blink, rewritten in Embedded C++ style on top of ESP-IDF (FreeRTOS / GPIO driver). Used to practice the conventions you actually want in real firmware: no heap, no STL, no globals beyond what is strictly required, a superloop architecture and an interrupt-driven button.

## What it does
- Drives an LED in three modes — **Blink → Always On → Always Off → Blink → …** — switched by a single GPIO button.
- The button is wired through `attachInterrupt`-style ISR (`gpio_isr_handler_add`) with HW debouncing inside the ISR. The ISR is minimal: it only sets a `volatile bool` flag; mode-switching logic happens in the superloop.
- All timing is non-blocking, driven by `esp_timer_get_time()` / FreeRTOS ticks — no `delay()`.
- Measures the cost of the superloop: every 1000 iterations it prints total + average µs per iteration to Serial.

## Embedded C++ patterns demonstrated
- `enum class Led::State` for LED state instead of `bool` / magic ints.
- `constexpr` (in `Config` namespace) for pin numbers, periods and counts — no magic numbers in the code.
- `static const` / `inline constexpr` for configuration; no dynamic allocation, no STL containers.
- Superloop in `app_main()`, no blocking calls.
- `volatile` only where actually needed (ISR-shared flag).
- `Led` class with `init()` / `set(State)` — peripheral access wrapped in a small RAII-style object.

## Pins
- LED — GPIO 11
- Button — GPIO 7 (active-low, pulled up)

## Build
PlatformIO + ESP-IDF framework — open the folder and run `pio run -t upload`.
