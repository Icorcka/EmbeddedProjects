#pragma once

#include "driver/gpio.h"

class Led {
public:
    enum class State : bool {
        OFF = false,
        ON  = true
    };

    explicit Led(gpio_num_t pin);

    void init();
    void set(State state);

private:
    gpio_num_t pin_;
};
