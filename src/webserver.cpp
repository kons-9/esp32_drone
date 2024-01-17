#include "webserver.hpp"
#include "moter_task.hpp"
#include "led_task.hpp"

AsyncWebServer server(80);

const char *ssid = "toshikiiPhone";
const char *password = "dronedrone";
const char *html
    = R"(<html> <head> <title>ESP32_WebServer</title> </head> <body> <h1>ESP32_WebServer</h1> <p>change speed</p> <p> <button type="button" onclick="location.href='PLUS'">plus 10</button> <button type="button" onclick="location.href='MINUS'">minus 10</button> </p><p><button type="button" onclick="location.href='RAINBOW'">rainbow</button></p> </body> </html>)";
ulong speed = 0;

void webserver_init(void) {

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // GETリクエストに対するハンドラーを登録
    // rootにアクセスされた時のレスポンス
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(200, "text/html", html); });

    // プラスボタンにアクセスされた時のレスポンス
    server.on("/PLUS", HTTP_GET, [](AsyncWebServerRequest *request) {
        speed += 10;
        request->redirect("/");
        change_speed(speed);
        Serial.println("PLUSBUTTON is Pressed");
    });
    // マイナスボタンにアクセスされた時のレスポンス
    server.on("/MINUS", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (speed < 10) {
            speed = 0;
        } else {
            speed -= 10;
        }
        request->redirect("/");
        change_speed(speed);
        Serial.println("MINUSBUTTON is Pressed");
    });

    server.on("/RAINBOW", HTTP_GET, [](AsyncWebServerRequest *request) {
        led_task_t led = led_task_t::RAINBOW;
        change_led_type(led);

        request->redirect("/");
        Serial.println("RAINBOW is Pressed");
    });

    // ESP32_WebServer start
    server.begin();
    Serial.println("ESP32_WebServer start!");
}

void webserver_run(void *args) {
    volatile bool flag = true;
    while (flag) {
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        delay(2000);
    }
}
