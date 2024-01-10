#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

#define LED_QUEUE_SIZE 10
extern QueueHandle_t g_led_queue;

// from serial or communication(todo) or sensor?
typedef enum {
    FAST,
    SLOW,
} led_task_t;

class led_task_manager_t {
  public:
    led_task_manager_t(gpio_num_t pin) {
        gpio_pad_select_gpio(pin);
        gpio_set_direction(pin, GPIO_MODE_OUTPUT);
        this->led_pin = pin;
    }

    void run();
    void init();

  private:
    void inline fast_exec();
    void inline slow_exec();
    gpio_num_t led_pin;
    led_task_t led_task_type;
};

void led_task(void *arg);