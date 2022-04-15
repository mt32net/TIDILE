#include "./RequestHandler.hpp"
#include "../TIDILE.hpp"
#include "helper/color.hpp"
#include "helper/time.hpp"
#include "config/config_includes.hpp"
#include "SPIFFS.h"

#define COLORHOURKEYWORD "'colorHourKeyword'"
#define COLORMINUTEKEYWORD "'colorMinuteKeyword'"
#define COLORSECONDSKEYWORD "'colorSecondKeyword'"
#define DIMMSECONDSKEYWORD "'dimmSecondsKeyword'"
#define BRIGHTNESSKEYWORD "'brightnessKeyword'"
#define COLORTEMPERATUREKEYWORD "'colorTemperatureKeyword'"
#define COLORPRESSUREKEYWORD "'colorPressureKeyword'"
#define SHOWSECONDSKEYWORD "'secondsEnabledKeyword'"
#define NIGHTTIMESTARTKEYWORD "'nighttimeStartKeyword'"
#define NIGHTTIMEENDKEYWORD "'nighttimeEndKeyword'"
#define NIGHTTIMEENABLEDKEYWORD "'nightTimeEnabledKeyword'"
#define INFLUENCEKEYWORD "'envLightInfluence'"
#define CURRENTTIMEKEYWORD "'currentTimeKeyword'"
#define MINUTESKEYWORD "'minutesKeyword'"
#define HOURSKEYWORD "'hoursKeyword'"
#define SECONDSKEYWORD "'secondsKeyword'"
#define CLOCKFORMAT24HKEYWORD "'24HKeyword'"

RequestHandler::RequestHandler()
{
}

void RequestHandler::setup(ClockConfig *config, TIDILE *tidile)
{
    this->config = config;
    this->tidile = tidile;
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
    this->tidile->flushConfig();
    // this->config->serialize(preferences);
}

void RequestHandler::onEnvColors(AsyncWebServerRequest *request)
{
    this->config->colorTemperature = hexToColor(request->getParam("color_temp")->value());
    this->config->colorPressure = hexToColor(request->getParam("color_press")->value());
    request->redirect("/");
    this->tidile->flushConfig();
    // this->config->serialize(preferences);
}

void RequestHandler::onManual(AsyncWebServerRequest *request)
{
    ClockTime time;
    getTime(&time);
    time.seconds = time.seconds + request->getParam("last")->value().toInt();
    tidile->displaCustom(hexToColor(request->getParam("color")->value()), time);
    request->redirect("/");
    this->tidile->flushConfig();
}

void RequestHandler::onLamp(AsyncWebServerRequest *request)
{
    tidile->clockMode = (!tidile->clockMode);
    ClockTime time;
    // TODO necessry to get correct time here?
    getTime(&time);
    tidile->displaCustom(hexToColor(request->getParam("color")->value()), time);
    request->redirect("/");
    this->tidile->flushConfig();
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
    // this->config->serialize(preferences);
    this->tidile->flushConfig();
}

void RequestHandler::onIndex(AsyncWebServerRequest *request)
{
    String html = SPIFFS.open("/index.html").readString();
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
    html.replace(NIGHTTIMEENABLEDKEYWORD, (this->config->nightTimeEnabled) ? "checked" : "");
    html.replace(INFLUENCEKEYWORD, String(this->config->lightInfluence));
    // Do not use getTime here -> timeout when server is not responding -> crash
    // html.replace(CURRENTTIMEKEYWORD, getDateTimeToString());
    html.replace(CLOCKFORMAT24HKEYWORD, (this->config->format == ClockFormat::Format_24H) ? "checked" : "");

    request->send(200, "text/html", html);
}

void RequestHandler::onNightTime(AsyncWebServerRequest *request)
{
    if (request->hasParam("settings"))
    {
        this->config->nightTimeEnabled = false;
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
            this->config->nightTimeEnabled = request->getParam("time_enabled")->value().equals("on");
        }
    }
    if (request->hasParam("nightTimeTilMorning"))
    {
        this->config->tempOverwriteNightTime = true;
    }
    request->redirect("/");
    // this->config->serialize(preferences);
    this->tidile->flushConfig();
}

void RequestHandler::onStyleSheet(AsyncWebServerRequest *request)
{
    String html = SPIFFS.open("/styles.css").readString();
    html.replace(COLORHOURKEYWORD, colorToHex(this->config->colorHours));
    html.replace(COLORMINUTEKEYWORD, colorToHex(this->config->colorMinutes));
    html.replace(COLORSECONDSKEYWORD, colorToHex(this->config->colorSeconds));
    // Do not use getTime here -> timeout when server is not responding -> crash
    // ClockTime time;
    // getTime(&time);
    // html.replace(MINUTESKEYWORD, String(time.minutes));
    // html.replace(HOURSKEYWORD, String(time.hours));
    // html.replace(SECONDSKEYWORD, String(time.seconds));
    request->send(200, "text/css", html);
}