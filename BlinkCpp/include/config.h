#pragma once

#include <cstdint>
#include "driver/gpio.h"

namespace Config {
    inline constexpr gpio_num_t LED_PIN    = GPIO_NUM_11;
    inline constexpr gpio_num_t BUTTON_PIN = GPIO_NUM_7;

    inline constexpr uint32_t POLL_PERIOD_MS  = 10;
    inline constexpr uint32_t DEBOUNCE_MS     = 50;
    inline constexpr uint32_t BLINK_PERIOD_MS = 500;

    inline constexpr uint8_t SHORT_PRESS_BLINK_COUNT = 3;
}
