#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

void event_init(void);
void start_task(void);
void wait_start_task(void);
void delete_start_task(void);
