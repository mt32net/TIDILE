#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "RequestHandler.hpp"
#include "config/config_includes.hpp"

/**
 * @brief Webser class to initialize the WEbserver for configuration
 *
 */
class Webserver
{
public:
    Webserver(){};

    void setup(RequestHandler *requestHandler, AsyncWebServer *server)
    {
        this->requestHandler = requestHandler;
        this->server = server;
        // #pragma region HTTP requestHandler
        server->on("/", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onIndex(request); });
        server->on("/colors", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onColors(request); });
        server->on("/envcolors", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onEnvColors(request); });
        server->on("/other", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onOther(request); });
        server->on("/time", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onNightTime(request); });
        server->on("/manual", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onManual(request); });
        server->on("/toggleLamp", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onLamp(request); });
        // server->on("/env", HTTP_GET, [requestHandler](AsyncWebServerRequest *request){handler->onEnv(request);});
        // server->on("/times", HTTP_GET, [requestHandler](AsyncWebServerRequest *request){handler->onTimes(request);});
        server->on("/styles.css", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
                   { requestHandler->onStyleSheet(request); });

        server->onNotFound([](AsyncWebServerRequest *request)
                           { request->send(404); });
        server->begin();
        Serial.println("HTTP server started");
        // #pragma endregion
    }

private:
    RequestHandler *requestHandler;
    AsyncWebServer *server;
};