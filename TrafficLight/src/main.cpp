#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

#include <climits>

#include "Config.h"
#include "TrafficLight.h"

static const char* TAG = "trafficlight";

static TaskHandle_t s_tl_task  = nullptr;
static TaskHandle_t s_btn_task = nullptr;

static constexpr uint32_t NOTIFY_BTN_EDGE     = 1u << 0;
static constexpr uint32_t NOTIFY_TOGGLE_NIGHT = 1u << 0;

static void IRAM_ATTR button_isr_handler(void*) {
    BaseType_t hp = pdFALSE;
    xTaskNotifyFromISR(s_btn_task, NOTIFY_BTN_EDGE, eSetBits, &hp);
    portYIELD_FROM_ISR(hp);
}

static void traffic_light_task(void*) {
    TrafficLight tl;
    tl.start();

    while (true) {
        uint32_t cmd = 0;
        if (xTaskNotifyWait(0, ULONG_MAX, &cmd, 0) == pdTRUE) {
            if (cmd & NOTIFY_TOGGLE_NIGHT) {
                tl.toggleNight();
                ESP_LOGI(TAG, "night mode = %d", tl.nightMode());
            }
        }
        tl.tick(esp_timer_get_time());
        vTaskDelay(pdMS_TO_TICKS(cfg::LOOP_TICK_MS));
    }
}

static void button_task(void*) {
    while (true) {
        uint32_t bits = 0;
        if (xTaskNotifyWait(0, ULONG_MAX, &bits, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        vTaskDelay(pdMS_TO_TICKS(cfg::DEBOUNCE_MS));
        if (gpio_get_level(cfg::PIN_BUTTON) != 0) {
            continue;
        }

        xTaskNotify(s_tl_task, NOTIFY_TOGGLE_NIGHT, eSetBits);

        while (gpio_get_level(cfg::PIN_BUTTON) == 0) {
            vTaskDelay(pdMS_TO_TICKS(cfg::DEBOUNCE_MS));
        }
        vTaskDelay(pdMS_TO_TICKS(cfg::DEBOUNCE_MS));

        xTaskNotifyWait(ULONG_MAX, 0, nullptr, 0);
    }
}

static void setup_button() {
    gpio_config_t io = {};
    io.pin_bit_mask = 1ULL << cfg::PIN_BUTTON;
    io.mode         = GPIO_MODE_INPUT;
    io.pull_up_en   = GPIO_PULLUP_ENABLE;
    io.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io.intr_type    = GPIO_INTR_NEGEDGE;
    gpio_config(&io);
}

extern "C" void app_main(void) {
    setup_button();

    xTaskCreatePinnedToCore(button_task, "btn",
                            cfg::TASK_STACK, nullptr,
                            cfg::TASK_PRIO_BTN, &s_btn_task, tskNO_AFFINITY);

    xTaskCreatePinnedToCore(traffic_light_task, "tl",
                            cfg::TASK_STACK, nullptr,
                            cfg::TASK_PRIO_TL, &s_tl_task, tskNO_AFFINITY);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(cfg::PIN_BUTTON, button_isr_handler, nullptr);

    ESP_LOGI(TAG, "started: G=%d Y=%d R=%d BTN=%d",
             cfg::PIN_GREEN, cfg::PIN_YELLOW, cfg::PIN_RED, cfg::PIN_BUTTON);
}
