#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "definements.hpp"
#include <Preferences.h>

struct Color
{
  byte red = 0;
  byte green = 0;
  byte blue = 0;

  Color(byte r, byte g, byte b)
  {
    red = r;
    green = g;
    blue = b;
  }

  Color()
  {
  }

  ///deserializes this Color from startPosition into this object
  ///@param preferences is the preference object
  void deserialize(Preferences* preferences, String name)
  {
#if defined(useEEPROM)
    red = preferences->getInt((name + String("_red")).c_str());
    green = preferences->getInt((name + String("_green")).c_str());
    blue = preferences->getInt((name + String("_blue")).c_str());
#endif
  }

  ///serializes this Color object into the given storage container
  ///@param preferences is the preference object
  void serialize(Preferences* preferences, String name)
  {
#if defined(useEEPROM)
    preferences->putInt((name + String("_red")).c_str(), red);
    preferences->putInt((name + String("_green")).c_str(), green);
    preferences->putInt((name + String("_blue")).c_str(), blue);
#endif
  }

  CRGB toCRGB()
  {
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
  uint8_t brightness = 60;
  boolean blinkingEnabled = false;

  ///deserializes this ClockConfig from startPosition into this object
  ///@param preferences is the preference object
  void deserialize(Preferences* preferences)
  {
#if defined(useEEPROM)
    displaySeconds = preferences->getBool("displaySeconds");
    colorHours.deserialize(preferences, "hours");
    colorMinutes.deserialize(preferences, "minutes");
    colorSeconds.deserialize(preferences, "seconds");

    colorHumidity.deserialize(preferences, "humidity");
    colorTemperature.deserialize(preferences, "temperature");
    colorPressure.deserialize(preferences, "pressure");

    nightTimeLight = preferences->getBool("nightTimeLight");
    displayOffBegin = preferences->getInt("displayOffBegin");
    displayOffEnd = preferences->getInt("displayOffEnd");
    brightness = preferences->getInt("brightness");
#endif
  }

  ///serializes this ClockConfig object into the given storage container
  ///@param preferences is the preference object
  void serialize(Preferences* preferences)
  {
#if defined(useEEPROM)
    preferences->putBool("displaySeconds", displaySeconds);

    colorHours.serialize(preferences, "hours");
    colorMinutes.serialize(preferences, "minutes");
    colorSeconds.serialize(preferences, "seconds");

    colorHumidity.serialize(preferences, "humidity");
    colorTemperature.serialize(preferences, "temperature");
    colorPressure.serialize(preferences, "pressure");

    preferences->putBool("nightTimeLight", nightTimeLight);
    preferences->putInt("displayOffBegin", displayOffBegin);
    preferences->putInt("displayOffEnd", displayOffEnd);
    preferences->putInt("brightness", brightness);
#endif
  }
};