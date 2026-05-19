# EmbeddedProjects

A collection of small embedded projects, mostly ESP32 / ESP32-S3.

## Highlights
- **[SmartLights](SmartLights/)** — adaptive night light on ESP32-S3. Reads an LDR at 50 Hz, runs a moving-average filter, auto-calibrates min/max ambient light on boot, applies hysteresis and software fading, and drives 8 LEDs via PWM. Non-blocking architecture (`millis()`), sensor fault detection, Serial diagnostics.

## Other projects
- [Lb1](Lb1/) — police-flasher light pattern with mode switching.
- [SpeedLights](SpeedLights/) — cycle through blink speeds using an external button and the BOOT button.
- [AdcLdr](AdcLdr/) — LDR ADC reader on ESP32-S3, compares calculated vs. `analogReadMillivolts()` voltage and sweeps resolution/attenuation.
