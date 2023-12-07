#include "util.hpp"
#include "led_task.hpp"

led_task_t g_led_task_type;

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
    if (g_led_task_type == FAST)
    {
        fast_exec();
    }
    else
    {
        slow_exec();
    }
}

void led_task_manager_t::init()
{
    g_led_task_type = FAST;
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