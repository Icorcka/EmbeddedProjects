# RelayMotor — Relay Response Time & PWM Motor Control

ESP32-S3 (ESP-IDF) project with two parts:

1. **Relay response timing** — measures how long a relay takes to mechanically switch its contact after the coil is energized.
2. **Soft PWM motor speed control** — drives a motor via LEDC PWM, with a potentiometer setting the duty cycle.

Both parts are in the same project; in `src/main.cpp` the motor loop runs by default and the relay block is kept commented out so you can swap whichever you want to flash. (The `Relay` class is fully implemented in `relay.cpp`.)

## Part 1 — Relay response time
- One GPIO drives the relay coil, another GPIO reads the contact state.
- The state pin uses a GPIO interrupt: the ISR captures the contact transition timestamp with `esp_timer_get_time()`, so timing isn't tied to the loop period.
- `Relay::measure_response_us()` toggles the coil, waits for the ISR flag, and returns the µs between command and contact change.
- The main loop runs `MEASUREMENT_COUNT = 10` cycles and prints each result plus the average to Serial.

## Part 2 — Soft PWM motor control
- Potentiometer read via the ADC oneshot driver (ADC2, channel 6, 12 dB attenuation).
- A dead zone (`DEAD_ZONE = 300`) clamps small readings to zero so the motor doesn't creep at rest.
- The raw value feeds `Motor::set_duty()`, which drives the motor pin through the LEDC peripheral at 5 kHz / 12-bit resolution.
- Update period: 50 ms — non-blocking, FreeRTOS-tick based.

## Pins
- Motor — GPIO 42 (LEDC channel 0)
- Potentiometer — ADC2 channel 6
- Relay control — GPIO 7
- Relay state (input from contact) — GPIO 12

## Build
PlatformIO + ESP-IDF framework — open the folder and run `pio run -t upload`.
