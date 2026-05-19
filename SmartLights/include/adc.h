#pragma once
#include <Arduino.h>
#include <stdint.h>

#define LDR_PIN 17
#define ADC_RESOLUTION_BITS 12
#define ADC_MAX 4095
#define U_REF 3300
#define SENSOR_SHORT_THRESHOLD 5
#define SENSOR_DISCONNECT_THRESHOLD 4090

bool isSensorValid(uint16_t raw);
void adcInit();
uint16_t readRaw();
uint16_t calcVoltage(uint16_t raw);
uint16_t readMilliVolts();
void printReadings(uint16_t raw, uint16_t uCalc, uint16_t analogMV);
