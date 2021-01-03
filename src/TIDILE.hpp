#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"
#include "Handler.hpp"
#include "Webserver.hpp"
#include "definements.hpp"
#include "config.hpp"
#include "helper.hpp"
#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#endif

class TIDILE
{
public:
    TIDILE();
    ///setup function
    ///@param leds the LED array wich is already registered in FastLED within the main file
    ///@param numberLEDS the number of LEDS within the array
    ///@param configuration the pointer to thee configuration object where all settings are saved
    void setup(CRGB leds[NUM_LEDS], int numberLEDs, AsyncWebServer *server);
    void displaCustom(Color colorCode, ClockTime until);
#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
    void addBMP(Adafruit_BME280 *bmp);
    ClockEnv getEnv();
#endif
    void loop();
    ///displays current time
    void displayTime(ClockTime time);
    ///displays information about your sorrounding such as temperature, humdity and pressure. IF the sensors are connected and defined in the config file
    void displayEnv(ClockEnv env);
    ///@param progress value between 0 and 99
    ClockConfig *getConfig();
    bool clockMode = true;
    Color lmapColor = Color(255, 255, 255);

private:
    CRGB *leds;
    ClockConfig configuration;
    Handler handler;
    Preferences preferences;
    Webserver webserver;
    AsyncWebServer *server;
#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
    Adafruit_BME280 *bmp;
#endif
    int numberLEDs;
    int mapToLEDs(int value, int max);
    void clear();
    void startupLEDs(int delay);

    int lastSec = 0;
    ClockTime customDisplayTil;
    int loopI = 0;
    int touchAverage = 30;
    long lightAvg = 0;
};