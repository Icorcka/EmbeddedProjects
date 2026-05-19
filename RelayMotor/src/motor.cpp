#include "motor.h"

#include "driver/ledc.h"
#include "config.h"

Motor::Motor(gpio_num_t pin) : pin_(pin) {}

void Motor::init() {
    ledc_timer_config_t timer_cfg = {
        .speed_mode      = Config::LEDC_MODE,
        .duty_resolution = Config::LEDC_RES,
        .timer_num       = Config::LEDC_TIMER,
        .freq_hz         = Config::LEDC_FREQ_HZ,
        .clk_cfg         = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer_cfg);

    ledc_channel_config_t ch_cfg = {
        .gpio_num   = pin_,
        .speed_mode = Config::LEDC_MODE,
        .channel    = Config::LEDC_CHANNEL,
        .timer_sel  = Config::LEDC_TIMER,
        .duty       = 0,
        .hpoint     = 0,
    };
    ledc_channel_config(&ch_cfg);
}

void Motor::set_duty(uint32_t duty) {
    ledc_set_duty(Config::LEDC_MODE, Config::LEDC_CHANNEL, duty);
    ledc_update_duty(Config::LEDC_MODE, Config::LEDC_CHANNEL);
}
