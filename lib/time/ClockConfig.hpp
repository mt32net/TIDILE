#pragma once
#include "Arduino.h"
#include <FastLED.h>
#include <LittleFS.h>

#include <color.hpp>
#include "ArduinoJson.h"
#include <StreamUtils.h>
#include <vector>
#include <file.hpp>

#include "vectorSerialization.hpp"

/**
 * @brief CHosen clock format
 */
enum ClockFormat {
    Format_24H = 23,
    Format_12H = 11
};

// TODO The right one pls
#define ENTRY_COUNTS 10

#define CONFIG_FILE_NAME "/config.json"
#define DEFAULT_BLINK_BRIGHTNESS 0.7
#define DEFAULT_DISPLAY_SECONDS true
#define DEFAULT_NIGHT_TIME_ENABLED true
#define DEFAULT_LIGHT_INFLUENCE 50
#define DEFAULT_NIGHT_TIME_BEGIN 2300
#define DEFAULT_NIGHT_TIME_END 600
#define DEFAULT_BRIGHTNESS 10
#define DEFAULT_DIMM_SECONDS false
#define DEFAULT_PRESENCE_INTERVAL 100

/**
 * @brief Configuration class for TIDILE, will be saved and restored out of permanent storage at each reboot
 *
 */
struct ClockConfig {
    bool displaySeconds = DEFAULT_DISPLAY_SECONDS;
    Color colorHours = Color(255, 0, 0);
    Color colorMinutes = Color(0, 255, 0);
    Color colorSeconds = Color(0, 0, 255);
    Color colorHumidity = Color();
    Color colorTemperature = Color();
    Color colorPressure = Color();
    bool nightTimeEnabled = DEFAULT_NIGHT_TIME_ENABLED;
    uint16_t lightInfluence = DEFAULT_LIGHT_INFLUENCE;
    uint16_t nightTimeBegin = DEFAULT_NIGHT_TIME_BEGIN; // (13:30 -> 1330, 06:24 -> 624)
    uint16_t nightTimeEnd = DEFAULT_NIGHT_TIME_END;
    uint8_t brightness = DEFAULT_BRIGHTNESS;
    bool dimmSeconds = DEFAULT_DIMM_SECONDS;
    bool tempOverwriteNightTime = false;
    ClockFormat format = ClockFormat::Format_24H;
    bool reverseDirection = false;
    int lastNightTimeOverwriteCheckTime = 0;
    bool presenceDetection = false;
    std::vector<String> presenceDeviceHostnames = {};

    void deserialize(JsonObject *json) {
        Serial.println("Deserializing config");
        JsonObject hours = (*json)["hours"];
        colorHours.deserialize(&hours);
        JsonObject minutes = (*json)["minutes"];
        colorMinutes.deserialize(&minutes);
        JsonObject seconds = (*json)["seconds"];
        colorSeconds.deserialize(&seconds);

        displaySeconds = (*json)["displaySeconds"];
        dimmSeconds = (*json)["dimmSeconds"];
        nightTimeEnabled = (*json)["nightTimeEnabled"];
        nightTimeBegin = (*json)["nightTimeBegin"];
        nightTimeEnd = (*json)["nightTimeEnd"];
        brightness = (*json)["brightness"];
        lightInfluence = (*json)["lightInfluence"];
        format = (*json)["format"];
        reverseDirection = (*json)["reverse_direction"];
        presenceDetection = (*json)["presenceDetection"];
        JsonArray hostnames = (*json)["presenceDeviceHostnames"];
        presenceDeviceHostnames = deserializeVector(hostnames);
    }

    void serialize(JsonObject *json) {
        // preferences->begin("prefs", false);
        // preferences->putBool("valid", true);
        (*json)["valid"] = true;
        // preferences->putBool("displSecs", displaySeconds);
        (*json)["displaySeconds"] = displaySeconds;
        // preferences->putBool("dimmSecs", dimmSeconds);
        (*json)["dimmSeconds"] = dimmSeconds;

        JsonObject hours = (*json)["hours"].to<JsonObject>();
        colorHours.serialize(&hours);
        JsonObject minutes = (*json)["minutes"].to<JsonObject>();
        colorMinutes.serialize(&minutes);
        JsonObject seconds = (*json)["seconds"].to<JsonObject>();
        colorSeconds.serialize(&seconds);

        (*json)["nightTimeEnabled"] = nightTimeEnabled;
        (*json)["nightTimeBegin"] = nightTimeBegin;
        (*json)["nightTimeEnd"] = nightTimeEnd;
        (*json)["brightness"] = brightness;
        (*json)["lightInfluence"] = lightInfluence;

        (*json)["format"] = format;
        (*json)["reverse_direction"] = reverseDirection;

        (*json)["presenceDetection"] = presenceDetection;
        JsonArray array = (*json)["presenceDeviceHostnames"].to<JsonArray>();
        for (String &s: presenceDeviceHostnames) {
            array.add(s);
        }
    }

    void flushConfig() {
        // TODO calc size
        JsonDocument doc;
        JsonObject json = doc.to<JsonObject>();
        Serial.println("Flushing config");
        serialize(&json);
        String buff;
        // TODO change back to file
        Serial.println("Serializing config");
        serializeJson(json, buff);
        serializeJson(json, Serial);
        Serial.println("Serialized config, writing to file");
        writeFile(LittleFS, CONFIG_FILE_NAME, buff.c_str());
        Serial.println("Wrote config to file.");
        toSerial();
    }

    void toSerial() {
        Serial.print("displaySeconds: ");
        Serial.println(displaySeconds);
        Serial.print("colorHours: ");
        Serial.println(colorHours.toString());
        Serial.print("colorMinutes: ");
        Serial.println(colorMinutes.toString());
        Serial.print("colorSeconds: ");
        Serial.println(colorSeconds.toString());
        Serial.print("colorHumidity: ");
        Serial.println(colorHumidity.toString());
        Serial.print("colorTemperature: ");
        Serial.println(colorTemperature.toString());
        Serial.print("colorPressure: ");
        Serial.println(colorPressure.toString());
        Serial.print("nightTimeEnabled: ");
        Serial.println(nightTimeEnabled);
        Serial.print("lightInfluence: ");
        Serial.println(lightInfluence);
        Serial.print("nightTimeBegin: ");
        Serial.println(nightTimeBegin);
        Serial.print("nightTimeEnd: ");
        Serial.println(nightTimeEnd);
        Serial.print("brightness: ");
        Serial.println(brightness);
        Serial.print("dimmSeconds: ");
        Serial.println(dimmSeconds);
        Serial.print("tempOverwriteNightTime: ");
        Serial.println(tempOverwriteNightTime);
        Serial.print("format: ");
        Serial.println(format);
        Serial.print("reverseDirection: ");
        Serial.println(reverseDirection);
        Serial.print("lastNightTimeOverwriteCheckTime: ");
        Serial.println(lastNightTimeOverwriteCheckTime);
        Serial.print("presenceDetection: ");
        Serial.println(presenceDetection);
        Serial.print("presenceDeviceHostnames: ");
        Serial.println(presenceDeviceHostnames.size());
    }
};
