#pragma once
#include <Arduino.h>

#define LED_PIN 46
#define LDR_THRESHOLD 512

void ledInit();
void updateLed(int ldrValue);
