#pragma once
#include "Arduino.h"
#include <FastLED.h>
#include "config/config_includes.hpp"
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
  bool displaySeconds = DEFAULT_DISPLAY_SECONDS;
  Color colorHours = Color(255, 0, 0);
  Color colorMinutes = Color(0, 255, 0);
  Color colorSeconds = Color(0, 0, 255);
  Color colorHumidity = Color();
  Color colorTemperature = Color();
  Color colorPressure = Color();
  bool nightTimeEnabled = DEFAULT_NIGHT_TIME_ENABLED;
  uint16_t lightInfluence = DEFAULT_LIGHT_INFLUENCE;
  uint16_t nightTimeBegin = DEFAULT_NIGHT_TIME_BEGIN; //(13:30 -> 1330, 06:24 -> 624)
  uint16_t nightTimeEnd = DEFAULT_NIGHT_TIME_END;
  uint8_t brightness = DEFAULT_BRIGHTNESS;
  bool dimmSeconds = DEFAULT_DIMM_SECONDS;
  bool tempOverwriteNightTime = false;
  ClockFormat format = ClockFormat::Format_24H;
  int ledCount = DEFAULT_LED_COUNT;

  ///deserializes this ClockConfig from startPosition into this object
  ///@param preferences is the preference object
  void deserialize(Preferences *preferences)
  {
    preferences->begin("prefs");
    if (!preferences->getBool("valid", false))
    {
      serialize(preferences);
      return;
    }
    displaySeconds = preferences->getBool("displSecs", DEFAULT_DISPLAY_SECONDS);
    colorHours.deserialize(preferences, "hours");
    colorMinutes.deserialize(preferences, "minutes");
    colorSeconds.deserialize(preferences, "seconds");
    dimmSeconds = preferences->getBool("dimmSecs", DEFAULT_DIMM_SECONDS);

    colorHumidity.deserialize(preferences, "humid");
    colorTemperature.deserialize(preferences, "temper");
    colorPressure.deserialize(preferences, "pressure");

    nightTimeEnabled = preferences->getBool("nightLight", DEFAULT_NIGHT_TIME_ENABLED);
    nightTimeBegin = preferences->getInt("nightTimeBegin", DEFAULT_NIGHT_TIME_BEGIN);
    nightTimeEnd = preferences->getInt("nightTimeEnd", DEFAULT_NIGHT_TIME_END);
    brightness = preferences->getInt("brightness", DEFAULT_BRIGHTNESS);
    lightInfluence = preferences->getInt("lightInflu", DEFAULT_LIGHT_INFLUENCE);

    format = (preferences->getInt("format") == ClockFormat::Format_24H) ? ClockFormat::Format_24H : ClockFormat::Format_12H;

    ledCount = preferences->getInt("ledCount", DEFAULT_LED_COUNT);

    preferences->end();
  }

  ///serializes this ClockConfig object into the given storage container
  ///@param preferences is the preference object
  void serialize(Preferences *preferences)
  {
    preferences->begin("prefs", false);
    preferences->putBool("valid", true);

    preferences->putBool("displSecs", displaySeconds);

    colorHours.serialize(preferences, "hours");
    colorMinutes.serialize(preferences, "minutes");
    colorSeconds.serialize(preferences, "seconds");
    preferences->putBool("dimmSecs", dimmSeconds);

    colorHumidity.serialize(preferences, "humid");
    colorTemperature.serialize(preferences, "temper");
    colorPressure.serialize(preferences, "pressure");

    preferences->putBool("nightLight", nightTimeEnabled);
    preferences->putInt("nightTimeBegin", nightTimeBegin);
    preferences->putInt("nightTimeEnd", nightTimeEnd);
    preferences->putInt("brightness", brightness);
    preferences->putInt("lightInflu", lightInfluence);

    preferences->putInt("ledCount", ledCount);

    preferences->putInt("format", format);
    preferences->end();
  }
};