#include "Handler.hpp"
#include "ESPAsyncWebServer.h"

Handler::Handler(ClockConfig * config) {
    this->config = config;
};

void Handler::onColors(AsyncWebServerRequest *request) {
    this->config->colorMinutes = Color(
        request->getParam("color_min_r")->value().toInt(), 
        request->getParam("color_min_g")->value().toInt(),
        request->getParam("color_min_b")->value().toInt()
    );
    request->send(200);
};