#include "util.hpp"
#include "communication.hpp"
#include "led_task.hpp"
#include "moter_task.hpp"

#define MOTER_TASK_PRIORITY 10
#define LED_TASK_PRIORITY 5
#define COMMUNICATION_TASK_PRIORITY 7

extern "C" void app_main()
{
    printf("Hello world!\n");

    led_task_manager_t led_manager(GPIO_NUM_2);
    led_manager.init();
    moter_driver_t moter_driver(GPIO_NUM_4);

    // function, name, stack size, args, priority, task handle
    xTaskCreate(moter_task, "moter_task", 2048, &moter_driver, MOTER_TASK_PRIORITY, NULL);
    xTaskCreate(communication_task, "ble_task", 2048, NULL, COMMUNICATION_TASK_PRIORITY, NULL);
    xTaskCreate(led_task, "led_blink", 2048, &led_manager, LED_TASK_PRIORITY, NULL);
}