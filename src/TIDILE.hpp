#pragma once
#include "Arduino.h"
#include <FastLED.h>
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"
#include "RequestHandler.hpp"
#include "Webserver.hpp"
#include "mqtt/MQTTHandler.hpp"
#include "config/config_includes.hpp"
#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#endif

/**
 * @brief Easy to use class to create custom clock with led strip
 *
 */
class TIDILE
{
public:
    TIDILE();

    /**
     * @brief Loads the configuration from the Prefernces flash, if not available, the default config will be saved and used
     *
     * @return a pointer to the Configuration inside of TIDILE
     */
    ClockConfig *loadClockConfig();
    /**
     * @brief Setup function
     *
     * @param numberLEDS the number of LEDS within the array
     * @param configuration the pointer to thee configuration object where all settings are saved
     */
    void setup(CRGB *leds, int numberLEDs, AsyncWebServer *server);

    /**
     * @brief way to diasply color on all leds until a certain time is reached
     *
     * @param colorCode the color all leds should have
     * @param until the time until normal operation is returned
     */
    void displaCustom(Color colorCode, ClockTime until);

    String getConfigJson();
#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
    /**
     * @brief add BME280 sensor to tidile
     *
     * @param bmp
     */
    void addBMP(Adafruit_BME280 *bmp);
    /**
     * @brief Get the Env object
     *
     * @return ClockEnv
     */
    ClockEnv getEnv();
#endif
    /**
     * @brief update method, call this method to update tidile
     *
     */
    void update();
    /**
     * @brief displays current time
     *
     * @param time current time
     */
    void displayTime(ClockTime time);
    /**
     * @brief displays information about your sorrounding such as temperature, humdity and pressure. IF the sensors are connected and defined in the config file
     *
     * @param env
     */
    void displayEnv(ClockEnv env);
    ClockConfig *getConfig();
    bool clockMode = true;
    Color lmapColor = Color(255, 255, 255);

private:
    // FastLED Array
    CRGB *leds;
    ClockConfig configuration;
    RequestHandler requestHandler;
    Preferences preferences;
    Webserver webserver;
    MQTTHandler mqtt;
    AsyncWebServer *server;
#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
    Adafruit_BME280 *bmp;
#endif
    int numberLEDs;
    int mapToLEDs(int value, int max);
    /**
     * @brief set all LEDs to black
     */
    void clear();
    /**
     * @brief small startup animation to test leds connected
     *
     * @param delay the delay betwenn each LED operation in animation
     */
    void startupLEDs(int delay);

    void mqttCallback(char *topic, byte *payload, unsigned int length);

    int lastSec = 0;
    ClockTime customDisplayTil;
    int loopI = 0;
    int touchAverage = 30;
    long lightAvg = 0;

#ifdef RUN_TESTS
    /**
     * @brief friend method to test Tidile
     *
     */
    friend void tidileTests();
    friend class MQTTHandler;
#endif
};