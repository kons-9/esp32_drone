#include "webserver.hpp"
#include "moter_task.hpp"
#include "led_task.hpp"
#include "wifi.hpp"
#include "event.hpp"

static AsyncWebServer server(80);

static const char *html = (char *)malloc(6000);

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
    server.on("/plus", HTTP_GET, [](AsyncWebServerRequest *request) {
        speed_t speed = get_speed();
        speed += 10;
        request->redirect("/");
        change_speed(speed);
        ESP_LOGI("webserver", "PLUSBUTTON is Pressed");
    });
    // マイナスボタンにアクセスされた時のレスポンス
    server.on("/minus", HTTP_GET, [](AsyncWebServerRequest *request) {
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

    server.on("/slow", HTTP_GET, [](AsyncWebServerRequest *request) {
        const led_task_t led = led_task_t::SLOW;
        change_led_type(led);

        request->redirect("/");
        ESP_LOGI("webserver", "SLOW is Pressed");
    });

    server.on("/fast", HTTP_GET, [](AsyncWebServerRequest *request) {
        const led_task_t led = led_task_t::FAST;
        change_led_type(led);

        request->redirect("/");
        ESP_LOGI("webserver", "FAST is Pressed");
    });

    server.on("/rainbow", HTTP_GET, [](AsyncWebServerRequest *request) {
        led_task_t led = led_task_t::RAINBOW;
        change_led_type(led);

        request->redirect("/");
        ESP_LOGI("webserver", "RAINBOW is Pressed");
    });

    // "/setSpeed" エンドポイントの処理
    server.on("/changeSpeed", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("speed")) {
            // ここの処理は考える必要がある。現状-999から+999までの値が送られてくる...はず
            speed_t speed = get_speed();
            String speedParam = request->getParam("speed")->value();
            speed_t newSpeed = speedParam.toInt();
            speed = newSpeed;
            change_speed(speed);
            speed = get_speed();
            ESP_LOGI("webserver", "speed is changed to %d", speed);
        } else {
            ESP_LOGW("webserver", "Invalid param: speed is not changed");
        }
        request->redirect("/");
    });

    // server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     ESP_LOGI("webserver", "start_task");
    // start_task();
    //     request->redirect("/");
    // });
    server.begin();
    ESP_LOGI("webserver", "ESP32_WebServer start!");
}

void webserver_reset(void) {
    server.reset();
    ESP_LOGI("webserver", "ESP32_WebServer reset!");
}
