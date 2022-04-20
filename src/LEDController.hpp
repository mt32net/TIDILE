#pragma once

#include <FastLED.h>
#include "helper/color.hpp"
#include "ClockConfig.hpp"

class LEDController {
public:
    LEDController() {};
    LEDController(short numberLEDs, CRGB * leds, ClockConfig * config, short numberZones);
    void setZone(short index, Color color);
    void setBrightness(byte brightness);
private:
    ClockConfig * config;
    CRGB * leds;
    short numberZones;
    short numberLeds;
    double multiplier;

    short mapIndex(short zoneIndex);
};