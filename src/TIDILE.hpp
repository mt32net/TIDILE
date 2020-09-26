#pragma once
#include <FastLED.h>
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"
#include "definements.hpp"
#include "config.hpp"
#include "helper.hpp"

class TIDILE
{
public:
    TIDILE();
    ///setup function
    ///@param leds the LED array wich is already registered in FastLED within the main file
    ///@param numberLEDS the number of LEDS within the array
    ///@param configuration the pointer to thee configuration object where all settings are saved
    void setup(CRGB leds[NUM_LEDS], int numberLEDs, ClockConfig *configuration);
    ///displays current time
    ClockTime displayTime();
    ///displays information about your sorrounding such as temperature, humdity and pressure. IF the sensors are connected and defined in the config file
    void displayEnv(ClockEnv env);
    ///@param progress value between 0 and 99
    ///@param duration in ms
    void displayCustom(int progress, CRGB color, int duration);
private:
    CRGB *leds;
    ClockConfig *configuration;
    int numberLEDs;
    int mapToLEDs(int value, int max);
    void clear();
    bool inTimeWindow(ClockTime time);
    
};