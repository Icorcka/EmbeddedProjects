#include "led.h"

#include <cstdint>

Led::Led(gpio_num_t pin) : pin_(pin) {}

void Led::init() {
    gpio_reset_pin(pin_);
    gpio_set_direction(pin_, GPIO_MODE_OUTPUT);
}

void Led::set(State state) {
    gpio_set_level(pin_, static_cast<uint32_t>(state));
}
