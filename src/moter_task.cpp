#include "moter_task.hpp"
#include "util.hpp"

// TODO: changed by communication task
// mabye use mutex? or channel?
speed_t g_speed = 0;

void moter_task(void *arg)
{
    moter_driver_t *driver = (moter_driver_t *)arg;
    driver->init();
    while (true)
    {
        driver->exec(g_speed);
    }
}

void moter_driver_t::send_pwm_signal(speed_t speed)
{
    // TODO
}

void moter_driver_t::servo_cfg_init()
{
    // TODO
}

void moter_driver_t::init()
{
    servo_cfg_init();

    // init servo driver
    send_pwm_signal(MOTER_MAX_SPEED);
    vTaskMilliSecondDelay(2000);

    send_pwm_signal(MOTER_MIN_SPEED);
    vTaskMilliSecondDelay(1000);
}

void moter_driver_t::exec(speed_t speed)
{
    // TODO: send pwm signal
    if (speed > MOTER_MAX_SPEED)
    {
        speed = MOTER_MAX_SPEED;
        ESP_LOGW(TAG, "speed is over MOTER_MAX_SPEED");
    }
    else if (speed < MOTER_MIN_SPEED)
    {
        speed = MOTER_MIN_SPEED;
        ESP_LOGW(TAG, "speed is under MOTER_MIN_SPEED");
    }

    send_pwm_signal(speed);
}