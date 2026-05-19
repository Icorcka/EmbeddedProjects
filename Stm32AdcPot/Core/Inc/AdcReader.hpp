#pragma once

#include "main.h"
#include <cstdint>

class AdcReader {
public:
    static constexpr uint32_t VDDA_CAL_MV = 3300U;
    static constexpr uint32_t FULL_SCALE  = 4095U;

    bool begin();

    uint32_t readRaw(uint32_t channel);

    uint32_t getVDDA_mV();

private:
    ADC_HandleTypeDef hadc_{};
};
