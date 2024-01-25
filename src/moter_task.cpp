#include "moter_task.hpp"
#include "event.hpp"
#include "led_task.hpp"

static const char *TAG = "moter_task";

constexpr static uint16_t SAFE_CNT_MAX = 1000;
constexpr static TickType_t MOTER_TASK_DELAY = 10;

static EventGroupHandle_t moter_event = xEventGroupCreate();
static constexpr EventBits_t MOTER_SPEED_CHANGE_BIT = BIT0;

static speed_t speed = 0;

void change_speed(const speed_t next_speed) {
    ESP_LOGI(TAG, "Changing speed");
    speed = next_speed;
    if (xEventGroupSetBits(moter_event, MOTER_SPEED_CHANGE_BIT) & MOTER_SPEED_CHANGE_BIT) {
        ESP_LOGI(TAG, "Succeeded to send speed");
    } else {
        ESP_LOGE(TAG, "Failed to send speed");
        speed = 0;
    }
}

speed_t get_speed() {
    return speed;
}

void moter_task(void *arg) {
    moter_driver_t *driver = (moter_driver_t *)arg;
    uint16_t safe_cnt = 0;

    while (true) {
        // clear on exit を on にするとバグる
        if (xEventGroupWaitBits(moter_event, MOTER_SPEED_CHANGE_BIT, pdFALSE, pdFALSE, MOTER_TASK_DELAY)) {
            xEventGroupClearBits(moter_event, MOTER_SPEED_CHANGE_BIT);
            safe_cnt = 0;
            ESP_LOGI(TAG, "speed: %d", (uint8_t)speed);
            driver->exec(speed);
        } else {
            safe_cnt++;
            if (safe_cnt > SAFE_CNT_MAX) {
                ESP_LOGW(TAG, "safe_cnt is over 1000, so speed is set to 0");
                speed = 0;
                safe_cnt = 0;
            }
            // send speed periodically
            driver->exec(speed);
        }
    }
}

inline void moter_driver_t::send_pwm_signal(const speed_t speed) {
    servo.write(speed);
}

void moter_driver_t::init(void) {

    ESP_LOGI(TAG, "Initializing Moter Task Manager");

    // not use in normal init
    // ESP_LOGI(TAG, "Initializing Moter Driver");
    // send_pwm_signal(MOTER_MAX_SIGNAL);
    // vTaskMilliSecondDelay(2000);

    send_pwm_signal(MOTER_MIN_SIGNAL);
    vTaskMilliSecondDelay(1000);
    ESP_LOGI(TAG, "Moter Task Manager initialized");
}

inline signal_t moter_driver_t::speed_to_signal(const speed_t speed) {
    // ratio should be integer
    constexpr static signal_t RATIO = (MOTER_MAX_SIGNAL - MOTER_MIN_SIGNAL) / (MOTER_MAX_SPEED - MOTER_MIN_SPEED);
    static_assert((MOTER_MAX_SIGNAL - MOTER_MIN_SIGNAL) % (MOTER_MAX_SPEED - MOTER_MIN_SPEED) == 0,
                  "RATIO should be integer");
    return RATIO * speed + MOTER_MIN_SIGNAL;
}

static bool flag = false;

void moter_driver_t::exec(speed_t speed) {
    if (speed >= MOTER_MAX_SPEED) {
        speed = MOTER_MAX_SPEED;
        ESP_LOGW(TAG, "speed is MOTER_MAX_SPEED");
        change_led_type(led_task_t::MAX_SPEED);
        flag = true;
    } else if (speed <= MOTER_MIN_SPEED) {
        speed = MOTER_MIN_SPEED;
        ESP_LOGW(TAG, "speed is MOTER_MIN_SPEED");
        change_led_type(led_task_t::MIN_SPEED);
        flag = true;
    }
    if (flag) {
        change_led_type(led_task_t::RAINBOW);
    }

    send_pwm_signal(speed_to_signal(speed));
}
