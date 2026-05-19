#pragma once
#include <Arduino.h>
#include <stdint.h>

#define CALIBRATION_TIME 5000

void calibrate();
uint16_t getCalibratedMin();
uint16_t getCalibratedMax();
