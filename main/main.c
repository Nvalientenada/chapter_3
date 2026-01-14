/* Nada Aloussi*/
/* 13/1/26*/
/* LED turns on only if Button 2 is held down and we press Button 1*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN      GPIO_NUM_11
#define BUTTON_PIN   GPIO_NUM_4
#define BUTTON_PIN2  GPIO_NUM_5

void app_main(void) {
    // LED output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    // Button 1 input (pull-up)  => pressed = 0
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);
    gpio_pulldown_dis(BUTTON_PIN);

    // Button 2 input (pull-up)  => pressed = 0
    gpio_reset_pin(BUTTON_PIN2);
    gpio_set_direction(BUTTON_PIN2, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN2);
    gpio_pulldown_dis(BUTTON_PIN2);

    int last_b1 = 1;
    int press_count = 0;

    // for debug printing without slowing the loop too much
    int debug_counter = 0;

    while (1) {
        int b1 = gpio_get_level(BUTTON_PIN);
        int b2 = gpio_get_level(BUTTON_PIN2);

        // Print about 2x per second (without blocking)
        debug_counter++;
        if (debug_counter >= 25) {          // 25 * 20ms = 500ms
            printf("B1=%d  B2=%d  count=%d\n", b1, b2, press_count);
            debug_counter = 0;
        }

        // Detect Button1 press edge (released -> pressed)
        if (last_b1 == 1 && b1 == 0) {

            // Only count presses if Button2 is held down
            if (b2 == 0) {
                press_count++;

                // ON on 1st, 3rd, 5th... press
                gpio_set_level(LED_PIN, (press_count % 2 == 1) ? 1 : 0);
            }

            // Debounce: wait until bounce is over
            vTaskDelay(200 / portTICK_PERIOD_MS);

            // Optional: wait for release so holding Button1 doesn't re-trigger
            while (gpio_get_level(BUTTON_PIN) == 0) {
                vTaskDelay(10 / portTICK_PERIOD_MS);
            }
        }

        last_b1 = b1;
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
