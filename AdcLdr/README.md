# AdcLdr — ESP32-S3 LDR ADC Reader

ESP32-S3 program that reads a light-dependent resistor (LDR) connected as a voltage divider, samples the ADC every 100 ms and compares two ways of measuring the voltage.

## What it does
- Reads the LDR every 100 ms.
- Captures three values each cycle:
  - RAW ADC value via `analogRead()`
  - Voltage computed from RAW using `U = raw / ADC_MAX * U_REF`
  - Voltage measured directly via `analogReadMillivolts()`
- Prints all three to Serial (115200 baud) so the calculated vs. measured voltages and their error can be compared.
- Sweeps ADC resolution (`analogReadResolution()`, 9–12 bit) and pin attenuation (`analogSetPinAttenuation()`, 0 / 2.5 / 6 / 11 dB) so you can observe their effect on range, stability and error.
- Drives an LED that turns on when the LDR reading crosses a threshold (`LDR_THRESHOLD = 512`).

## Pins
- LDR — GPIO 17 (ADC input, in a voltage divider to GND)
- LED — GPIO 46

## Build
PlatformIO project — open the folder and run `pio run -t upload`.
