#include "led_task.hpp"

static const char *TAG = "led_task";
static constexpr UBaseType_t LED_QUEUE_SIZE = 10;

static QueueHandle_t g_led_queue = NULL;
static led_task_t s_led_task_type;

void change_led_type(const led_task_t task_type) {
    ESP_LOGI(TAG, "Changing led task type");
    if (g_led_queue == NULL) {
        ESP_LOGE(TAG, "g_led_queue is NULL");
        myexit(1);
    }
    if (xQueueSendToBack(g_led_queue, &task_type, 10) != pdTRUE) {
        if (xQueueSendToBack(g_led_queue, &task_type, 10) != pdTRUE) {
            ESP_LOGE(TAG, "Failed to send led task");
            myexit(1);
        }
        s_led_task_type = task_type;
    }

    const char *led_task_to_str(const led_task_t task_type) {
        switch (task_type) {
        case FAST: return "FAST";
        case SLOW: return "SLOW";
        case RAINBOW: return "RAINBOW";
        default: myexit(1);
        }

        return NULL;
    }

    led_task_t get_led_task_type() {
        return s_led_task_type;
    }

    void led_task(void *arg) {
        led_task_manager_t *manager = (led_task_manager_t *)arg;
        manager->run();
    }

    void led_receive_task(void *arg) {
        led_task_manager_t *manager = (led_task_manager_t *)arg;
        if (g_led_queue == NULL) {
            ESP_LOGE(TAG, "g_led_queue is NULL");
            myexit(1);
        }

        change_led_type(RAINBOW);

        while ((volatile bool)(true)) {
            if (xQueueReceive(g_led_queue, &s_led_task_type, 1000) == pdTRUE) {
                manager->write(s_led_task_type);
            }
        }
    }

    void led_task_manager_t::write(const led_task_t task_type) {
        void led_task_manager_t::write(const led_task_t task_type) {
            led_task_type = task_type;
        }

        void led_task_manager_t::run() {
            while ((volatile bool)(true)) {
                while ((volatile bool)(true)) {
                    switch (led_task_type) {
                    case FAST:
                        ESP_LOGI(TAG, "FAST");
                        fast_exec();
                        break;
                    case SLOW:
                        ESP_LOGI(TAG, "SLOW");
                        slow_exec();
                        break;
                    case RAINBOW:
                        // this means rainbow_cnt == 0 mod 256
                        if (rainbow_cnt == 0) {
                            ESP_LOGI(TAG, "RAINBOW");
                        }
                        rainbow_exec();
                        break;
                    default:
                        ESP_LOGE(TAG, "Unknown task type");
                        ESP_LOGI(TAG, "reset task type to FAST");
                        change_led_type(FAST);
                        break;
                    }
                }
            }

            void led_task_manager_t::init() {
                ESP_LOGI(TAG, "Initializing LED Task Manager");
                g_led_queue = xQueueCreate(LED_QUEUE_SIZE, sizeof(led_task_t));
                if (g_led_queue == NULL) {
                    ESP_LOGE(TAG, "Failed to create queue");
                    myexit(1);
                }
                strip.begin();
                strip.show();  // Initialize all pixels to 'off'

                ESP_LOGI(TAG, "Initializing LED Task Manager done");
            }

            void inline led_task_manager_t::fast_exec() {
                gpio_set_level(debug_led_pin, debug_led_state);
                vTaskMilliSecondDelay(1000);
            }
            void inline led_task_manager_t::slow_exec() {
                gpio_set_level(debug_led_pin, debug_led_state);
                vTaskMilliSecondDelay(2000);
            }

            void inline led_task_manager_t::rainbow_exec() {
                rainbow_cycle(rainbow_cnt);
                vTaskMilliSecondDelay(RAINBOW_DELAY);
                rainbow_cnt++;
            }

            uint32_t inline led_task_manager_t::wheel(uint8_t wheelPos) {
                wheelPos = 255 - wheelPos;
                uint8_t three_times_pos = wheelPos * 3;
                if (wheelPos < 85) {
                    return strip.Color(255 - three_times_pos, 0, three_times_pos);
                }
                wheelPos -= 85;
                three_times_pos = wheelPos * 3;
                if (wheelPos < 85) {
                    return strip.Color(0, three_times_pos, 255 - three_times_pos);
                }
                wheelPos -= 85;
                three_times_pos = wheelPos * 3;
                return strip.Color(three_times_pos, 255 - three_times_pos, 0);
            }

            void inline led_task_manager_t::rainbow_cycle(uint16_t rainbow_cnt) {
                constexpr uint8_t div_256 = 256 / NUMPIXELS;

                for (uint16_t i = 0, j = 0; i < strip.numPixels(); i++, j += div_256) {
                    strip.setPixelColor(i, wheel(((j + rainbow_cnt) & 255)));
                }
                strip.show();
            }
