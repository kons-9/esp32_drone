#include "webserver.hpp"

AsyncWebServer server(80);

const char* ssid = "toshikiiPhone";
const char* password = "dronedrone";
const char * html = R"(<html> <head> <title>ESP32_WebServer</title> </head> <body> <h1>ESP32_WebServer</h1> <p>速度変更</p> <p>スピード：%SPEED%</p> <p> <button type="button" onclick="location.href='PLUS'">プラス10</button> <button type="button" onclick="location.href='MINUS'">マイナス10</button> </p> </body> </html>)";

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

void webserver_init(void) {

    Serial.begin(115200);

    // SPIFFSのセットアップ
    // if (!SPIFFS.begin(true)) {
    //   Serial.println("An Error has occurred while mounting SPIFFS");
    //   return;
    // }

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
      request->send(200, "text/html",html);
    });

    // style.cssにアクセスされた時のレスポンス

    // プラスボタンにアクセスされた時のレスポンス
    server.on("/PLUS", HTTP_GET, [](AsyncWebServerRequest * request) {
      speed +=10;
      request->redirect("/");
      Serial.print("PLUSBUTTON is Pressed");
    });
    // マイナスボタンにアクセスされた時のレスポンス
    server.on("/MINUS", HTTP_GET, [](AsyncWebServerRequest * request) {
      speed -= 10;
      request->redirect("/");
      Serial.print("MINUSBUTTON is Pressed");
    });

    // ESP32_WebServer start
    server.begin();
    Serial.println("ESP32_WebServer start!");
}
void webserver_run(void *args) {
  volatile bool flag=true;
  while(flag){
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  delay(2000);}
}
