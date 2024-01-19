#include "util.hpp"
#include "wifi.hpp"
#include <ESPAsyncWebServer.h>

extern const char *base_html;

void webserver_init(void);
void webserver_run(void *args);
