#include "moter_task.hpp"
#include "util.hpp"

#include "ESP32_Servo.h"

static const char *TAG = "moter_task";

QueueHandle_t g_moter_queue;

void moter_task(void *arg) {
    moter_driver_t *driver = (moter_driver_t *)arg;
    speed_t speed = 0;

    while (true) {
        if (xQueueReceive(g_moter_queue, &speed, 10) == pdTRUE) {
            ESP_LOGI(TAG, "speed: %d", (uint8_t)speed);
            driver->exec(speed);
        }
    }
}

void moter_driver_t::send_pwm_signal(speed_t speed) {
    // TODO
}

void moter_driver_t::servo_cfg_init() {
    // TODO
}

void moter_driver_t::init() {

    ESP_LOGI(TAG, "Initializing Moter Task Manager");
    g_moter_queue = xQueueCreate(MOTER_QUEUE_SIZE, sizeof(speed_t));
    if (g_moter_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create moter queue");
        exit(1);
    }
    servo_cfg_init();

    // init servo driver
    send_pwm_signal(MOTER_MAX_SPEED);
    vTaskMilliSecondDelay(2000);

    send_pwm_signal(MOTER_MIN_SPEED);
    vTaskMilliSecondDelay(1000);
}

void moter_driver_t::exec(speed_t speed) {
    // TODO: send pwm signal
    if (speed > MOTER_MAX_SPEED) {
        speed = MOTER_MAX_SPEED;
        ESP_LOGW(TAG, "speed is over MOTER_MAX_SPEED");
    } else if (speed < MOTER_MIN_SPEED) {
        speed = MOTER_MIN_SPEED;
        ESP_LOGW(TAG, "speed is under MOTER_MIN_SPEED");
    }

    send_pwm_signal(speed);
}
