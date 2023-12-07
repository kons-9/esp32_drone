#include "moter_task.hpp"
#include "util.hpp"
#include <freertos/queue.h>

static const char *TAG = "moter_driver_t";

#define MOTER_QUEUE_SIZE 10
QueueHandle_t moter_queue;

void moter_task(void *arg)
{
    moter_driver_t *driver = (moter_driver_t *)arg;
    speed_t speed = 0;
    while (true)
    {
        if (xQueueReceive(moter_queue, &speed, 10) == pdTRUE)
        {
            ESP_LOGI(TAG, "speed: %d", speed);
            driver->exec(speed);
        }
    }
}

void moter_driver_t::send_pwm_signal(speed_t speed)
{
    // TODO
}

void moter_driver_t::servo_cfg_init()
{
    // TODO
}

void moter_driver_t::init()
{
    servo_cfg_init();

    // init servo driver
    send_pwm_signal(MOTER_MAX_SPEED);
    vTaskMilliSecondDelay(2000);

    send_pwm_signal(MOTER_MIN_SPEED);
    vTaskMilliSecondDelay(1000);
}

void moter_driver_t::exec(speed_t speed)
{
    // TODO: send pwm signal
    if (speed > MOTER_MAX_SPEED)
    {
        speed = MOTER_MAX_SPEED;
        ESP_LOGW(TAG, "speed is over MOTER_MAX_SPEED");
    }
    else if (speed < MOTER_MIN_SPEED)
    {
        speed = MOTER_MIN_SPEED;
        ESP_LOGW(TAG, "speed is under MOTER_MIN_SPEED");
    }

    send_pwm_signal(speed);
}