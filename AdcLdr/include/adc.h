#pragma once
#include <Arduino.h>

#define LDR_PIN 17
#define ADC_MAX 4095.0
#define U_REF 3300.0

void adcInit();
void updateResolution();
void updateAttenuation();
int readRaw();
float calcVoltage(int raw);
int readMilliVolts();
void printReadings(int raw, float uCalc, int analogMV);
