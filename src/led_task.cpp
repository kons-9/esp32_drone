#include "led_task.hpp"
#include "driver/gpio.h"

void
led_task::run() {
    while (true) {
        if (task_type == FAST) {
            fast_exec();
        } else {
            slow_exec();
        }
    }
}

void
init_led(gpio_num_t pin) {
    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}


void
led_task::fast_exec() {
    gpio_set_level(this.led_pin, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(led_pin, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
void
led_task::slow_exec() {
    gpio_set_level(led_pin, 1);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    gpio_set_level(led_pin, 0);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
}