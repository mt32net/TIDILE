#pragma once
#include "Arduino.h"
#include <FastLED.h>
#include "config/config_includes.hpp"
#include <Preferences.h>
#include "helper/color.hpp"
#include "ArduinoJson.h"

/**
 * @brief CHosen clock format
 */
enum ClockFormat
{
  Format_24H = 23,
  Format_12H = 11
};

// TODO The right one pls
#define ENTRY_COUNTS 10

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
  int ledCount = LED_COUNT;

  void deserialize(JsonObject *json)
  {
    JsonObject hours = (*json).getMember("hours");
    colorHours.deserialize(&hours);
    JsonObject minutes = (*json).getMember("minutes");
    colorMinutes.deserialize(&minutes);
    JsonObject seconds = (*json).getMember("seconds");
    colorSeconds.deserialize(&seconds);

    displaySeconds = (*json)["displaySeconds"];
    dimmSeconds = (*json)["dimmSeconds"];

    // colorHumidity.deserialize(preferences, "humid");
    // colorTemperature.deserialize(preferences, "temper");
    // colorPressure.deserialize(preferences, "pressure");

    nightTimeEnabled = (*json)["nightTimeEnabled"];
    nightTimeBegin = (*json)["nightTimeBegin"];
    nightTimeEnd = (*json)["nightTimeEnd"];
    brightness = (*json)["brightness"];
    lightInfluence = (*json)["lightInfluence"];
    format = (*json)["format"];

    // format = (preferences->getInt("format") == ClockFormat::Format_24H) ? ClockFormat::Format_24H : ClockFormat::Format_12H;

    // ledCount = json["ledCount"];
  }

  void serialize(JsonObject *json)
  {

    // preferences->begin("prefs", false);
    // preferences->putBool("valid", true);
    (*json)["valid"] = true;
    // preferences->putBool("displSecs", displaySeconds);
    (*json)["displaySeconds"] = displaySeconds;
    // preferences->putBool("dimmSecs", dimmSeconds);
    (*json)["dimmSeconds"] = dimmSeconds;

    // Make these things serialize!

    JsonObject hours = (*json).getOrAddMember("hours").to<JsonObject>();
    colorHours.serialize(&hours);
    JsonObject minutes = (*json).getOrAddMember("minutes").to<JsonObject>();
    colorMinutes.serialize(&minutes);
    JsonObject seconds = (*json).getOrAddMember("seconds").to<JsonObject>();
    colorSeconds.serialize(&seconds);
    // (*json)["humidity"] = colorHumidity.serialize();
    // (*json)["temperature"] = colorTemperature.serialize();
    // (*json)["pressure"] = colorPressure.serialize();

    // preferences->putBool("nightLight", nightTimeEnabled);
    (*json)["nightTimeEnabled"] = nightTimeEnabled;
    (*json)["nightTimeBegin"] = nightTimeBegin;
    (*json)["nightTimeEnd"] = nightTimeEnd;
    (*json)["brightness"] = brightness;
    (*json)["lightInfluence"] = lightInfluence;
    // preferences->putInt("nightTimeBegin", nightTimeBegin);
    // preferences->putInt("nightTimeEnd", nightTimeEnd);
    // preferences->putInt("brightness", brightness);
    // preferences->putInt("lightInflu", lightInfluence);

    // preferences->putInt("ledCount", ledCount);
    // (*json)["ledCount"] = ledCount;

    // preferences->putInt("format", format);
    (*json)["format"] = format;
    // preferences->end();

    // serializeJsonPretty(json, Serial);
  }
};