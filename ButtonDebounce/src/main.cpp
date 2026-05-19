#include <cstdio>
#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

constexpr gpio_num_t BUTTON_GPIO          = GPIO_NUM_37;
constexpr uint32_t   POLL_MS              = 10;
constexpr uint32_t   STABLE_SAMPLES       = 4;
constexpr uint8_t    BUTTON_LEVEL_PRESSED = 0;
constexpr uint32_t   TASK_STACK           = 4096;
constexpr uint32_t   TASK_PRIO            = 10;

static const char *TAG = "BTN";
static uint32_t    press_counter = 0;

enum class BtnState : uint8_t {
    RELEASED,
    PRESSED,
};

static void button_task(void *arg) {
    BtnState state  = BtnState::RELEASED;
    uint32_t stable = 0;

    while (true) {
        int level = gpio_get_level(BUTTON_GPIO);

        if (state == BtnState::RELEASED) {
            if (level == BUTTON_LEVEL_PRESSED) {
                if (++stable >= STABLE_SAMPLES) {
                    state  = BtnState::PRESSED;
                    stable = 0;
                    press_counter++;
                    ESP_LOGI(TAG, "press #%lu",
                             static_cast<unsigned long>(press_counter));
                }
            } else {
                stable = 0;
            }
        } else {
            if (level != BUTTON_LEVEL_PRESSED) {
                if (++stable >= STABLE_SAMPLES) {
                    state  = BtnState::RELEASED;
                    stable = 0;
                }
            } else {
                stable = 0;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(POLL_MS));
    }
}

extern "C" void app_main(void) {
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << BUTTON_GPIO,
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io));

    xTaskCreate(button_task, "btn_task", TASK_STACK, nullptr, TASK_PRIO, nullptr);

    ESP_LOGI(TAG, "Ready. GPIO%d, polling debounce (poll=%lums, stable=%lu samples = %lums)",
             BUTTON_GPIO,
             static_cast<unsigned long>(POLL_MS),
             static_cast<unsigned long>(STABLE_SAMPLES),
             static_cast<unsigned long>(POLL_MS * STABLE_SAMPLES));
}
