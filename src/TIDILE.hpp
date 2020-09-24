#pragma once
#include <FastLED.h>
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"
#include "definements.hpp"
#include "config.h"

class TIDILE
{
private:
    CRGB *leds;
    ClockConfig *configuration;
    int numberLEDs;
    int mapToLEDs(int value, int max);
    void clear();
public:
    TIDILE();
    void setup(CRGB leds[NUM_LEDS], int numberLEDs, ClockConfig *configuration);
    void displayTime(ClockTime time);
    void displayEnv(ClockEnv *env);
    ///@param progress value between 0 and 99
    ///@param duration in ms
    void displayCustom(int progress, CRGB color, int duration);
};