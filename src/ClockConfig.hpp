#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "definements.hpp"
#include <Preferences.h>
#include "helper/color.hpp"

/**
 * @brief CHosen clock format
 */
enum ClockFormat
{
  Format_24H = 23,
  Format_12H = 11
};

/**
 * @brief Configuration class for TIDILE, will be saved and restored out of permanent storage at each reboot
 * 
 */
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
  int lightInfluence = 50;
  uint16_t nightTimeBegin = 0; //(13:30 -> 1330, 06:24 -> 624)
  uint16_t nightTimeEnd = 600;
  uint8_t brightness = 60;
  bool dimmSeconds = false;
  bool tempOverwriteNightTime = false;
  ClockFormat format = ClockFormat::Format_24H;

  ///deserializes this ClockConfig from startPosition into this object
  ///@param preferences is the preference object
  void deserialize(Preferences *preferences)
  {
    preferences->begin("prefs");
    displaySeconds = preferences->getBool("displSecs");
    colorHours.deserialize(preferences, "hours");
    colorMinutes.deserialize(preferences, "minutes");
    colorSeconds.deserialize(preferences, "seconds");
    dimmSeconds = preferences->getBool("dimmSecs");

    colorHumidity.deserialize(preferences, "humid");
    colorTemperature.deserialize(preferences, "temper");
    colorPressure.deserialize(preferences, "pressure");

    nightTimeLight = preferences->getBool("nightLight");
    nightTimeBegin = preferences->getInt("nightTimeBegin");
    nightTimeEnd = preferences->getInt("nightTimeEnd");
    brightness = preferences->getInt("brightness");
    lightInfluence = preferences->getInt("lightInflu");

    format = (preferences->getInt("format") == ClockFormat::Format_24H) ? ClockFormat::Format_24H : ClockFormat::Format_12H;
    preferences->end();
  }

  ///serializes this ClockConfig object into the given storage container
  ///@param preferences is the preference object
  void serialize(Preferences *preferences)
  {
    preferences->begin("prefs", false);
    preferences->putBool("displSecs", displaySeconds);

    colorHours.serialize(preferences, "hours");
    colorMinutes.serialize(preferences, "minutes");
    colorSeconds.serialize(preferences, "seconds");
    preferences->putBool("dimmSecs", dimmSeconds);

    colorHumidity.serialize(preferences, "humid");
    colorTemperature.serialize(preferences, "temper");
    colorPressure.serialize(preferences, "pressure");

    preferences->putBool("nightLight", nightTimeLight);
    preferences->putInt("nightTimeBegin", nightTimeBegin);
    preferences->putInt("nightTimeEnd", nightTimeEnd);
    preferences->putInt("brightness", brightness);
    preferences->putInt("lightInflu", lightInfluence);

    preferences->putInt("format", format);
    preferences->end();
  }
};