#pragma once
#include <WiFi.h>

enum wifi_err {
    WIFI_OK,
    WIFI_ERR,
};

extern const char *WIFI_SSID;
extern const char *WIFI_PASS;

wifi_err wifi_init(void);
void wifi_reset(void);
bool wifi_is_connected(void);
void display_wifi_info(void);
void wifi_task(void *arg);