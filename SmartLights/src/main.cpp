#include <Arduino.h>
#include <stdint.h>
#include "adc.h"
#include "led.h"
#include "filter.h"
#include "calibration.h"

#define READ_INTERVAL (20)

void setup() {
  Serial.begin(115200);
  adcInit();
  calibrate();
  ledInit();
  filterInit();
}

uint32_t lastReadTime = 0;

void loop() {
  if (millis() - lastReadTime >= READ_INTERVAL) {
    lastReadTime = millis();

    uint16_t raw = readRaw();
    if(!isSensorValid(raw)) return;
    filterAdd(raw);
    uint16_t filtered = filterAverage();
    uint16_t uCalc = calcVoltage(filtered);
    uint16_t analogMV = readMilliVolts();

    printReadings(filtered, uCalc, analogMV);
    uint8_t brightness = normilizeBrightness(filtered);
    fadeLed(brightness);
  }
}
