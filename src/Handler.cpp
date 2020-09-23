#include "Handler.hpp"
#include "index_file.h"

Handler::Handler(ClockConfig * config, TIDILE *tidile) {
    this->config = config;
    this->tidile = tidile;
};

void Handler::onColors(AsyncWebServerRequest *request) {
    this->config->colorMinutes = hexToColor(request->getParam("color_min")->value());
    this->config->colorHours = hexToColor(request->getParam("color_hour")->value());
    request->redirect("/");
    this->config->serialize();
};


void Handler::onBlink(AsyncWebServerRequest *request) {
    boolean en = false;
    bool showSecs = false;
    if (request->hasParam("enabled")) {
        if (request->getParam("enabled")->value().equals("on")) en = true;
    }
    if (request->hasParam("brightness")) {
        this->config->brightness = request->getParam("brightness")->value().toInt();
    }
    if (request->hasParam("show_seconds")) {
        if (request->getParam("show_secs")->value().equals("on")) showSecs = true;
    }
    this->config->blinkingEnabled = en;
    this->config->displaySeconds = showSecs;
    request->redirect("/");
    this->config->serialize();
};

void Handler::onCustom(AsyncWebServerRequest *request) {
    int duration = request->getParam("duration")->value().toInt();
    int progress = request->getParam("progress")->value().toInt();
    this->tidile->displayCustom(progress, CRGB::Aquamarine, duration);
    request->redirect("/");
};

void Handler::onIndex(AsyncWebServerRequest *request) {
    String html = index_html;
    html.replace(COLORHOURKEYWORD, colorToHEX(this->config->colorHours));
    html.replace(COLORMINUTEKEYWORD, colorToHEX(this->config->colorMinutes));
    html.replace(BLINKINGENABLEDKEYWORD, String(this->config->blinkingEnabled));
    html.replace(BRIGHTNESSKEYWORD, String(this->config->brightness));
    html.replace(COLORTEMPERATUREKEYWORD, colorToHEX(this->config->colorTemperature));
    html.replace(COLORPRESSUREKEYWORD, colorToHEX(this->config->colorPressure));
    html.replace(COLORHUMIDITYKEYWORD, colorToHEX(this->config->colorHumidity));
    request->send(200, "text/html", html);
};

// Thank you stack overflow!
Color Handler::hexToColor(String input) {
    long rgb = strtol(input.c_str() + 1, 0, 16); // parse as Hex, skipping the leading '#'
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;
    return Color(r, g, b);
}

String Handler::colorToHEX(Color color){
    String red = String(color.red, HEX);
    if(red.length() == 1) red = "0" + red;
    String green = String(color.green, HEX);
    if(green.length() == 1) green = "0" + green;
    String blue = String(color.blue, HEX);
    if(blue.length() == 1) blue = "0" + blue;
    return "#" + red + green + blue;
}