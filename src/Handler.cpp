#include "Handler.hpp"
#include "index_file.h"

Handler::Handler(ClockConfig * config, CircleClock *clock) {
    this->config = config;
    this->clock = clock;
};

void Handler::onColors(AsyncWebServerRequest *request) {
    this->config->colorMinutes = hexToColor(request->getParam("color_min")->value());
    this->config->colorHours = hexToColor(request->getParam("color_hour")->value());
    request->redirect("/");
};


void Handler::onBlink(AsyncWebServerRequest *request) {
    boolean en = false;
    if (request->hasParam("enabled")) {
        if (request->getParam("enabled")->value().equals("on")) en = true;
    }
    this->config->blinkingEnabled = en;
    request->redirect("/");
};

void Handler::onCustom(AsyncWebServerRequest *request) {
    int duration = request->getParam("duration")->value().toInt();
    int progress = request->getParam("progress")->value().toInt();
    this->clock->displayCustom(progress, CRGB::Aquamarine, duration);
    request->redirect("/");
};

void Handler::onIndex(AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
};

Color Handler::hexToColor(String input) {
    long rgb = strtol(input.c_str() + 1, 0, 16); // parse as Hex, skipping the leading '#'
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;
    return Color(r, g, b);
}