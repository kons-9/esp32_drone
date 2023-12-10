#include "util.hpp"
#include "led_task.hpp"
#include "moter_task.hpp"

struct parser_t
{
    command_t command;
    // for led
    led_task_t led_task;
    // for moter
    speed_t speed;
};

enum command_t
{
    CMD_ERROR = -1,

    // init
    CMD_INIT,

    // for led
    CMD_LED_FAST,
    CMD_LED_SLOW,

    // for moter
    CMD_MOTER_SPEED,
};