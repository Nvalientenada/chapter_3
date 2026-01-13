#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
//#include "freetos/task.h"

#define LED_PIN  GPIO_NUM_11 // Choose your LED pin ( pin 10 doesnt work for my board)
#define BUTTON_PIN GPIO_NUM_4 // Choose your button pin

void app_main(void) {
    // TO-DO: Configure LED output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0); 
    

    // TO-DO: Configure Button input
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(BUTTON_PIN);
    gpio_pulldown_dis(BUTTON_PIN);

    int led_state = 0 ;
    int last_button = 1;

    while (1) {
        int current_b = gpio_get_level(BUTTON_PIN); // to keep track of the current button state

        // TO-DO: Implement LED toggle and button logic here
        if ( last_button == 1 && current_b == 0){
            led_state = !led_state;
            gpio_set_level(LED_PIN, led_state);
            // NEED TO DEBOUNCE 
            vTaskDelay( 200 / portTICK_PERIOD_MS); 
            
        }
        last_button = current_b ;
        vTaskDelay(10 / portTICK_PERIOD_MS);

    }
}