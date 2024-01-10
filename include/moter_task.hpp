#pragma once
#include "util.hpp"
#include "ESP32Servo.h"

using speed_t = uint32_t;
using signal_t = uint32_t;

#define MOTER_QUEUE_SIZE 10

void change_speed(const speed_t speed);
void moter_task(void *arg);

class moter_driver_t {
  public:
    moter_driver_t(gpio_num_t out) {
        this->out = out;
        gpio_pad_select_gpio(this->out);
        gpio_set_direction(this->out, GPIO_MODE_OUTPUT);
        ESP_LOGI("moter_task_h", "moter_driver_t constructor called");
    }
    ~moter_driver_t() {
        ESP_LOGI("moter_task_h", "moter_driver_t destructor called");
        servo.detach();
    }

    void init();
    void exec(speed_t speed);

  private:
    inline void servo_cfg_init();
    inline void send_pwm_signal(const speed_t speed);
    inline signal_t speed_to_signal(const speed_t speed);
    Servo servo;
    const signal_t MOTER_MAX_SIGNAL = 1000;
    const signal_t MOTER_MIN_SIGNAL = 500;
    const speed_t MOTER_MAX_SPEED = 100;
    const speed_t MOTER_MIN_SPEED = 0;
    gpio_num_t out;
};