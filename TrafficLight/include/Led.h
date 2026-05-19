#pragma once

#include "driver/gpio.h"

class Led {
public:
    explicit Led(gpio_num_t pin);

    void on();
    void off();
    void set(bool on);
    void toggle();
    bool isOn() const;

private:
    void write(bool on);

    gpio_num_t pin_;
    bool state_;
};
