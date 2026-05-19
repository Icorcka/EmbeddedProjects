#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

typedef struct {
    gpio_num_t pin;
    uint32_t period_ms;
    uint32_t last_ms;
    bool state;
} led_t;

static led_t leds[] = {
    { .pin = GPIO_NUM_42, .period_ms = 200  },
    { .pin = GPIO_NUM_47, .period_ms = 500  },
    { .pin = GPIO_NUM_37, .period_ms = 1000 },
};

static const size_t LED_COUNT = sizeof(leds) / sizeof(leds[0]);

static inline uint32_t millis(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

static void configure_led(gpio_num_t pin)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

static void update_led(led_t *led, uint32_t now)
{
    if (now - led->last_ms >= led->period_ms) {
        led->last_ms = now;
        led->state = !led->state;
        gpio_set_level(led->pin, led->state);
    }
}

void app_main(void)
{
    for (size_t i = 0; i < LED_COUNT; i++) {
        configure_led(leds[i].pin);
    }

    while (true) {
        uint32_t now = millis();

        for (size_t i = 0; i < LED_COUNT; i++) {
            update_led(&leds[i], now);
        }

        vTaskDelay(1);
    }
}
