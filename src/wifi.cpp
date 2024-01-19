#include "wifi.hpp"

static const char *TAG = "wifi";

wifi_err wifi_init(void) {
    ESP_LOGI(TAG, "Initializing WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        ESP_LOGI(TAG, "Connecting to WiFi...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "Connected to the WiFi network");
    display_wifi_info();
    return WIFI_OK;
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
