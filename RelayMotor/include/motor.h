#pragma once

#include <cstdint>
#include "driver/gpio.h"

class Motor {
public:
    explicit Motor(gpio_num_t pin);

    void init();
    void set_duty(uint32_t duty);

private:
    gpio_num_t pin_;
};
