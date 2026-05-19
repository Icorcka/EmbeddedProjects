# SmartLights — ESP32-S3 Smart Night Light

ESP32-S3 night light that automatically adjusts LED brightness to ambient light: the darker the room, the brighter the LEDs. Built around an LDR voltage divider with non-blocking sampling, digital filtering, calibration and smooth fading.

## What it does
- Samples an LDR through the ADC every 20 ms (50 Hz) using `millis()` — fully non-blocking.
- Runs the raw ADC reading through a moving-average filter to suppress high-frequency noise.
- Runs a startup calibration that captures the actual min/max light levels in the room so the brightness curve always uses the full dynamic range.
- Validates the sensor every cycle and rejects open / short-circuit readings (`SENSOR_SHORT_THRESHOLD`, `SENSOR_DISCONNECT_THRESHOLD`).
- Maps the filtered value into a PWM brightness with hysteresis to avoid flicker around thresholds.
- Drives 8 LEDs with software fading for smooth transitions on sharp light changes.
- Logs `raw`, `filtered`, computed voltage and `analogReadMillivolts()` to Serial at 115200 baud for diagnostics.

## Pins
- LDR — GPIO 17 (ADC, voltage divider to GND)
- LEDs — GPIO 46, 13, 12, 11, 10, 18, 16, 1

## Build
PlatformIO project — open the folder and run `pio run -t upload`.
