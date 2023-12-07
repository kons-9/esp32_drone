#include "util.hpp"
#include "led_task.hpp"
#include <freertos/queue.h>

static const char *TAG = "led_task_manager_t";

// from serial or communication(todo) or sensor?
#define LED_QUEUE_SIZE 10
QueueHandle_t led_queue;

void led_task(void *arg)
{
    led_task_manager_t *manager = (led_task_manager_t *)arg;
    while (true)
    {
        manager->run();
    }
}

void led_task_manager_t::run()
{
    led_task_t cur_task_type;
    if (xQueueReceive(led_queue, &cur_task_type, 10) == pdTRUE)
    {
        switch (cur_task_type)
        {
        case FAST:
            ESP_LOGI(TAG, "FAST");
            fast_exec();
            break;
        case SLOW:
            ESP_LOGI(TAG, "SLOW");
            slow_exec();
            break;
        default:
            ESP_LOGE(TAG, "Unknown task type");
            break;
        }
        led_task_type = cur_task_type;
    }
}

void led_task_manager_t::init()
{
    led_queue = xQueueCreate(LED_QUEUE_SIZE, sizeof(led_task_t));
    if (led_queue == NULL)
    {
        ESP_LOGE(TAG, "Failed to create queue");
        exit(1);
    }
    led_task_type = FAST;
}

void inline led_task_manager_t::fast_exec()
{
    gpio_set_level(led_pin, 1);
    vTaskMilliSecondDelay(1000);
    gpio_set_level(led_pin, 0);
    vTaskMilliSecondDelay(1000);
}
void inline led_task_manager_t::slow_exec()
{
    gpio_set_level(led_pin, 1);
    vTaskMilliSecondDelay(2000);
    gpio_set_level(led_pin, 0);
    vTaskMilliSecondDelay(2000);
}