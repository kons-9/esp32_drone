#include "event.hpp"
#include "util.hpp"

#include <esp_log.h>

static EventGroupHandle_t drone_event_start_task;
static constexpr EventBits_t START_TASK_BIT = BIT0;

void event_init(void) {
    ESP_LOGI("event", "Initializing event group");
    drone_event_start_task = xEventGroupCreate();
    if (drone_event_start_task == NULL) {
        ESP_LOGE("event", "Failed to create event group");
        vTaskMilliSecondDelay(1000);
        exit(1);
    }
}

void start_task(void) {
    ESP_LOGI("event", "start task");
    xEventGroupSetBits(drone_event_start_task, START_TASK_BIT);
}

void wait_start_task(void) {
    ESP_LOGI("event", "wait start task");
    while (xEventGroupGetBits(drone_event_start_task) != START_TASK_BIT) {
        ESP_LOGI("event", "wait start task");
        vTaskMilliSecondDelay(1000);
    }
}

void delete_start_task(void) {
    ESP_LOGI("event", "delete start task");
    vEventGroupDelete(drone_event_start_task);
}