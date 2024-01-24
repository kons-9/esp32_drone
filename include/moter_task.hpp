#pragma once
#include "util.hpp"
#include "ESP32Servo.h"

using speed_t = uint32_t;
using signal_t = uint32_t;

void change_speed(const speed_t speed);
speed_t get_speed(void);
void moter_task(void *arg);

class moter_driver_t {
  private:
    inline void servo_cfg_init(void) {
        servo.setPeriodHertz(50);
        servo.attach(out, MOTER_MIN_SIGNAL, MOTER_MAX_SIGNAL);
    }

  public:
    moter_driver_t(gpio_num_t out)
        : out(out)
        , servo(Servo()) {
        gpio_pad_select_gpio(out);
        gpio_set_direction(out, GPIO_MODE_OUTPUT);

        servo_cfg_init();
    }
    ~moter_driver_t() {
        servo.detach();
    }

    void init(void);
    void exec(speed_t speed);

  private:
    inline void send_pwm_signal(const speed_t speed);
    inline signal_t speed_to_signal(const speed_t speed);
    Servo servo;
    static constexpr signal_t MOTER_MAX_SIGNAL = 2000;
    static constexpr signal_t MOTER_MIN_SIGNAL = 1000;
    static constexpr speed_t MOTER_MAX_SPEED = 100;
    static constexpr speed_t MOTER_MIN_SPEED = 0;
    const gpio_num_t out;
};
