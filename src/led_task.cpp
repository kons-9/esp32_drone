#include "led_task.hpp"
#include "event.hpp"

static const char *TAG = "led_task";

static led_task_t s_led_task_type;

void change_led_type(const led_task_t task_type) {
    ESP_LOGI(TAG, "Changing led task type");

    s_led_task_type = task_type;
    if (xEventGroupSetBits(drone_event_group, (EventBits_t)drone_event_bit_t::LED_TYPE_CHANGE) != pdPASS) {
        ESP_LOGE(TAG, "Failed to send led task");
        myexit(1);
    }
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

    change_led_type(RAINBOW);

    while ((volatile bool)(true)) {
        if (xEventGroupWaitBits(drone_event_group,
                                (EventBits_t)drone_event_bit_t::LED_TYPE_CHANGE,
                                pdTRUE,
                                pdFALSE,
                                portMAX_DELAY)) {
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
