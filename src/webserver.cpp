#include "webserver.hpp"
#include "moter_task.hpp"
#include "led_task.hpp"
#include "wifi.hpp"

static AsyncWebServer server(80);

static const char *html = (char *)malloc(1024);

void webserver_init(void) {
    // GETリクエストに対するハンドラーを登録
    // rootにアクセスされた時のレスポンス
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        // レスポンスを返す
        speed_t speed = get_speed();
        led_task_t led = get_led_task_type();
        sprintf((char *)html, base_html, speed, led_task_to_str(led));

        request->send(200, "text/html", html);
    });

    // プラスボタンにアクセスされた時のレスポンス
    server.on("/PLUS", HTTP_GET, [](AsyncWebServerRequest *request) {
        speed_t speed = get_speed();
        speed += 10;
        request->redirect("/");
        change_speed(speed);
        ESP_LOGI("webserver", "PLUSBUTTON is Pressed");
    });
    // マイナスボタンにアクセスされた時のレスポンス
    server.on("/MINUS", HTTP_GET, [](AsyncWebServerRequest *request) {
        speed_t speed = get_speed();
        if (speed < 10) {
            speed = 0;
        } else {
            speed -= 10;
        }
        request->redirect("/");
        change_speed(speed);
        ESP_LOGI("webserver", "MINUSBUTTON is Pressed");
    });

    server.on("/SLOW", HTTP_GET, [](AsyncWebServerRequest *request) {
        const led_task_t led = led_task_t::SLOW;
        change_led_type(led);

        request->redirect("/");
        ESP_LOGI("webserver", "SLOW is Pressed");
    });

    server.on("/FAST", HTTP_GET, [](AsyncWebServerRequest *request) {
        const led_task_t led = led_task_t::FAST;
        change_led_type(led);

        request->redirect("/");
        ESP_LOGI("webserver", "FAST is Pressed");
    });

    server.on("/RAINBOW", HTTP_GET, [](AsyncWebServerRequest *request) {
        led_task_t led = led_task_t::RAINBOW;
        change_led_type(led);

        request->redirect("/");
        ESP_LOGI("webserver", "RAINBOW is Pressed");
    });
}

void webserver_run(void *args) {
    server.begin();
    ESP_LOGI("webserver", "ESP32_WebServer start!");
    while (true) {
        display_wifi_info();
        vTaskMilliSecondDelay(10000);
    }
}
