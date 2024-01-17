#include "webserver.hpp"

AsyncWebServer server(80);

const char* ssid = "xxxxxxxx";
const char* password = "xxxxxxxx";

ulong speed = 0;

// 内部変数でhtmlファイル内のSTATEの文字を変える
String processor(const String& var) {
  Serial.println(var);
  if (var == "SPEED") {
    return String(speed);
  } else {
    return String();
  }
}

void webserver_init(void *args) {

    Serial.begin(115200);

    // SPIFFSのセットアップ
    if (!SPIFFS.begin(true)) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

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
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    // style.cssにアクセスされた時のレスポンス
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send(SPIFFS, "/style.css", "text/css");
    });
    // 画像にアクセスされた時のレスポンス
    server.on("/image", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send(SPIFFS, "/image.png", "image/png");
    });
    // プラスボタンにアクセスされた時のレスポンス
    server.on("/PLUS", HTTP_GET, [](AsyncWebServerRequest * request) {
      speed +=10;
    });
    // マイナスボタンにアクセスされた時のレスポンス
    server.on("/MINUS", HTTP_GET, [](AsyncWebServerRequest * request) {
      speed -= 10;
      request->redirect("/");
    });

    // ESP32_WebServer start
    server.begin();
    Serial.println("ESP32_WebServer start!");
}
void webserver_run() {
  delay(1000);
}