#pragma once

#include <Arduino.h>
#include "WiFiHelper.hpp"
#include "ClockConfig.hpp"
#include "http/PingManager.hpp"
#include "../topics/custom.hpp"

#ifdef FEATURE_WEB_SERVER
#include <ESPAsyncWebServer.h>
#else
class AsyncWebServer;
class AsyncWebServerRequest;
#endif

#define WEBSERVER_DEFAULT_DOC_SIZE 1024

#define ENDPOINT_COLORS "/colors"
#define ENDPOINT_GENERAL "/general"
#define ENDPOINT_NIGHT_TIME "/nightTime"
#define ENDPOINT_CREDENTIALS "/credentials"
#define ENDPOINT_WIFI "/wifi"
#define ENDPOINT_NETWORKS "/networks"
#define ENDPOINT_CUSTOM "/custom"
#define ENDPOINT_PRESENCE "/presence"


/**
 * @brief Webser class to initialize the WEbserver for configuration
 *
 */
class Webserver
{
public:
    Webserver(){};

    void setup(AsyncWebServer* server, ClockConfig* config, WiFiHelper* wifiHelper, Custom* custom, PingManager * ping);

private:
    AsyncWebServer* server;
    ClockConfig* config;
    WiFiHelper* wifiHelper;
    Custom* custom;
    PingManager * pingManager;

    unsigned long lastRequestMillis = 0;
    void initializeRoutes();
    bool isRateLimited(AsyncWebServerRequest* request);
};