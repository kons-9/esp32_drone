#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_task.hpp"

#define MOTER_TASK_PRIORITY 10
#define LED_TASK_PRIORITY 5
#define COMMUNICATION_TASK_PRIORITY 7

// communication_task()のみが書き込むので、
// 特に排他制御はしない
led_task_t led_task_type = FAST;
uint8_t motor_speed = 0;

void led_task(const led_task_manager_t& manager) {
    while (true) {
        switch (led_task_type) {
            FAST:
                manager.fast_exec();
                break;
            SLOW:
                manager.slow_exec();
                break;
        }
    }
}

void moter_task(const moter_driver& driver) {
    while (true) {
    }
}

void communication_task() {
    while (true) {
        char command[128];
        scanf("%s", command);
        printf("Command: %s\n", command);
    }
}

extern "C" void app_main() {
    printf("Hello world!\n");
    
    led_task_manager_t led_manager(GPIO_NUM_2);
    led_manager.init();
    moter_driver_t moter_driver(GPIO_NUM_4);
    moter_driver.init();


    // function, name, stack size, args, priority, task handle
    xTaskCreate(&moter_task, "moter_task", 2048, moter_driver, MOTER_TASK_PRIORITY, NULL);
    xTaskCreate(&communication_task, "ble_task", 2048, NULL, COMMUNICATION_TASK_PRIORITY, NULL);
    xTaskCreate(&led_task, "led_blink", 2048, led_manager, LED_TASK_PRIORITY, NULL);
}