#include "relay.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "esp_timer.h"
#include "config.h"

volatile int64_t Relay::contact_change_us_ = 0;
volatile bool    Relay::contact_event_     = false;

void IRAM_ATTR Relay::state_isr(void* /*arg*/) {
    contact_change_us_ = esp_timer_get_time();
    contact_event_ = true;
}

Relay::Relay(gpio_num_t ctrl_pin, gpio_num_t state_pin, bool active_level)
    : ctrl_pin_(ctrl_pin), state_pin_(state_pin), active_level_(active_level) {}

void Relay::init() {
    gpio_reset_pin(ctrl_pin_);
    gpio_set_direction(ctrl_pin_, GPIO_MODE_OUTPUT);
    set(false);

    gpio_reset_pin(state_pin_);
    gpio_set_direction(state_pin_, GPIO_MODE_INPUT);
    gpio_set_pull_mode(state_pin_, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(state_pin_, GPIO_INTR_ANYEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(state_pin_, state_isr, nullptr);
}

void Relay::set(bool active) {
    gpio_set_level(ctrl_pin_, active ? active_level_ : !active_level_);
}

bool Relay::read_state() const {
    return gpio_get_level(state_pin_) == 0;
}

int64_t Relay::measure_response_us() {
    contact_event_ = false;
    int64_t start_us = esp_timer_get_time();
    set(true);

    while (!contact_event_) {
        if (esp_timer_get_time() - start_us > Config::MEASURE_TIMEOUT_US) {
            set(false);
            return -1;
        }
        vTaskDelay(1);
    }
    int64_t delay_us = contact_change_us_ - start_us;

    vTaskDelay(pdMS_TO_TICKS(Config::SETTLE_DELAY_MS));
    set(false);
    vTaskDelay(pdMS_TO_TICKS(Config::SETTLE_DELAY_MS));

    return delay_us;
}
