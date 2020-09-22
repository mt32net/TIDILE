#pragma once

#include "ClockTime.h"
#include <FastLED.h>

class Display {
    
    public:
        void displayTime(CRGB * leds, ClockTime * time);
};
