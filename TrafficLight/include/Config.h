#pragma once

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include <cstdint>

namespace cfg {

constexpr gpio_num_t PIN_GREEN  = GPIO_NUM_6;
constexpr gpio_num_t PIN_YELLOW = GPIO_NUM_17;
constexpr gpio_num_t PIN_RED    = GPIO_NUM_8;
constexpr gpio_num_t PIN_BUTTON = GPIO_NUM_45;

constexpr uint32_t T_GREEN_MS       = 5000;
constexpr uint32_t T_GREEN_BLINK_MS = 3000;
constexpr uint32_t T_YELLOW_MS      = 2000;
constexpr uint32_t T_RED_MS         = 5000;
constexpr uint32_t T_RED_YELLOW_MS  = 1000;

constexpr uint32_t GREEN_BLINK_HALF_MS = 250;
constexpr uint32_t NIGHT_BLINK_HALF_MS = 500;

constexpr uint32_t DEBOUNCE_MS  = 40;
constexpr uint32_t LOOP_TICK_MS = 10;

constexpr UBaseType_t TASK_PRIO_TL  = 5;
constexpr UBaseType_t TASK_PRIO_BTN = 6;
constexpr uint32_t    TASK_STACK    = 4096;

}
