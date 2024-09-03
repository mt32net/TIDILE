#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "ClockConfig.hpp"
#include "../config/config_includes.hpp"
#include "time.hpp"
#include "topic.hpp"

struct NightTimeTopic: public Topic
{
    bool enabled = true;
    int begin = 2300; // 2300 -> 23:00
    int end = 600;    // 600 -> 6:00

    void deserializeFromJSON(JsonDocument &doc) override
    {
        enabled = doc[JSON_NAME_NIGHT_TIME_ENABLED].as<bool>();
        begin = doc[JSON_NAME_NIGHT_TIME_BEGIN].as<int>();
        end = doc[JSON_NAME_NIGHT_TIME_END].as<int>();
    }

    void serializeToJson(JsonDocument &doc) override {
        doc[JSON_NAME_NIGHT_TIME_ENABLED] = enabled;
        doc[JSON_NAME_NIGHT_TIME_BEGIN] = timeIntToTimeString(begin);
        doc[JSON_NAME_NIGHT_TIME_END] = timeIntToTimeString(end);
    }

    void saveToConfig(ClockConfig *config) override
    {
        config->nightTimeEnabled = enabled;
        config->nightTimeBegin = begin;
        config->nightTimeEnd = end;
    }

    void loadFromConfig(ClockConfig *config) override
    {
        enabled = config->nightTimeEnabled;
        begin = config->nightTimeBegin;
        end = config->nightTimeEnd;
    }
};
