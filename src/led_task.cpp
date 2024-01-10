#include "led_task.hpp"

static const char *TAG = "led_task";
QueueHandle_t g_led_queue = NULL;

void change_led_type(const led_task_t task_type) {
    ESP_LOGI(TAG, "Changing led task type");
    if (g_led_queue == NULL) {
        ESP_LOGE(TAG, "g_led_queue is NULL");
        myexit(1);
    }
    if (xQueueSendToBack(g_led_queue, (void *)&task_type, 10) != pdTRUE) {
        ESP_LOGE(TAG, "Failed to send led task");
        myexit(1);
    }
}
extern "C" void change_led_type_by_uint8(const uint8_t task_type) {
    if (task_type > 1) {
        ESP_LOGE(TAG, "Unknown task type");
    } else {
        change_led_type((led_task_t)task_type);
    }
}


void led_task(void *arg) {
    led_task_manager_t *manager = (led_task_manager_t *)arg;
    manager->run();
}

void led_receive_task(void *arg) {
    led_task_manager_t *manager = (led_task_manager_t *)arg;

    while (true) {
        led_task_t cur_task_type;
        if (xQueueReceive(g_led_queue, &cur_task_type, 1000) == pdTRUE) {
            manager->write(cur_task_type);
        }
    }
}

void led_task_manager_t::write(led_task_t task_type) {
    led_task_type = task_type;
}

void led_task_manager_t::run() {
    xTaskCreate(led_receive_task, "led_receive_task", 2048, this, 5, NULL);
    while (true) {
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
            ESP_LOGI(TAG, "RAINBOW");
            rainbow_exec();
            break;
        default: ESP_LOGE(TAG, "Unknown task type"); break;
        }
    }
}


// コンストラクタ内部でやってもいいかも
void led_task_manager_t::init() {
    ESP_LOGI(TAG, "Initializing LED Task Manager");
    g_led_queue = xQueueCreate(LED_QUEUE_SIZE, sizeof(led_task_t));
    if (g_led_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create queue");
        exit(1);
    }
    led_task_type = FAST;
    strip = Adafruit_NeoPixel(NUMPIXELS, led_pin, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.show();  // Initialize all pixels to 'off'
}

void inline led_task_manager_t::fast_exec() {
    gpio_set_level(led_pin, 1);
    vTaskMilliSecondDelay(1000);
    gpio_set_level(led_pin, 0);
    vTaskMilliSecondDelay(1000);
}
void inline led_task_manager_t::slow_exec() {
    gpio_set_level(led_pin, 1);
    vTaskMilliSecondDelay(2000);
    gpio_set_level(led_pin, 0);
    vTaskMilliSecondDelay(2000);
}

void inline led_task_manager_t::rainbow_exec() {
    rainbow_cycle(20);
}

uint32_t inline led_task_manager_t::wheel(byte wheelPos) {
    wheelPos = 255 - wheelPos;
    if (wheelPos < 85) {
        return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
    }
    if (wheelPos < 170) {
        wheelPos -= 85;
        return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
    wheelPos -= 170;
    return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

void inline led_task_manager_t::rainbow_cycle(uint8_t wait) {
    uint16_t i, j;
    for (j = 0; j < 256 * 5; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}
