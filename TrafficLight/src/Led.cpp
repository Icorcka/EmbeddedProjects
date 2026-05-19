#include "Led.h"

Led::Led(gpio_num_t pin) : pin_(pin), state_(false) {
    gpio_reset_pin(pin_);
    gpio_set_direction(pin_, GPIO_MODE_OUTPUT);
    gpio_set_level(pin_, 0);
}

void Led::on()  { write(true); }
void Led::off() { write(false); }
void Led::set(bool on) { write(on); }
void Led::toggle() { write(!state_); }
bool Led::isOn() const { return state_; }

void Led::write(bool on) {
    state_ = on;
    gpio_set_level(pin_, on ? 1 : 0);
}
