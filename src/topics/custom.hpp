#pragma once
#include "../config/config_includes.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "ClockConfig.hpp"
#include "time.hpp"
#include "topic.hpp"

enum ClockMode {
    NORMAL,
    COLOR,
    TIME,
};

struct CustomTopic: public Topic
{
    void modifyLEDs(LEDController *ledController, int numberLEDs, ClockTime time, ClockEnv env) override {
        ledController->setAll(color);
    }

    bool displayEnv() override {
        return mode != NORMAL;
    }

    ClockMode mode = NORMAL;
    long unixEnd = 0;
    Color color = Color(0, 0, 255);
    // in format HHMM, HH: 0 - 24
    ClockTime customTime = ClockTime();

    void deserializeFromJSON(JsonDocument &doc) override {
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
            doc[JSON_NAME_CUSTOM_TIME]["seconds"],
            doc[JSON_NAME_CUSTOM_TIME]["minutes"],
            doc[JSON_NAME_CUSTOM_TIME]["hours"],
        };
    }

    void serializeToJson(JsonDocument &doc) override {
        switch (mode) {
            case NORMAL: doc[JSON_NAME_CUSTOM_MODE] = "NORMAL"; break;
            case COLOR: doc[JSON_NAME_CUSTOM_MODE] = "COLOR"; break;
            case TIME: doc[JSON_NAME_CUSTOM_MODE] = "TIME"; break;
       }
    }

    void saveToConfig(ClockConfig *config) override {

    }

    void loadFromConfig(ClockConfig *config) override {

    }
};