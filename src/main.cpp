#include "util.hpp"
// #include "driver/uart.h"
// #include "communication.hpp"
#include "led_task.hpp"
#include "moter_task.hpp"
#include "webserver.hpp"

#include <cstring>

#define MOTER_TASK_PRIORITY 10
#define LED_TASK_PRIORITY 5
#define COMMUNICATION_TASK_PRIORITY 7

#define LED_GPIO GPIO_NUM_2
#define MOTER_GPIO GPIO_NUM_4


void init(moter_driver_t &moter_driver, led_task_manager_t &led_manager) {

    while (1) {
        printf("Waiting for second...\n");
        vTaskMilliSecondDelay(1000);

        // communication_init();
        led_manager.init();
        moter_driver.init();
        
        return;
    }
}

// extern "C" void app_main() {
void setup() {

    led_task_manager_t led_manager(LED_GPIO);
    moter_driver_t moter_driver(MOTER_GPIO);
    init(moter_driver, led_manager);

    ESP_LOGI("app_main", "Starting tasks...");

    // function, name, stack size, args, priority, task handle
    xTaskCreate(moter_task, "moter_task", 2048, &moter_driver, MOTER_TASK_PRIORITY, NULL);
    // xTaskCreate(communication_task, "ble_task", 2048, NULL, COMMUNICATION_TASK_PRIORITY, NULL);
    xTaskCreate(led_task, "led_blink", 2048, &led_manager, LED_TASK_PRIORITY, NULL);
}

void loop() {
    vTaskMilliSecondDelay(100000);
}