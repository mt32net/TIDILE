#pragma once
#include <Arduino.h>
#include "definements.hpp"
#if defined(useSDCard)
#include <SPI.h>
#include <SD.h>
#endif
#if defined(useEEPROM)
#include <EEPROM.h>
#endif

struct Color
{
  byte red = 0;
  byte green = 0;
  byte blue = 0;

  Color deserialize(int adress)
  {
    #if defined(useEEPROM)
    red = EEPROM.read()
    #endif
    return {};
  }

  void serialize(Color color, int startPos, int *endPos)
  {
  }
};

struct ClockData
{
  bool displaySeconds = false;
  Color colorHours = {};
  Color colorMinutes = {};
  Color colorSeconds = {};

#ifdef displayHumidity
  Color humidity = {};
#endif

  ClockData deserialize(int startPos)
  {
  }

  void serialize(ClockData data)
  {
  }
};