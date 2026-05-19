#include "filter.h"

static uint16_t buffer[FILTER_SIZE];
static uint8_t index = 0;
static uint8_t count = 0;

void filterInit() {
    for (auto& sample : buffer) {
        sample = 0;
    }
    index = 0;
    count = 0;
}

void filterAdd(uint16_t value) {
    buffer[index] = value;
    index = (index + 1) % FILTER_SIZE;

    if (count < FILTER_SIZE) {
        count++;
    }
}

uint16_t filterAverage() {
    if (count == 0) {
        return 0;
    }

    uint32_t sum = 0;
    for (uint8_t i = 0; i < count; i++) {
        sum += buffer[i];
    }
    return (uint16_t)(sum / count);
}
