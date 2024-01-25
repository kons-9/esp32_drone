#include "led_task.hpp"
#include "event.hpp"

static const char *TAG = "led_task";

static led_task_t s_led_task_type = led_task_t::FAST;
static EventGroupHandle_t led_event = xEventGroupCreate();
static constexpr EventBits_t LED_TYPE_CHANGE_BIT = BIT0;

void change_led_type(const led_task_t task_type) {
    if (led_event == NULL) {
        ESP_LOGE(TAG, "drone_event_group is NULL");
        return;
    }
    ESP_LOGI(TAG, "Changing led task type");

    auto pre = s_led_task_type;
    s_led_task_type = task_type;
    if (xEventGroupSetBits(led_event, LED_TYPE_CHANGE_BIT) & LED_TYPE_CHANGE_BIT) {
        ESP_LOGI(TAG, "Succeeded to send led task");
    } else {
        ESP_LOGE(TAG, "Failed to send led task");
        s_led_task_type = pre;
    }
}

const char *led_task_to_str(const led_task_t task_type) {
    switch (task_type) {
    case FAST: return "FAST";
    case SLOW: return "SLOW";
    case RAINBOW: return "RAINBOW";
    case MAX_SPEED: return "MAX_SPEED";
    case MIN_SPEED: return "MIN_SPEED";
    default: {
        ESP_LOGE(TAG, "Unknown task type");
        return "Unknown task type";
    };
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

    while ((volatile bool)(true)) {
        // clear on exit を on にするとバグる
        if (xEventGroupWaitBits(led_event, LED_TYPE_CHANGE_BIT, pdFALSE, pdFALSE, portMAX_DELAY)) {
            xEventGroupClearBits(led_event, LED_TYPE_CHANGE_BIT);
            manager->write(s_led_task_type);
        }
    }
}

void led_task_manager_t::write(const led_task_t task_type) {
    led_task_type = task_type;
}

void led_task_manager_t::run() {
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
        case MAX_SPEED:
            ESP_LOGI(TAG, "MAX_SPEED");
            max_speed_exec();
            break;
        case MIN_SPEED:
            ESP_LOGI(TAG, "MIN_SPEED");
            min_speed_exec();
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
    strip.begin();
    strip.show();  // Initialize all pixels to 'off'
}

void inline led_task_manager_t::fast_exec() {
    debug_led_state = 1 ^ debug_led_state;
    gpio_set_level(debug_led_pin, debug_led_state);
    vTaskMilliSecondDelay(1000);
}
void inline led_task_manager_t::slow_exec() {
    debug_led_state = 1 ^ debug_led_state;
    gpio_set_level(debug_led_pin, debug_led_state);
    vTaskMilliSecondDelay(2000);
}

void inline led_task_manager_t::rainbow_exec() {
    rainbow_cycle(rainbow_cnt);
    vTaskMilliSecondDelay(RAINBOW_DELAY);
    rainbow_cnt++;
}
void inline led_task_manager_t::max_speed_exec() {
    strip.setPixelColor(0, strip.Color(255, 0, 0));
    strip.show();
    vTaskMilliSecondDelay(100);
}

void inline led_task_manager_t::min_speed_exec() {
    strip.setPixelColor(0, strip.Color(0, 255, 0));
    strip.show();
    vTaskMilliSecondDelay(100);
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
    static_assert(256 % NUMPIXELS == 0, "NUMPIXELS should be divisor of 256");

    for (uint16_t i = 0, j = rainbow_cnt; i < strip.numPixels(); i++, j += div_256) {
        strip.setPixelColor(i, wheel((j & 255)));
    }
    strip.show();
}
