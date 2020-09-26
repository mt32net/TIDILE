#include "Handler.hpp"
#include "index_file.hpp"

Handler::Handler(ClockConfig * config, TIDILE *tidile, Preferences* preferences) {
    this->config = config;
    this->tidile = tidile;
    this->preferences = preferences;
}

void Handler::onColors(AsyncWebServerRequest *request) {
    this->config->colorMinutes = hexToColor(request->getParam("color_min")->value());
    this->config->colorHours = hexToColor(request->getParam("color_hour")->value());
    this->config->colorSeconds = hexToColor(request->getParam("color_sec")->value());
    request->redirect("/");
    this->config->serialize(preferences);
}

void Handler::onEnvColors(AsyncWebServerRequest *request) {
    this->config->colorTemperature = hexToColor(request->getParam("color_temp")->value());
    this->config->colorPressure = hexToColor(request->getParam("color_press")->value());
    request->redirect("/");
    this->config->serialize(preferences);
}

void Handler::onBlink(AsyncWebServerRequest *request) {
    if (request->hasParam("enabled")) {
        this->config->blinkingEnabled = request->getParam("enabled")->value().equals("on");
    }
    if (request->hasParam("brightness")) {
        this->config->brightness = request->getParam("brightness")->value().toInt();
    }
    if (request->hasParam("show_seconds")) {
        this->config->displaySeconds = request->getParam("show_seconds")->value().equals("on");
    }
    request->redirect("/");
    this->config->serialize(preferences);
}

void Handler::onCustom(AsyncWebServerRequest *request) {
    int duration = request->getParam("duration")->value().toInt();
    int progress = request->getParam("progress")->value().toInt();
    this->tidile->displayCustom(progress, CRGB::Aquamarine, duration);
    request->redirect("/");
}

void Handler::onIndex(AsyncWebServerRequest *request) {
    String html = index_html;
    html.replace(COLORHOURKEYWORD, colorToHex(this->config->colorHours));
    html.replace(COLORMINUTEKEYWORD, colorToHex(this->config->colorMinutes));
    html.replace(BLINKINGENABLEDKEYWORD, (this->config->blinkingEnabled)? "checked" : "");
    html.replace(BRIGHTNESSKEYWORD, String(this->config->brightness));
    html.replace(COLORTEMPERATUREKEYWORD, colorToHex(this->config->colorTemperature));
    html.replace(COLORPRESSUREKEYWORD, colorToHex(this->config->colorPressure));
    html.replace(SHOWSECONDSKEYWORD, (this->config->displaySeconds)? "checked" : "");
    html.replace(NIGHTTIMESTARTKEYWORD, timeIntToTimeString(this->config->nightTimeBegin));
    html.replace(NIGHTTIMEENDKEYWORD, timeIntToTimeString(this->config->nightTimeEnd));
    request->send(200, "text/html", html);
}

void Handler::onNightTime(AsyncWebServerRequest *request){
    if (request->hasParam("begin_time")) {
        this->config->nightTimeBegin = timeStringToTimeInt(request->getParam("begin_time")->value());
    }
    if (request->hasParam("end_time")) {
        this->config->nightTimeEnd = timeStringToTimeInt(request->getParam("end_time")->value());
    }
}

String Handler::timeIntToTimeString(int timeInt){
    String time = String(timeInt);
    return time.substring(0, 2) + ":" + time.substring(2, 4);
}

int Handler::timeStringToTimeInt(String timeString){
    timeString.remove(2,1);
    return timeString.toInt();
}

// Thank you stack overflow!
Color Handler::hexToColor(String input) {
    long rgb = strtol(input.c_str() + 1, 0, 16); // parse as Hex, skipping the leading '#'
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;
    return Color(r, g, b);
}

String Handler::colorToHex(Color color){
    String red = String(color.red, HEX);
    if(red.length() == 1) red = "0" + red;
    String green = String(color.green, HEX);
    if(green.length() == 1) green = "0" + green;
    String blue = String(color.blue, HEX);
    if(blue.length() == 1) blue = "0" + blue;
    return "#" + red + green + blue;
}