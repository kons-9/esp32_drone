#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

enum class drone_event_bit_t : EventBits_t {
    MOTER_SPEED_CHANGE = 1 << 1,
    LED_TYPE_CHANGE = 1 << 2,
    START_TASK = 1 << 3,
};

// TODO: globalじゃなくてもいい
extern EventGroupHandle_t drone_event_group;