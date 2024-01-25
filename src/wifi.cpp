#include "wifi.hpp"
#include "webserver.hpp"
#include "util.hpp"

static const char *TAG = "wifi";

wifi_err wifi_init(void) {
    ESP_LOGI(TAG, "Initializing WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        ESP_LOGI(TAG, "Connecting to WiFi...");
        vTaskMilliSecondDelay(1000);
    }
    ESP_LOGI(TAG, "Connected to the WiFi network");
    display_wifi_info();
    return WIFI_OK;
}

void wifi_reset(void) {
    ESP_LOGI(TAG, "Resetting WiFi");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        ESP_LOGI(TAG, "Connecting to WiFi...");
        vTaskMilliSecondDelay(1000);
    }
    ESP_LOGI(TAG, "Connected to the WiFi network");
    display_wifi_info();
}
bool wifi_is_connected(void) {
    return WiFi.status() == WL_CONNECTED;
}

void display_wifi_info(void) {
    // ssid
    ESP_LOGI(TAG, "SSID: %s", WiFi.SSID().c_str());
    // hostname
    ESP_LOGI(TAG, "Hostname: %s", WiFi.getHostname());
    // ip address
    ESP_LOGI(TAG, "IP address: %s", WiFi.localIP().toString().c_str());
    // mac address
    ESP_LOGI(TAG, "MAC address: %s", WiFi.macAddress().c_str());
}

void wifi_task(void *arg) {
    webserver_init();
    auto cnt = 0;
    while (true) {
        if (!wifi_is_connected()) {
            ESP_LOGW(TAG, "WiFi is not connected");
            wifi_reset();
            webserver_reset();
        } else {
            if (cnt % 10 == 0) {
                display_wifi_info();
            }
            cnt++;
            vTaskMilliSecondDelay(10000);
        }
    }
}
