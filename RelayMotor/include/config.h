#pragma once

#include <cstdint>
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_adc/adc_oneshot.h"

namespace Config {
public:
    static constexpr gpio_num_t MOTOR_PIN = GPIO_NUM_42;

    static constexpr adc_unit_t POT_ADC_UNIT    = ADC_UNIT_2;
    static constexpr adc_channel_t POT_ADC_CHANNEL = ADC_CHANNEL_6;
    static constexpr adc_atten_t POT_ADC_ATTEN   = ADC_ATTEN_DB_12;

    static constexpr ledc_mode_t LEDC_MODE    = LEDC_LOW_SPEED_MODE;
    static constexpr ledc_timer_t LEDC_TIMER   = LEDC_TIMER_0;
    static constexpr ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_0;
    static constexpr ledc_timer_bit_t LEDC_RES     = LEDC_TIMER_12_BIT;
    static constexpr uint32_t LEDC_FREQ_HZ = 5000;

    static constexpr uint32_t UPDATE_PERIOD_MS = 50;
    static constexpr int32_t DEAD_ZONE = 300;

    static constexpr gpio_num_t RELAY_CTRL_PIN  = GPIO_NUM_7;
    static constexpr gpio_num_t RELAY_STATE_PIN = GPIO_NUM_12;
    static constexpr bool       RELAY_ACTIVE_LEVEL = true;

    static constexpr int      MEASUREMENT_COUNT = 10;
    static constexpr uint32_t SETTLE_DELAY_MS  = 200;
    static constexpr int64_t  MEASURE_TIMEOUT_US = 1'000'000;
};
