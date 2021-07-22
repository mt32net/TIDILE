#pragma once
#include <Arduino.h>
#include "definements.hpp"
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"

String timeIntToTimeString(int timeInt);
int timeStringToTimeInt(String timeString);
int hmsToTimeInt(ClockTime time);
ClockTime getTime();
String getDateTimeToString();
bool isNightTime(ClockConfig configuration, ClockTime time);
void resetOverwriteNightTimeIfLegit(ClockConfig configuration, ClockTime time);
