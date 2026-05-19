#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <iterator>

const uint8_t LED_PINS[] = {46, 13, 12, 11, 10, 18, 16, 1};
const uint8_t NUM_LEDS = std::size(LED_PINS);

void ledInit();
void fadeLed(uint8_t targetBrightness);

uint8_t normilizeBrightness(uint16_t ldrValue);
