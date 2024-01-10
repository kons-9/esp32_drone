#include "moter_task.hpp"

static const char *TAG = "moter_task";

QueueHandle_t moter_queue;

void change_speed(const speed_t speed) {
    ESP_LOGI(TAG, "Changing speed");
    if (moter_queue == NULL) {
        ESP_LOGE(TAG, "g_moter_queue is NULL");
        myexit(1);
    }
    if (xQueueSendToBack(moter_queue, (void *)&speed, 10) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to send speed");
        myexit(1);
    }
}

void moter_task(void *arg) {
    moter_driver_t *driver = (moter_driver_t *)arg;
    speed_t speed = 0;

    while (true) {
        if (xQueueReceive(moter_queue, &speed, 10) == pdTRUE) {
            ESP_LOGI(TAG, "speed: %d", (uint8_t)speed);
            driver->exec(speed);
        }
    }
}

inline void moter_driver_t::send_pwm_signal(const speed_t speed) {
    servo.write(speed);
}

inline void moter_driver_t::servo_cfg_init() {
    servo.setPeriodHertz(50);
    servo.attach(out, MOTER_MIN_SIGNAL, MOTER_MAX_SIGNAL);
}

void moter_driver_t::init() {

    ESP_LOGI(TAG, "Initializing Moter Task Manager");
    moter_queue = xQueueCreate(MOTER_QUEUE_SIZE, sizeof(speed_t));
    if (moter_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create moter queue");
        exit(1);
    }
    servo_cfg_init();

    // init servo driver
    send_pwm_signal(MOTER_MAX_SIGNAL);
    vTaskMilliSecondDelay(2000);

    send_pwm_signal(MOTER_MIN_SIGNAL);
    vTaskMilliSecondDelay(1000);
}

inline signal_t moter_driver_t::speed_to_signal(const speed_t speed) {
    return (MOTER_MAX_SIGNAL - MOTER_MIN_SIGNAL) / (MOTER_MAX_SPEED - MOTER_MIN_SPEED) * speed + MOTER_MIN_SIGNAL;
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

    send_pwm_signal(speed_to_signal(speed));
}
