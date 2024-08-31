#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "ClockConfig.hpp"
#include "../config/config_includes.hpp"

struct General
{
    bool showSeconds = true;
    bool format24H = true;
    uint16_t brightness = 10;
    uint16_t lightSensorInfluence = 50;
    // int ledCount = LED_COUNT;
    bool reverseDirection = false;

    void deserializeFromJSON(JsonDocument &doc)
    {
        showSeconds = doc[JSON_NAME_SHOW_SECONDS] | true;
        format24H = doc[JSON_NAME_24H_FORMAT] | true;
        brightness = doc[JSON_NAME_BRIGHTNESS] | 10;
        lightSensorInfluence = doc[JSON_NAME_LIGHT_SENSOR_INFLUENCE] | 50;
        reverseDirection = doc[JSON_NAME_REVERSE_DIRECTION] | false;
        // ledCount = doc[JSON_NAME_LED_COUNT].as<int>();
    }

    void serializeToJson(JsonDocument &doc) {
        doc[JSON_NAME_SHOW_SECONDS] = showSeconds;
        doc[JSON_NAME_24H_FORMAT] = format24H;
        doc[JSON_NAME_BRIGHTNESS] = brightness;
        doc[JSON_NAME_LIGHT_SENSOR_INFLUENCE] = lightSensorInfluence;
        doc[JSON_NAME_REVERSE_DIRECTION] = reverseDirection;
        // doc[JSON_NAME_LED_COUNT] = ledCount;
    }

    void saveToConfig(ClockConfig *config)
    {
        config->displaySeconds = showSeconds;
        config->format = format24H ? Format_24H : Format_12H;
        config->brightness = brightness;
        config->lightInfluence = lightSensorInfluence;
        config->reverseDirection = reverseDirection;
        // config->ledCount = ledCount;
    }

    void loadFromConfig(ClockConfig *config)
    {
        showSeconds = config->displaySeconds;
        format24H = config->format == Format_24H;
        brightness = config->brightness;
        lightSensorInfluence = config->lightInfluence;
        reverseDirection = config->reverseDirection;
        // ledCount = config->ledCount;
    }
};