#pragma once
#include "util.hpp"

using speed_t = uint32_t;

#define MOTER_QUEUE_SIZE 10
extern QueueHandle_t g_moter_queue;

void moter_task(void *arg);

class moter_driver_t
{
public:
    moter_driver_t(gpio_num_t out)
    {
        this->out = out;
        gpio_pad_select_gpio(this->out);
        gpio_set_direction(this->out, GPIO_MODE_OUTPUT);
        ESP_LOGI("moter_task_h", "moter_driver_t constructor called");
    }

    void init();
    void exec(speed_t speed);

private:
    void servo_cfg_init();
    void send_pwm_signal(speed_t speed);
    const speed_t MOTER_MAX_SPEED = 255;
    const speed_t MOTER_MIN_SPEED = 0;
    gpio_num_t out;
};