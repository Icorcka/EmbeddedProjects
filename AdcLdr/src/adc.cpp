#include "adc.h"

static int resolution = 12;
static unsigned long lastChange = 0;
static int attenIndex = 0;
static unsigned long lastAttenChange = 0;
static adc_attenuation_t attens[] = {ADC_0db, ADC_2_5db, ADC_6db, ADC_11db};
static const char* attenNames[] = {"0dB", "2.5dB", "6dB", "11dB"};

void adcInit() {
  pinMode(LDR_PIN, INPUT);
  analogReadResolution(12);
}

void updateResolution() {
  if (millis() - lastChange >= 5000) {
    lastChange = millis();
    resolution++;
    if (resolution > 12) resolution = 9;
    analogReadResolution(resolution);
    Serial.print("--- Resolution: ");
    Serial.print(resolution);
    Serial.println(" bit ---");
  }
}

void updateAttenuation() {
  if (millis() - lastAttenChange >= 7000) {
    lastAttenChange = millis();
    attenIndex = (attenIndex + 1) % 4;
    analogSetPinAttenuation(LDR_PIN, attens[attenIndex]);
    Serial.print("--- Attenuation: ");
    Serial.print(attenNames[attenIndex]);
    Serial.println(" ---");
  }
}

int readRaw() {
  return analogRead(LDR_PIN);
}

float calcVoltage(int raw) {
  return (raw / ADC_MAX) * U_REF;
}

int readMilliVolts() {
  return analogReadMilliVolts(LDR_PIN);
}

void printReadings(int raw, float uCalc, int analogMV) {
  Serial.print("RAW: "); Serial.print(raw);
  Serial.print("\tUcalc: "); Serial.print(uCalc, 1);
  Serial.print(" mV\tAnalogMV: "); Serial.print(analogMV);
  Serial.print(" mV\tDifference: ");
  Serial.println(analogMV - uCalc);
}
