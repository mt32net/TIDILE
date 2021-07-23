#include "RequestHandler.hpp"
#include "html/index_file.hpp"
#include "html/style.hpp"
#include "TIDILE.hpp"
#include "helper/color.hpp"
#include "helper/time.hpp"

RequestHandler::RequestHandler()
{
}

void RequestHandler::setup(ClockConfig *config, TIDILE *tidile, Preferences *preferences)
{
    this->config = config;
    this->tidile = tidile;
    this->preferences = preferences;
}

void RequestHandler::onColors(AsyncWebServerRequest *request)
{
    this->config->colorMinutes = hexToColor(request->getParam("color_min")->value());
    this->config->colorHours = hexToColor(request->getParam("color_hour")->value());
    this->config->colorSeconds = hexToColor(request->getParam("color_sec")->value());
    this->config->dimmSeconds = false;
    if (request->hasParam("dimm_seconds"))
    {
        this->config->dimmSeconds = request->getParam("dimm_seconds")->value().equals("on");
    }
    request->redirect("/");
    this->config->serialize(preferences);
}

void RequestHandler::onEnvColors(AsyncWebServerRequest *request)
{
    this->config->colorTemperature = hexToColor(request->getParam("color_temp")->value());
    this->config->colorPressure = hexToColor(request->getParam("color_press")->value());
    request->redirect("/");
    this->config->serialize(preferences);
}

void RequestHandler::onManual(AsyncWebServerRequest *request)
{
    ClockTime time = getTime();
    time.seconds = time.seconds + request->getParam("last")->value().toInt();
    tidile->displaCustom(hexToColor(request->getParam("color")->value()), time);
    request->redirect("/");
}

void RequestHandler::onLamp(AsyncWebServerRequest *request)
{
    tidile->clockMode = (!tidile->clockMode);
    tidile->displaCustom(hexToColor(request->getParam("color")->value()), getTime());
    request->redirect("/");
}

void RequestHandler::onOther(AsyncWebServerRequest *request)
{
    this->config->displaySeconds = false;
    this->config->format = ClockFormat::Format_12H;
    if (request->hasParam("brightness"))
    {
        this->config->brightness = request->getParam("brightness")->value().toInt();
    }
    if (request->hasParam("influence"))
    {
        this->config->lightInfluence = request->getParam("influence")->value().toInt();
    }
    if (request->hasParam("show_seconds"))
    {
        this->config->displaySeconds = request->getParam("show_seconds")->value().equals("on");
    }
    if (request->hasParam("format"))
    {
        this->config->format = ClockFormat::Format_24H;
    }
    request->redirect("/");
    this->config->serialize(preferences);
}

void RequestHandler::onIndex(AsyncWebServerRequest *request)
{
    String html = index_html;
    html.replace(COLORHOURKEYWORD, colorToHex(this->config->colorHours));
    html.replace(COLORMINUTEKEYWORD, colorToHex(this->config->colorMinutes));
    html.replace(COLORSECONDSKEYWORD, colorToHex(this->config->colorSeconds));
    html.replace(DIMMSECONDSKEYWORD, (this->config->dimmSeconds) ? "checked" : "");
    html.replace(BRIGHTNESSKEYWORD, String(this->config->brightness));
    html.replace(COLORTEMPERATUREKEYWORD, colorToHex(this->config->colorTemperature));
    html.replace(COLORPRESSUREKEYWORD, colorToHex(this->config->colorPressure));
    html.replace(SHOWSECONDSKEYWORD, (this->config->displaySeconds) ? "checked" : "");
    html.replace(NIGHTTIMESTARTKEYWORD, timeIntToTimeString(this->config->nightTimeBegin));
    html.replace(NIGHTTIMEENDKEYWORD, timeIntToTimeString(this->config->nightTimeEnd));
    html.replace(NIGHTTIMEENABLEDKEYWORD, (this->config->nightTimeLight) ? "checked" : "");
    html.replace(INFLUENCEKEYWORD, String(this->config->lightInfluence));
    html.replace(CURRENTTIMEKEYWORD, getDateTimeToString());
    html.replace(CLOCKFORMAT24HKEYWORD, (this->config->format == ClockFormat::Format_24H) ? "checked" : "");

    request->send(200, "text/html", html);
}

void RequestHandler::onNightTime(AsyncWebServerRequest *request)
{
    if (request->hasParam("settings"))
    {
        this->config->nightTimeLight = false;
        if (request->hasParam("begin_time"))
        {
            this->config->nightTimeBegin = timeStringToTimeInt(request->getParam("begin_time")->value());
        }
        if (request->hasParam("end_time"))
        {
            this->config->nightTimeEnd = timeStringToTimeInt(request->getParam("end_time")->value());
        }
        if (request->hasParam("time_enabled"))
        {
            this->config->nightTimeLight = request->getParam("time_enabled")->value().equals("on");
        }
    }
    if (request->hasParam("nightTimeTilMorning"))
    {
        this->config->tempOverwriteNightTime = true;
    }
    request->redirect("/");
    this->config->serialize(preferences);
}

void RequestHandler::onStyleSheet(AsyncWebServerRequest *request)
{
    String html = style_css;
    html.replace(COLORHOURKEYWORD, colorToHex(this->config->colorHours));
    html.replace(COLORMINUTEKEYWORD, colorToHex(this->config->colorMinutes));
    html.replace(COLORSECONDSKEYWORD, colorToHex(this->config->colorSeconds));
    ClockTime time = getTime();
    html.replace(MINUTESKEYWORD, String(time.minutes));
    html.replace(HOURSKEYWORD, String(time.hours));
    html.replace(SECONDSKEYWORD, String(time.seconds));
    request->send(200, "text/css", html);
}