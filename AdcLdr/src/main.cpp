#include <Arduino.h>
#include "adc.h"
#include "led.h"

void setup() {
  Serial.begin(115200);
  adcInit();
  ledInit();
}

void loop() {
  updateResolution();
  updateAttenuation();

  int raw = readRaw();
  float uCalc = calcVoltage(raw);
  int analogMV = readMilliVolts();

  printReadings(raw, uCalc, analogMV);
  updateLed(raw);

  delay(100);
}
