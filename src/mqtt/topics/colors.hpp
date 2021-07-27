#pragma once
#include <Arduino.h>
#include "../../helper/color.hpp"
#include <ArduinoJson.h>
#include "../../ClockConfig.hpp"
#include "../../config/config_includes.hpp"

struct Colors
{
    Color hours = Color(255, 0, 0);
    Color minutes = Color(0, 255, 0);
    Color seconds = Color(0, 0, 255);
    bool dimmSeconds = false;

    void deserializeFromJSON(JsonDocument &doc)
    {
        hours = hexToColor(String(doc[JSON_NAME_COLOR_HOURS].as<const char *>()));
        minutes = hexToColor(String(doc[JSON_NAME_COLOR_MINUTES].as<const char *>()));
        seconds = hexToColor(String(doc[JSON_NAME_COLOR_SECONDS].as<const char *>()));
        dimmSeconds = doc[JSON_NAME_DIMM_SECONDS].as<bool>();
    }

    void saveToConfig(ClockConfig *config)
    {
        config->colorHours = hours;
        config->colorMinutes = minutes;
        config->colorSeconds = seconds;
        config->dimmSeconds = dimmSeconds;
    }

    void loadFromConfig(ClockConfig &config)
    {
        hours = config.colorHours;
        minutes = config.colorMinutes;
        seconds = config.colorSeconds;
        dimmSeconds = config.dimmSeconds;
    }
};