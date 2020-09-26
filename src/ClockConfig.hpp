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
    red = preferences->getInt((name + String("_r")).c_str());
    green = preferences->getInt((name + String("_g")).c_str());
    blue = preferences->getInt((name + String("_b")).c_str());
  }

  ///serializes this Color object into the given storage container
  ///@param preferences is the preference object
  void serialize(Preferences* preferences, String name)
  {
    preferences->putInt((name + String("_r")).c_str(), red);
    preferences->putInt((name + String("_g")).c_str(), green);
    preferences->putInt((name + String("_b")).c_str(), blue);
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
    preferences->begin("tidilePrefs");
    displaySeconds = preferences->getBool("displSecs");
    colorHours.deserialize(preferences, "hours");
    colorMinutes.deserialize(preferences, "minutes");
    colorSeconds.deserialize(preferences, "seconds");

    colorHumidity.deserialize(preferences, "humid");
    colorTemperature.deserialize(preferences, "temper");
    colorPressure.deserialize(preferences, "pressure");

    nightTimeLight = preferences->getBool("nightLight");
    displayOffBegin = preferences->getInt("displOffBegin");
    displayOffEnd = preferences->getInt("displOffEnd");
    brightness = preferences->getInt("brightness");
    preferences->end();
  }

  ///serializes this ClockConfig object into the given storage container
  ///@param preferences is the preference object
  void serialize(Preferences* preferences)
  {
    preferences->begin("prefs", false);
    preferences->putBool("displSecs", displaySeconds);

    colorHours.serialize(preferences, "hours");
    colorMinutes.serialize(preferences, "minutes");
    colorSeconds.serialize(preferences, "seconds");

    colorHumidity.serialize(preferences, "humid");
    colorTemperature.serialize(preferences, "temper");
    colorPressure.serialize(preferences, "pressure");

    preferences->putBool("nightLight", nightTimeLight);
    preferences->putInt("displOffBegin", displayOffBegin);
    preferences->putInt("displOffEnd", displayOffEnd);
    preferences->putInt("brightness", brightness);
    preferences->end();
  }
};