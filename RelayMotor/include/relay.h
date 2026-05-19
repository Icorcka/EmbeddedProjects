#pragma once

#include <cstdint>
#include "driver/gpio.h"

class Relay {
public:
    Relay(gpio_num_t ctrl_pin, gpio_num_t state_pin, bool active_level);

    void init();
    void set(bool active);
    bool read_state() const;

    int64_t measure_response_us();

private:
    gpio_num_t ctrl_pin_;
    gpio_num_t state_pin_;
    bool       active_level_;

    static volatile int64_t contact_change_us_;
    static volatile bool    contact_event_;

    static void state_isr(void* arg);
};
