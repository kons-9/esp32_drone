#include "util.hpp"
#include "led_task.hpp"

static const char *TAG = "led_task";
QueueHandle_t g_led_queue = NULL;

void led_task(void *arg)
{
    led_task_manager_t *manager = (led_task_manager_t *)arg;
    manager->run();
}

void led_task_manager_t::run()
{
    led_task_t cur_task_type;
    while (true)
    {
        if (xQueueReceive(g_led_queue, &cur_task_type, 0) == pdTRUE)
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
}

// コンストラクタ内部でやってもいいかも
void led_task_manager_t::init()
{
    g_led_queue = xQueueCreate(LED_QUEUE_SIZE, sizeof(led_task_t));
    if (g_led_queue == NULL)
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