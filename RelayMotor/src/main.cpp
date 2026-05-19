#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "config.h"
#include "motor.h"
#include "relay.h"

static adc_oneshot_unit_handle_t adc_handle;

static void pot_init() {
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = Config::POT_ADC_UNIT,
    };
    adc_oneshot_new_unit(&init_cfg, &adc_handle);

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten    = Config::POT_ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(adc_handle, Config::POT_ADC_CHANNEL, &chan_cfg);
}

static int pot_read() {
    int raw;
    adc_oneshot_read(adc_handle, Config::POT_ADC_CHANNEL, &raw);
    return raw;
}

extern "C" void app_main() {
    // --- Motor + potentiometer (HW2.2 part 1) ---
    Motor motor(Config::MOTOR_PIN);
    motor.init();
    pot_init();

    while (true) {
        int raw = pot_read();
        if (raw < Config::DEAD_ZONE) {
            raw = 0;
        }
        motor.set_duty(raw);
        vTaskDelay(pdMS_TO_TICKS(Config::UPDATE_PERIOD_MS));
    }

    // --- Relay response measurement (HW2.2 part 2) ---
    // Relay relay(Config::RELAY_CTRL_PIN, Config::RELAY_STATE_PIN, Config::RELAY_ACTIVE_LEVEL);
    // relay.init();
    // vTaskDelay(pdMS_TO_TICKS(500));
    //
    // printf("\n=== Starting %d relay response measurements ===\n", Config::MEASUREMENT_COUNT);
    //
    // int64_t total_us = 0;
    // int     valid_count = 0;
    // for (int i = 1; i <= Config::MEASUREMENT_COUNT; i++) {
    //     int64_t t = relay.measure_response_us();
    //     if (t < 0) {
    //         printf("Measurement %2d: TIMEOUT (no contact change detected)\n", i);
    //         continue;
    //     }
    //     total_us += t;
    //     valid_count++;
    //     printf("Measurement %2d: %lld us (%lld ms)\n",
    //            i, (long long)t, (long long)(t / 1000));
    // }
    //
    // if (valid_count > 0) {
    //     int64_t avg_us = total_us / valid_count;
    //     printf("\n=== Average over %d valid measurements: %lld us (%lld ms) ===\n",
    //            valid_count, (long long)avg_us, (long long)(avg_us / 1000));
    // } else {
    //     printf("\n=== No valid measurements — check wiring ===\n");
    // }
    //
    // while (true) {
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
}
