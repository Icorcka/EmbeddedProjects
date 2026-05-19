# EmbeddedProjects

A collection of small embedded projects, mostly ESP32 / ESP32-S3.

## Highlights
- **[SmartLights](SmartLights/)** — adaptive night light on ESP32-S3. Reads an LDR at 50 Hz, runs a moving-average filter, auto-calibrates min/max ambient light on boot, applies hysteresis and software fading, and drives 8 LEDs via PWM. Non-blocking architecture (`millis()`), sensor fault detection, Serial diagnostics.
- **[TrafficLight](TrafficLight/)** — Ukrainian-rules traffic-light FSM on ESP32-S3 (ESP-IDF + FreeRTOS, C++17, no Arduino). Two FreeRTOS tasks, GPIO ISR + Task Notifications (no globals), `esp_timer_get_time()` timing, button-triggered night mode (yellow blink) with safe restart from Red, all timings/pins as `constexpr`.

## Other projects
- [Lb1](Lb1/) — police-flasher light pattern with mode switching.
- [SpeedLights](SpeedLights/) — cycle through blink speeds using an external button and the BOOT button.
- [AdcLdr](AdcLdr/) — LDR ADC reader on ESP32-S3, compares calculated vs. `analogReadMillivolts()` voltage and sweeps resolution/attenuation.
- [BlinkCpp](BlinkCpp/) — classic blink rewritten in Embedded C++ on ESP-IDF: superloop, no heap/STL, `constexpr` config, ISR-driven button switching three LED modes.
- [RelayMotor](RelayMotor/) — relay contact response-time measurement (ISR-based timing, 10-sample averaging) plus soft PWM motor speed control via potentiometer and LEDC.
- [Stm32AdcPot](Stm32AdcPot/) — STM32F411 (BlackPill, STM32Cube HAL) potentiometer ADC: compares manual vs. VREFINT-calibrated voltage and tabulates the error.
