#pragma once

#include <driver/gpio.h>

typedef enum {
    FAST,
    SLOW
} led_task_t;

class led_task_manager_t {
    public:
        void led_task(gpio_num_t pin).led_pin(pin) {};
        void fast_exec();
        void slow_exec();

        void init();
    private:
        gpio_num_t led_pin;
};



