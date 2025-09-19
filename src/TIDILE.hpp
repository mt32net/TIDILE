#pragma once

#include "Arduino.h"
#include <FastLED.h>
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"
#include "config/config_includes.hpp"
#include "LEDController.hpp"
#include "plugins/TIDILE_Plugin.hpp"
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
    static TIDILE *instance;

    /**
     * @brief load the clock configuration from the file system
     *
     */
    void loadClockConfig();

    /**
     * @brief setup method, call this method to setup tidile
     *
     * @param leds the leds array
     * @param numberLEDs the number of leds
     */
    void setup(CRGB *leds, int numberLEDs);
    /**
     * @brief update method, call this method to update tidile in the loop
     *
     */
    void update();
    /**
     * @brief displays current time
     *
     * @param time current time
     */
    void displayTime(const ClockTime &time);

    /**
     * @brief add a plugin to the tidile object
     *
     * @param plugin the plugin to add
     */
    void addPlugin(TIDILE_Plugin *plugin);

    void setActive(bool active)
    {
        this->active = active;
    }

    ClockConfig *getConfig();
    Color lmapColor = Color(255, 255, 255);
    ClockEnv env;

private:
    // FastLED Array
    CRGB *leds;
    ClockConfig configuration;
    LEDController ledController;
    short numberZones = NUMBER_ZONES;
    int numberLEDs;
    std::vector<TIDILE_Plugin *> plugins;
    bool active = true;
    /**
     * @brief set all LEDs to black
     */
    void clear();
    /**
     * @brief small startup animation to test leds connected
     *
     * @param delay the delay betwenn each LED operation in animation
     * @param time the current time
     */
    void startupLEDs(int delay, ClockTime time) const;

    // Loop variables
    int lastSec = 0;
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