#include "moter_task.hpp"

static const char *TAG = "moter_task";

static QueueHandle_t moter_queue = NULL;
constexpr static UBaseType_t MOTER_QUEUE_SIZE = 10;

static speed_t speed = 0;

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

speed_t get_speed() {
    return speed;
}

void moter_task(void *arg) {
    moter_driver_t *driver = (moter_driver_t *)arg;
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

void moter_driver_t::init() {

    ESP_LOGI(TAG, "Initializing Moter Task Manager");
    moter_queue = xQueueCreate(MOTER_QUEUE_SIZE, sizeof(speed_t));
    if (moter_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create moter queue");
        myexit(1);
        myexit(1);
    }

    ESP_LOGI(TAG, "Initializing Moter Driver");
    ESP_LOGI(TAG, "Initializing Moter Driver");
    send_pwm_signal(MOTER_MAX_SIGNAL);
    vTaskMilliSecondDelay(2000);

    send_pwm_signal(MOTER_MIN_SIGNAL);
    vTaskMilliSecondDelay(1000);
    ESP_LOGI(TAG, "Moter Task Manager initialized");
    ESP_LOGI(TAG, "Moter Task Manager initialized");
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
