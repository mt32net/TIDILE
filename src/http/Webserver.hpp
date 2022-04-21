#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "../helper/WiFiHelper.hpp"
#include "config/config_includes.hpp"
#include "../topics/topicsInclude.hpp"
#include "../ClockConfig.hpp"

#define WEBSERVER_DEFAULT_DOC_SIZE 1024

#define ENDPOINT_COLORS "/colors"
#define ENDPOINT_GENERAL "/general"
#define ENDPOINT_NIGHT_TIME "/nightTime"
#define ENDPOINT_CREDENTIALS "/credentials"
#define ENDPOINT_WIFI "/wifi"
#define ENDPOINT_CUSTOM "/custom"


/**
 * @brief Webser class to initialize the WEbserver for configuration
 *
 */
class Webserver
{
public:
    Webserver(){};

    void setup(AsyncWebServer* server, ClockConfig* config, WiFiHelper* wifiHelper, Custom* custom);

private:
    AsyncWebServer* server;
    ClockConfig* config;
    WiFiHelper* wifiHelper;
    Custom* custom;

    void initializeRoutes();
};