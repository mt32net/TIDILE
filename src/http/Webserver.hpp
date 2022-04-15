#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "config/config_includes.hpp"
#include "../ClockConfig.hpp"

#define WEBSERVER_DEFAULT_DOC_SIZE 1024

#define ENDPOINT_COLORS "/colors"
#define ENDPOINT_GENERAL "/general"
#define ENDPOINT_NIGHT_TIME "/nightTime"

/**
 * @brief Webser class to initialize the WEbserver for configuration
 *
 */
class Webserver
{
public:
    Webserver(){};

    void setup(AsyncWebServer *server, ClockConfig *config);

private:
    AsyncWebServer *server;
    ClockConfig *config;

    void initializeRoutes();
};