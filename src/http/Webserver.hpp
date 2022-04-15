#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "../helper/WiFiHelper.hpp"
#include "config/config_includes.hpp"
#include "../ClockConfig.hpp"

#define WEBSERVER_DEFAULT_DOC_SIZE 1024

#define ENDPOINT_COLORS "/colors"
#define ENDPOINT_GENERAL "/general"
#define ENDPOINT_NIGHT_TIME "/nightTime"
#define ENDPOINT_CREDENTIALS "/credentials"
#define ENDPOINT_AP "/isAP"

/**
 * @brief Webser class to initialize the WEbserver for configuration
 *
 */
class Webserver
{
public:
    Webserver(){};

    void setup(AsyncWebServer *server, ClockConfig *config, WiFiHelper * wifiHelper);

private:
    AsyncWebServer *server;
    ClockConfig *config;
    WiFiHelper *wifiHelper;

    void initializeRoutes();
};