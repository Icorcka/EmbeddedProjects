# Lb1 — ESP32 Police Lights

ESP32 program that drives red and blue LEDs to imitate a police flasher, with extras: a third (green) LED, multiple blink patterns, and a button to cycle through modes at different speeds.

## What it does
- Uses three GPIO pins to drive red, blue and green LEDs through resistors.
- Alternates the red and blue LEDs in a looping police-flasher pattern.
- A button on GPIO 39 cycles through additional modes (traffic lights, dual, all-on, fast/slow variants).

## Pins
- Red LED — GPIO 3
- Blue LED — GPIO 18
- Green LED — GPIO 6
- Button — GPIO 39

## Build
PlatformIO project — open the folder and run `pio run -t upload`.
