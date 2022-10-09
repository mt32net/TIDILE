#pragma once
#include "../config/config_includes.hpp"

#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "../ClockConfig.hpp"
#include "../config/config_includes.hpp"
#include "../helper/time.hpp"

enum ClockMode {
    NORMAL,
    COLOR,
    TIME,
};

struct Custom
{
    ClockMode mode = NORMAL;
    long unixEnd = 0;
    Color color = Color(0, 0, 255);
    // in format HHMM, HH: 0 - 24
    ClockTime customTime = ClockTime();

    void deserializeFromJSON(JsonDocument &doc)
    {
        String modeString = doc[JSON_NAME_CUSTOM_MODE].as<String>();
        if (modeString.equals("TIME"))
            mode = TIME;
        else if (modeString.equals("COLOR"))
            mode = COLOR;
        else
            mode = NORMAL;
        unixEnd = doc[JSON_NAME_CUSTOM_END].as<long>();
        color = Color::hexToColor(doc[JSON_NAME_CUSTOM_COLOR].as<String>());
        customTime = ClockTime {
            seconds: doc[JSON_NAME_CUSTOM_TIME]["seconds"],
            minutes: doc[JSON_NAME_CUSTOM_TIME]["minutes"],
            hours: doc[JSON_NAME_CUSTOM_TIME]["hours"],
        };
    }


    void serializeToJson(JsonDocument &doc) {
        switch (mode) {
            case NORMAL: doc[JSON_NAME_CUSTOM_MODE] = "NORMAL"; break;
            case COLOR: doc[JSON_NAME_CUSTOM_MODE] = "COLOR"; break;
            case TIME: doc[JSON_NAME_CUSTOM_MODE] = "TIME"; break;
       }
    }
};