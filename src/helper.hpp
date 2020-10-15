#pragma once
#include <Arduino.h>
#include <time.h>
#include <WiFi.h>
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
	int hmsToTimeInt(ClockTime time);
	ClockTime getTime();
    String getDateTimeToString();
    bool isNightTime(ClockConfig configuration, ClockTime time);
    void setupWiFi();
    ///converts 6 to "06", 0 -> "00", 16 -> "16"
    String digitToTwoCharsDigit(int digit);
    void resetOverwriteNightTimeIfLegit(ClockConfig configuration, ClockTime time);
private:
    int timeTries = 0;
};

extern HelperClass Helper;