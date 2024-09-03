//
// Created by Tom Arlt on 03.09.24.
//

#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include "ClockConfig.hpp"

struct Topic : public TIDILE_Plugin {
    virtual void deserializeFromJSON(JsonDocument &doc) = 0;
    virtual void serializeToJson(JsonDocument &doc) = 0;
    virtual void saveToConfig(ClockConfig *config) = 0;
    virtual void loadFromConfig(ClockConfig *config) = 0;
};