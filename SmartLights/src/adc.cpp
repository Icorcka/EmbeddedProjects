#include "adc.h"

void adcInit() {
  pinMode(LDR_PIN, INPUT);
  analogReadResolution(ADC_RESOLUTION_BITS);
  analogSetPinAttenuation(LDR_PIN, ADC_11db);
}

uint16_t readRaw() {
  return (uint16_t)analogRead(LDR_PIN);
}

uint16_t calcVoltage(uint16_t raw) {
  return (uint16_t)((uint32_t)raw * U_REF / ADC_MAX);
}

uint16_t readMilliVolts() {
  return (uint16_t)analogReadMilliVolts(LDR_PIN);
}

bool isSensorValid(uint16_t raw){
  if(raw <= SENSOR_SHORT_THRESHOLD) {
    Serial.println("WARNING: Sensor short circuit!");
    return false;
  } else if(raw >= SENSOR_DISCONNECT_THRESHOLD) {
    Serial.println("WARNING: Sensor Sensor disconnected!");
    return false;
  } else {
    return true;
  }
}

void printReadings(uint16_t raw, uint16_t uCalc, uint16_t analogMV) {
  Serial.print("RAW: "); Serial.print(raw);
  Serial.print("\tUcalc: "); Serial.print(uCalc);
  Serial.print(" mV\tAnalogMV: "); Serial.print(analogMV);
  Serial.print(" mV\tDifference: ");
  Serial.println((int32_t)analogMV - (int32_t)uCalc);
}
