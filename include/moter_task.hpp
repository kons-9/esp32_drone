#define MOTER_MAX_SPEED 255
#define MOTER_MIN_SPEED 0

static const char *TAG = "moter_driver";

class moter_driver_t {
    public:
        moter_driver(gpio_num_t out){
            this->out = out;
            gpio_pad_select_gpio(this->out);
            gpio_set_direction(this->out, GPIO_MODE_OUTPUT);
        }

        void init() {
            // high for 2 seconds
            gpio_set_level(this->pin1, 1);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            // low for 1 second
            gpio_set_level(this->pin1, 0);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        
        void exec(uint8_t speed) {
            if (speed > MOTER_MAX_SPEED) {
                speed = MOTER_MAX_SPEED;
                ESP_LOGW(TAG, "speed is over MOTER_MAX_SPEED");
            } else if (speed < MOTER_MIN_SPEED) {
                speed = MOTER_MIN_SPEED;
                ESP_LOGW(TAG, "speed is under MOTER_MIN_SPEED");
            }
            gpio_set_level(this->out, speed);
        }


    private:
        gpio_num_t out;
};