#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../ClockConfig.hpp"
#include "../config/config_includes.hpp"

struct NightTime
{
    bool enabled = true;
    int begin = 2300; // 2300 -> 23:00
    int end = 600;    // 600 -> 6:00

    void deserializeFromJSON(JsonDocument &doc)
    {
        enabled = doc[JSON_NAME_NIGHT_TIME_ENABLED].as<bool>();
        begin = doc[JSON_NAME_NIGHT_TIME_BEGIN].as<int>();
        end = doc[JSON_NAME_NIGHT_TIME_END].as<int>();
    }

    void serializeToJson(JsonDocument &doc) {
        doc[JSON_NAME_NIGHT_TIME_ENABLED] = enabled;
        doc[JSON_NAME_NIGHT_TIME_BEGIN] = begin;
        doc[JSON_NAME_NIGHT_TIME_END] = end;
    }

    void saveToConfig(ClockConfig *config)
    {
        config->nightTimeEnabled = enabled;
        config->nightTimeBegin = begin;
        config->nightTimeEnd = end;
    }

    void loadFromConfig(ClockConfig *config)
    {
        enabled = config->nightTimeEnabled;
        begin = config->nightTimeBegin;
        end = config->nightTimeEnd;
    }
};