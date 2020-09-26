#pragma once
#include <Arduino.h>
#include <time.h>
#include "ClockInfo.hpp"
#include "definements.hpp"
#include "ClockConfig.hpp"

class HelperClass
{
public:
    HelperClass(void);
    Color hexToColor(String input);
    String colorToHex(Color color);
    String timeIntToTimeString(int timeInt);
    int timeStringToTimeInt(String timeString);
    ClockTime getTime();
private:
    int timeTries = 0;
};

extern HelperClass Helper;