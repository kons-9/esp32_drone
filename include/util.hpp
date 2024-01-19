#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"
#include "rom/gpio.h"
#include "esp_log.h"

void vTaskMilliSecondDelay(uint32_t ms);
void myexit(int code);