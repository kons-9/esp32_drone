#pragma once

#include "util.hpp"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include "Adafruit_NeoPixel.h"

// from serial or communication(todo) or sensor?
typedef enum {
    FAST,
    SLOW,
    RAINBOW,
} led_task_t;

const char *led_task_to_str(led_task_t task_type);

class led_task_manager_t {
  public:
    led_task_manager_t(gpio_num_t pin, gpio_num_t debug_pin = GPIO_NUM_3)
        : led_pin(pin)
        , debug_led_pin(debug_pin)
        , led_task_type(FAST)
        , strip(NUMPIXELS, pin, NEO_GRB + NEO_KHZ800) {
        gpio_pad_select_gpio(debug_pin);
        auto err = gpio_set_direction(debug_pin, GPIO_MODE_OUTPUT);
        if (err != ESP_OK) {
            ESP_LOGE("LED", "Failed to set direction");
            myexit(1);
        }

        ESP_LOGI("LED", "manager constructed");
    }

    void run();
    void init();
    void write(const led_task_t task_type);

  private:
    void inline fast_exec();
    void inline slow_exec();
    void inline rainbow_exec();

    // for rainbow
    void inline rainbow_cycle(uint16_t rainbow_cnt);
    uint32_t inline wheel(byte wheel_pos);
    Adafruit_NeoPixel strip;

    // state of task
    volatile led_task_t led_task_type;
    uint8_t debug_led_state = 0;
    // 意図的にオーバーフローさせる
    uint8_t rainbow_cnt = 0;

    // configurations
    const gpio_num_t led_pin;
    const gpio_num_t debug_led_pin;
    static constexpr uint16_t NUMPIXELS = 1 << 6;  // 2^6 = 64
    static constexpr uint8_t RAINBOW_DELAY = 20;
};

void led_task(void *arg);
led_task_t get_led_task_type(void);
void led_receive_task(void *arg);
void change_led_type(led_task_t task_type);
