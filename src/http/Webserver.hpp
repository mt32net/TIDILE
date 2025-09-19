#pragma once

#include <Arduino.h>
#include "WiFiHelper.hpp"
#include "ClockConfig.hpp"
#include "http/PingManager.hpp"
#include "../topics/custom.hpp"

class AsyncWebServer;
class AsyncWebServerRequest;

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
 * @brief Webserver class to initialize the WEbserver for configuration
 *
 */
class Webserver : public TIDILE_Plugin
{
public:
    void setup(ClockTime time) override;

    void loop(ClockTime time) override;

    Webserver() = default;

    Webserver *setup(CustomTopic *custom, PingManager *ping);

private:
    AsyncWebServer *server{};
    CustomTopic *custom{};
    PingManager *pingManager{};

    unsigned long lastRequestMillis = 0;
    void initializeRoutes();
    bool isRateLimited(AsyncWebServerRequest *request);
};