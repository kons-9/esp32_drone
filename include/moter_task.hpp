#pragma once
#include "util.hpp"

static const char *TAG = "moter_driver";
using speed_t = uint32_t;

void moter_task(void *arg);

class moter_driver_t
{
public:
    moter_driver_t(gpio_num_t out)
    {
        this->out = out;
        gpio_pad_select_gpio(this->out);
        gpio_set_direction(this->out, GPIO_MODE_OUTPUT);
        ESP_LOGI(TAG, "moter_driver_t constructor called");
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