#include "Handler.hpp"
#include "html/index_file.hpp"
#include "html/style.hpp"
#include "TIDILE.hpp"

Handler::Handler()
{
}

void Handler::setup(ClockConfig *config, TIDILE *tidile, Preferences *preferences)
{
    this->config = config;
    this->tidile = tidile;
    this->preferences = preferences;
}

void Handler::onColors(AsyncWebServerRequest *request)
{
    this->config->colorMinutes = Helper.hexToColor(request->getParam("color_min")->value());
    this->config->colorHours = Helper.hexToColor(request->getParam("color_hour")->value());
    this->config->colorSeconds = Helper.hexToColor(request->getParam("color_sec")->value());
    this->config->dimmSeconds = false;
    if (request->hasParam("dimm_seconds"))
    {
        this->config->dimmSeconds = request->getParam("dimm_seconds")->value().equals("on");
    }
    request->redirect("/");
    this->config->serialize(preferences);
}

void Handler::onEnvColors(AsyncWebServerRequest *request)
{
    this->config->colorTemperature = Helper.hexToColor(request->getParam("color_temp")->value());
    this->config->colorPressure = Helper.hexToColor(request->getParam("color_press")->value());
    request->redirect("/");
    this->config->serialize(preferences);
}

void Handler::onManual(AsyncWebServerRequest *request)
{   
    ClockTime time = Helper.getTime();
    time.seconds = time.seconds + 10;
    tidile->displaCustom(Helper.hexToColor(request->getParam("color")->value()), time);
    request->redirect("/");
}

void Handler::onOther(AsyncWebServerRequest *request)
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
    if(request->hasParam("format")){
        this->config->format = ClockFormat::Format_24H;
    }
    request->redirect("/");
    this->config->serialize(preferences);
}

void Handler::onIndex(AsyncWebServerRequest *request)
{
    String html = index_html;
    html.replace(COLORHOURKEYWORD, Helper.colorToHex(this->config->colorHours));
    html.replace(COLORMINUTEKEYWORD, Helper.colorToHex(this->config->colorMinutes));
    html.replace(COLORSECONDSKEYWORD, Helper.colorToHex(this->config->colorSeconds));
    html.replace(DIMMSECONDSKEYWORD, (this->config->dimmSeconds) ? "checked" : "");
    html.replace(BRIGHTNESSKEYWORD, String(this->config->brightness));
    html.replace(COLORTEMPERATUREKEYWORD, Helper.colorToHex(this->config->colorTemperature));
    html.replace(COLORPRESSUREKEYWORD, Helper.colorToHex(this->config->colorPressure));
    html.replace(SHOWSECONDSKEYWORD, (this->config->displaySeconds) ? "checked" : "");
    html.replace(NIGHTTIMESTARTKEYWORD, Helper.timeIntToTimeString(this->config->nightTimeBegin));
    html.replace(NIGHTTIMEENDKEYWORD, Helper.timeIntToTimeString(this->config->nightTimeEnd));
    html.replace(NIGHTTIMEENABLEDKEYWORD, (this->config->nightTimeLight) ? "checked" : "");
    html.replace(INFLUENCEKEYWORD, String(this->config->lightInfluence));
    html.replace(CURRENTTIMEKEYWORD, Helper.getDateTimeToString());
    html.replace(CLOCKFORMAT24HKEYWORD, (this->config->format == ClockFormat::Format_24H) ? "checked" : "");
    
    request->send(200, "text/html", html);
}

void Handler::onNightTime(AsyncWebServerRequest *request)
{
    if (request->hasParam("settings"))
    {
        this->config->nightTimeLight = false;
        if (request->hasParam("begin_time"))
        {
            this->config->nightTimeBegin = Helper.timeStringToTimeInt(request->getParam("begin_time")->value());
        }
        if (request->hasParam("end_time"))
        {
            this->config->nightTimeEnd = Helper.timeStringToTimeInt(request->getParam("end_time")->value());
        }
        if (request->hasParam("time_enabled"))
        {
            this->config->nightTimeLight = request->getParam("time_enabled")->value().equals("on");
        }
    }
    if(request->hasParam("nightTimeTilMorning")){
        this->config->tempOverwriteNightTime = true;
    }
    request->redirect("/");
    this->config->serialize(preferences);
}

void Handler::onStyleSheet(AsyncWebServerRequest *request){
    String html = style_css;
    html.replace(COLORHOURKEYWORD, Helper.colorToHex(this->config->colorHours));
    html.replace(COLORMINUTEKEYWORD, Helper.colorToHex(this->config->colorMinutes));
    html.replace(COLORSECONDSKEYWORD, Helper.colorToHex(this->config->colorSeconds));
    ClockTime time = Helper.getTime();
    html.replace(MINUTESKEYWORD, String(time.minutes));
    html.replace(HOURSKEYWORD, String(time.hours));
    html.replace(SECONDSKEYWORD, String(time.seconds));
    request->send(200, "text/css", html);
}