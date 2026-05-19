#pragma once
#include <stdint.h>

#define FILTER_SIZE 10

void filterInit();
void filterAdd(uint16_t value);
uint16_t filterAverage();
