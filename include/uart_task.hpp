#pragma once
#include "util.hpp"
#include "driver/uart.h"

void uart_event_task(void *args);

class uart_driver_t
{
public:
    uart_driver_t(uart_port_t uart_num);

    void init();
    void uart_read();
    void uart_write();
    QueueHandle_t uart_queue;
    uart_port_t uart_num;
    const int BUF_SIZE = 1024;

private:
    uart_config_t uart_config;
    TaskHandle_t uart_task_handle;
};
