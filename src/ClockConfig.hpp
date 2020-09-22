#pragma once
#include <Arduino.h>
#include <FastLED.h>
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

  Color(byte r, byte g, byte b){
    red = r;
    green = g;
    blue = b;
  }

  Color(){

  }

  ///deserializes this Color from startPosition into this object
  ///@param startPosAdress the start of the memory location
  void deserialize(int* startPosAdress)
  {
    #if defined(useEEPROM)
    red = (byte) EEPROM.read(*startPosAdress);
    green = (byte) EEPROM.read(*startPosAdress+1);
    blue = (byte) EEPROM.read(*startPosAdress+2);
    #endif
    (*startPosAdress) += 3;
  }

  ///serializes this Color object into the given storage container
  ///@param startPos is the given adress location the object is going to be saved to
  void serialize(int* startPos)
  {
    #if defined(useEEPROM)
    EEPROM.write(*startPos, (byte) red);
    EEPROM.write(*startPos+1, (byte) green);
    EEPROM.write(*startPos+2, (byte) blue);
    #endif

    (*startPos) += 3;
  }

  CRGB toCRGB(){
    return CRGB(red, green, blue);
  }
};

struct ClockConfig
{
  bool displaySeconds = false;
  Color colorHours = Color();
  Color colorMinutes = Color();
  Color colorSeconds = Color();
  Color colorHumidity = Color();
  Color colorTemperature = Color();
  Color colorPressure = Color();
  bool nightTimeLight = true;
  uint16_t displayOffBegin = 0; //(13:30 -> 1330, 06:24 -> 624)
  uint16_t displayOffEnd = 600;
  uint8_t brightness = 100;

  ///deserializes this ClockConfig from startPosition into this object
  ///@param startPos the start of the memory location
  void deserialize(int* startPos)
  {
    #if defined(useEEPROM)
      displaySeconds = (bool) EEPROM.read(*startPos);
      (*startPos) += 1;
      colorHours.deserialize(startPos);
      colorMinutes.deserialize(startPos);
      colorSeconds.deserialize(startPos);

      colorHumidity.deserialize(startPos);
      colorTemperature.deserialize(startPos);
      colorPressure.deserialize(startPos);

	  nightTimeLight = EEPROM.read(*startPos);
      (*startPos) += 1;
	  displayOffBegin = EEPROM.read(*startPos);
      (*startPos) += 1;
	  displayOffEnd = EEPROM.read(*startPos);
      (*startPos) += 1;
	  brightness = EEPROM.read(*startPos);

#endif
  }

  ///serializes this ClockConfig object into the given storage container
  ///@param startPos is the given adress location the object is going to be saved to
  void serialize(int* startPos)
  {
    #if defined(useEEPROM)
      EEPROM.write(*startPos, displaySeconds);
      (*startPos) += 1;

      colorHours.serialize(startPos);
      colorMinutes.serialize(startPos);
      colorSeconds.serialize(startPos);

      colorHumidity.serialize(startPos);
      colorTemperature.serialize(startPos);
      colorPressure.serialize(startPos);

      EEPROM.write(*startPos, nightTimeLight);
      (*startPos) += 1;
      EEPROM.write(*startPos, displayOffBegin);
      (*startPos) += 1;
      EEPROM.write(*startPos, displayOffEnd);
      (*startPos) += 1;
      EEPROM.write(*startPos, brightness);
    #endif
  }
};