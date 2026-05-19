# Stm32AdcPot — STM32 Potentiometer ADC Calibration Comparison

Originally specified for ESP32 but implemented on an **STM32F411CE (BlackPill)** with the STM32Cube HAL — same task, different MCU. Reads a potentiometer wired as a voltage divider through the ADC, then prints a side-by-side table of the manually computed voltage vs. the VREFINT-calibrated voltage and the error between them.

## What it does
- Samples the potentiometer on `PA0` (ADC channel 0) every ~100 ms.
- For each sample, prints a row to UART (115200 baud):
  - `RAW` — 12-bit ADC value
  - `U_manual(mV)` — voltage computed from RAW assuming nominal `VDDA = 3300 mV` and full-scale `4095`
  - `U_cali(mV)` — voltage derived using the actual `VDDA` measured via the internal VREFINT channel (the "calibrated" reading)
  - `Error(%)` — `|U_manual − U_cali| / U_cali`, with two decimals of precision
- All-integer math, no `printf` floats, so it stays cheap on a Cortex-M4 without FPU dependency in the format path.

| Parameter | Value |
|---|---|
| Resolution | 12-bit |
| Reference | VDDA (nominal 3.300 V, VREFINT-calibrated at runtime) |
| Sampling | ~100 ms per reading |
| Input | PA0, potentiometer as voltage divider |

## Layout
- `Core/Inc/AdcReader.hpp`, `Core/Src/AdcReader.cpp` — ADC init + raw read + VDDA-from-VREFINT helper
- `Core/Inc/UartLogger.hpp`, `Core/Src/UartLogger.cpp` — minimal UART printf wrapper
- `Core/Src/app.cpp` — `App_Init` / `App_Run` (table header + sampling loop)
- `Base.ioc` — STM32CubeMX configuration

## Build
PlatformIO with the `ststm32` platform, board `blackpill_f411ce`, framework `stm32cube`. Upload via DFU:

```
pio run -t upload
pio device monitor -b 115200
```
