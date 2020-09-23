#include "Handler.hpp"
#include "index_file.h"

Handler::Handler(ClockConfig * config, CircleClock *clock) {
    this->config = config;
    this->clock = clock;
};

void Handler::onColors(AsyncWebServerRequest *request) {
    this->config->colorMinutes = Color(
        request->getParam("color_min_r")->value().toInt(), 
        request->getParam("color_min_g")->value().toInt(),
        request->getParam("color_min_b")->value().toInt()
    );
    this->config->colorHours = Color(
        request->getParam("color_hour_r")->value().toInt(), 
        request->getParam("color_hour_g")->value().toInt(),
        request->getParam("color_hour_b")->value().toInt()
    );
    request->send(200);
};


void Handler::onBlink(AsyncWebServerRequest *request) {
    boolean en = false;
    if (request->getParam("enabled")->value().equals("on")) en = true;
    this->config->blinkingEnabled = en;
    request->send(200);
};

void Handler::onCustom(AsyncWebServerRequest *request) {
    int duration = request->getParam("duration")->value().toInt();
    int progress = request->getParam("progress")->value().toInt();
    this->clock->displayCustom(progress, CRGB::Aquamarine, duration);
    request->send(200);
};

void Handler::onIndex(AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
};