#include <cstdint>
#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_attr.h"
#include "config.h"
#include "led.h"

enum led_mode_t {
    MODE_BLINK,
    MODE_ON,
    MODE_OFF,
    MODE_COUNT
};

static volatile bool buttonPressed = false;

struct BlinkState {
    uint32_t last_blink_ms = 0;
    Led::State level = Led::State::OFF;
};

struct IterTimer {
    uint32_t count = 0;
    int64_t batch_start_us = 0;
};

static void IRAM_ATTR button_isr(void*) {
    static int64_t last_press_us = 0;
    int64_t now_us = esp_timer_get_time();
    if (now_us - last_press_us < static_cast<int64_t>(Config::DEBOUNCE_MS) * 1000) {
        return;
    }
    last_press_us = now_us;
    buttonPressed = true;
}

static void button_init() {
    gpio_reset_pin(Config::BUTTON_PIN);
    gpio_set_direction(Config::BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(Config::BUTTON_PIN, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(Config::BUTTON_PIN, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(Config::BUTTON_PIN, button_isr, nullptr);
}

static void led_apply_mode(Led& led, led_mode_t mode, uint32_t now_ms, BlinkState& blink) {
    switch (mode) {
        case MODE_BLINK:
            if ((now_ms - blink.last_blink_ms) >= Config::BLINK_PERIOD_MS) {
                blink.level = (blink.level == Led::State::ON) ? Led::State::OFF : Led::State::ON;
                led.set(blink.level);
                blink.last_blink_ms = now_ms;
            }
            break;
        case MODE_ON:
            led.set(Led::State::ON);
            break;
        case MODE_OFF:
            led.set(Led::State::OFF);
            break;
        default:
            break;
    }
}

static void iter_timer_tick(IterTimer& timer) {
    timer.count++;
    if (timer.count < 1000) {
        return;
    }

    int64_t batch_end_us = esp_timer_get_time();
    int64_t elapsed_us = batch_end_us - timer.batch_start_us;
    int64_t avg_int  = elapsed_us / 1000;
    int64_t avg_frac = elapsed_us % 1000;
    printf("1000 iter: %lld us total, avg %lld.%03lld us/iter\n",
           static_cast<long long>(elapsed_us),
           static_cast<long long>(avg_int),
           static_cast<long long>(avg_frac));

    timer.count = 0;
    vTaskDelay(1);
    timer.batch_start_us = esp_timer_get_time();
}

extern "C" void app_main() {
    Led led(Config::LED_PIN);
    led.init();
    button_init();

    led_mode_t mode = MODE_BLINK;
    BlinkState blink{};
    IterTimer timer{};
    timer.batch_start_us = esp_timer_get_time();

    while (true) {
        uint32_t now_ms = pdTICKS_TO_MS(xTaskGetTickCount());

        if (buttonPressed) {
            buttonPressed = false;
            mode = static_cast<led_mode_t>((mode + 1) % MODE_COUNT);
        }
        led_apply_mode(led, mode, now_ms, blink);
        iter_timer_tick(timer);
    }
}
