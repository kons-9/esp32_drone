#include "util.hpp"
#include "led_task.hpp"
#include "moter_task.hpp"
#include "webserver.hpp"
#include "event.hpp"

#include <cstring>

constexpr UBaseType_t MOTER_TASK_PRIORITY = 10;
constexpr UBaseType_t LED_TASK_PRIORITY = 5;
constexpr UBaseType_t COMMUNICATION_TASK_PRIORITY = 7;

constexpr gpio_num_t LED_GPIO = GPIO_NUM_2;
constexpr gpio_num_t LED_DEBUG_GPIO = GPIO_NUM_5;
constexpr gpio_num_t MOTER_GPIO = GPIO_NUM_4;

static led_task_manager_t led_manager = led_task_manager_t(LED_GPIO, LED_DEBUG_GPIO);
static moter_driver_t moter_driver = moter_driver_t(MOTER_GPIO);


void setup() {
    ESP_LOGI("app_main", "Initializing...");
    event_init();

    led_manager.init();
    xTaskCreate(led_receive_task, "led_receive_task", 2048, &led_manager, LED_TASK_PRIORITY, NULL);
    xTaskCreate(led_task, "led_blink", 8192, &led_manager, LED_TASK_PRIORITY, NULL);

    wifi_init();
    xTaskCreate(wifi_task, "wifi_info", 2048, NULL, COMMUNICATION_TASK_PRIORITY, NULL);

    ESP_LOGI("app_main", "Waiting for start operation...");
    // wait_start_task();
    ESP_LOGI("app_main", "start operation has received!");
    // delete_start_task();

    moter_driver.init();

    ESP_LOGI("app_main", "Starting tasks...");
    // function, name, stack size, args, priority, task handle
    xTaskCreate(moter_task, "moter_task", 2048, &moter_driver, MOTER_TASK_PRIORITY, NULL);
}

void loop() {
    vTaskMilliSecondDelay(10000);
}
