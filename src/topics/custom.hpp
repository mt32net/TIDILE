#pragma once
#include "../config/config_includes.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "ClockConfig.hpp"
#include "time.hpp"
#include "topic.hpp"

enum ClockMode
{
    NORMAL,
    COLOR
};

struct CustomTopic : public Topic
{
    void modifyLEDs(LEDController *ledController, int numberLEDs, ClockTime time, ClockEnv env) override
    {
        if (mode == NORMAL)
            return;

        ledController->setAll(color);
    }

    void loop(ClockTime time) override
    {
        if (mode != NORMAL && unixEnd != 0 && time.unixTime >= unixEnd)
        {
            mode = NORMAL;
            unixEnd = 0;
        }
    }

    ClockMode mode = NORMAL;
    long unixEnd = 0;
    Color color = Color(0, 0, 255);
    // in format HHMM, HH: 0 - 24

    void deserializeFromJSON(JsonDocument &doc) override
    {
        String modeString = doc[JSON_NAME_CUSTOM_MODE].as<String>();
        if (modeString.equals("COLOR"))
            mode = COLOR;
        else
            mode = NORMAL;
        unixEnd = doc[JSON_NAME_CUSTOM_END].as<long>();
        color = Color::hexToColor(doc[JSON_NAME_CUSTOM_COLOR].as<String>());
    }

    void serializeToJson(JsonDocument &doc) override
    {
        switch (mode)
        {
        case NORMAL:
            doc[JSON_NAME_CUSTOM_MODE] = "NORMAL";
            break;
        case COLOR:
            doc[JSON_NAME_CUSTOM_MODE] = "COLOR";
            break;
        }

        doc[JSON_NAME_CUSTOM_COLOR] = Color::colorToHex(color);
    }

    void saveToConfig(ClockConfig *config) override
    {
    }

    void loadFromConfig(ClockConfig *config) override
    {
    }
};