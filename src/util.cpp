#include "util.hpp"
void vTaskMilliSecondDelay(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void myexit(int code) {
    ESP_LOGI("myexit", "exit(%d)", code);
    vTaskMilliSecondDelay(1000);
    exit(code);
}