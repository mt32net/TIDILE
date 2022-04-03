#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../../ClockConfig.hpp"
#include "../../config/config_includes.hpp"

struct General
{
    bool showSeconds = true;
    bool format24H = true;
    uint16_t brightness = 10;
    uint16_t lightSensorInfluence = 50;
    int ledCount = DEFAULT_LED_COUNT;

    void deserializeFromJSON(JsonDocument &doc)
    {
        showSeconds = doc[JSON_NAME_SHOW_SECONDS].as<bool>();
        format24H = doc[JSON_NAME_24H_FORMAT].as<bool>();
        brightness = doc[JSON_NAME_BRIGHTNESS].as<uint16_t>();
        lightSensorInfluence = doc[JSON_NAME_LIGHT_SENSOR_INFLUENCE].as<uint16_t>();
        ledCount = doc[JSON_NAME_LED_COUNT].as<int>();
    }

    void saveToConfig(ClockConfig *config)
    {
        config->displaySeconds = showSeconds;
        config->format = format24H ? Format_24H : Format_12H;
        config->brightness = brightness;
        config->lightInfluence = lightSensorInfluence;
        config->ledCount = ledCount;
    }

    void loadFromConfig(ClockConfig &config)
    {
        showSeconds = config.displaySeconds;
        format24H = config.format == Format_24H;
        brightness = config.brightness;
        lightSensorInfluence = config.lightInfluence;
        ledCount = config.ledCount;
    }
};