#include "util.hpp"
#include "led_task.hpp"
#include "moter_task.hpp"
#include "webserver.hpp"
#include "webserver.hpp"

#include <cstring>

constexpr UBaseType_t MOTER_TASK_PRIORITY = 10;
constexpr UBaseType_t LED_TASK_PRIORITY = 5;
constexpr UBaseType_t COMMUNICATION_TASK_PRIORITY = 7;

constexpr gpio_num_t LED_GPIO = GPIO_NUM_2;
constexpr gpio_num_t LED_DEBUG_GPIO = GPIO_NUM_3;
constexpr gpio_num_t MOTER_GPIO = GPIO_NUM_4;

void setup() {
    ESP_LOGI("app_main", "Initializing...");

    led_task_manager_t *led_manager = new led_task_manager_t(LED_GPIO);
    moter_driver_t *moter_driver = new moter_driver_t(MOTER_GPIO);

    moter_driver->init();
    led_manager->init();
    wifi_init();
    webserver_init();

    vTaskMilliSecondDelay(1000);

    ESP_LOGI("app_main", "Starting tasks...");
    // function, name, stack size, args, priority, task handle
    xTaskCreate(moter_task, "moter_task", 2048, moter_driver, MOTER_TASK_PRIORITY, NULL);
    xTaskCreate(led_receive_task, "led_receive_task", 2048, led_manager, LED_TASK_PRIORITY, NULL);
    xTaskCreate(led_task, "led_blink", 8192, led_manager, LED_TASK_PRIORITY, NULL);
    xTaskCreate(webserver_run, "webserver", 2048, NULL, COMMUNICATION_TASK_PRIORITY, NULL);
}

void loop() {
    vTaskMilliSecondDelay(10000);
}
