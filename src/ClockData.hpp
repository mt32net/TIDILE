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

  void deserialize(int* startPosAdress)
  {
    #if defined(useEEPROM)
    red = (byte) EEPROM.read(*startPosAdress);
    green = (byte) EEPROM.read(*startPosAdress+1);
    blue = (byte) EEPROM.read(*startPosAdress+2);
    #endif
    (*startPosAdress) += 3;
  }

  void serialize(int* startPos)
  {
    #if defined(useEEPROM)
    EEPROM.write(*startPos, (byte) red);
    EEPROM.write(*startPos+1, (byte) green);
    EEPROM.write(*startPos+2, (byte) blue);
    #endif

    (*startPos) += 3;
  }
};

struct ClockData
{
  bool displaySeconds = false;
  Color colorHours = {};
  Color colorMinutes = {};
  Color colorSeconds = {};
  Color colorHumidity = {};
  bool nightTimeLight = true;
  uint16_t displayOffBegin = 0; //(13:30 -> 1330, 06:24 -> 624)
  uint16_t displayOffEnd = 600;

  ClockData deserialize(int startPos)
  {
  }

  void serialize(ClockData data)
  {
  }
};