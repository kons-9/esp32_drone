#pragma once

#include "util.hpp"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include "Adafruit_NeoPixel.h"

#define LED_QUEUE_SIZE 10
#define NUMPIXELS 60

// from serial or communication(todo) or sensor?
typedef enum {
    FAST,
    SLOW,
    RAINBOW,
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
    void write(led_task_t task_type);

  private:
    void inline fast_exec();
    void inline slow_exec();
    void inline rainbow_exec();

    void inline rainbow_cycle(uint8_t wait);
    uint32_t inline wheel(byte wheel_pos);
    Adafruit_NeoPixel strip;
    gpio_num_t led_pin;
    led_task_t led_task_type;
};

void led_task(void *arg);
void change_led_type(led_task_t task_type);

extern "C" void change_led_type_by_uint8(const uint8_t task_type);
