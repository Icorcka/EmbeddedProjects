#include <cstdio>
#include <cstdint>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

constexpr gpio_num_t MOTOR_GPIO      = GPIO_NUM_42;
constexpr gpio_num_t LED_GPIO        = GPIO_NUM_46;
constexpr uint64_t   ON_DURATION_US  = 60ULL  * 1000 * 1000;
constexpr uint64_t   OFF_DURATION_US = 300ULL * 1000 * 1000;
constexpr uint64_t   CYCLE_PERIOD_US = ON_DURATION_US + OFF_DURATION_US;

static const char        *TAG         = "FAN";
static esp_timer_handle_t off_timer   = nullptr;
static bool               fan_running = false;

static void fan_off_cb(void *arg) {
    if (!fan_running) {
        return;
    }
    gpio_set_level(MOTOR_GPIO, 0);
    gpio_set_level(LED_GPIO, 0);
    fan_running = false;
    ESP_LOGI(TAG, "OFF");
}

static void fan_on_cb(void *arg) {
    if (fan_running) {
        ESP_LOGW(TAG, "already running, skip start");
        return;
    }
    gpio_set_level(MOTOR_GPIO, 1);
    gpio_set_level(LED_GPIO, 1);
    fan_running = true;
    ESP_LOGI(TAG, "ON for %llus", ON_DURATION_US / 1000000ULL);
    esp_timer_start_once(off_timer, ON_DURATION_US);
}

extern "C" void app_main(void) {
    gpio_config_t io = {
        .pin_bit_mask = (1ULL << MOTOR_GPIO) | (1ULL << LED_GPIO),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io));
    gpio_set_level(MOTOR_GPIO, 0);
    gpio_set_level(LED_GPIO, 0);

    const esp_timer_create_args_t off_args = {
        .callback = &fan_off_cb,
        .name     = "fan_off",
    };
    ESP_ERROR_CHECK(esp_timer_create(&off_args, &off_timer));

    esp_timer_handle_t on_timer = nullptr;
    const esp_timer_create_args_t on_args = {
        .callback = &fan_on_cb,
        .name     = "fan_on",
    };
    ESP_ERROR_CHECK(esp_timer_create(&on_args, &on_timer));

    fan_on_cb(nullptr);
    ESP_ERROR_CHECK(esp_timer_start_periodic(on_timer, CYCLE_PERIOD_US));

    ESP_LOGI(TAG, "armed: ON=%llus, OFF=%llus, period=%llus",
             ON_DURATION_US  / 1000000ULL,
             OFF_DURATION_US / 1000000ULL,
             CYCLE_PERIOD_US / 1000000ULL);
}
