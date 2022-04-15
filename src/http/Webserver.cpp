#include "Webserver.hpp"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include "../topics/topicsInclude.hpp"

void Webserver::setup(AsyncWebServer *server, ClockConfig *config)
{
    this->config = config;
    this->server = server;

    // // #pragma region HTTP requestHandler
    // server->on("/", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onIndex(request); });
    // server->on("/colors", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onColors(request); });
    // server->on("/envcolors", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onEnvColors(request); });
    // server->on("/other", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onOther(request); });
    // server->on("/time", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onNightTime(request); });
    // server->on("/manual", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onManual(request); });
    // server->on("/toggleLamp", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onLamp(request); });
    // // server->on("/env", HTTP_GET, [requestHandler](AsyncWebServerRequest *request){handler->onEnv(request);});
    // // server->on("/times", HTTP_GET, [requestHandler](AsyncWebServerRequest *request){handler->onTimes(request);});
    // server->on("/styles.css", HTTP_GET, [requestHandler](AsyncWebServerRequest *request)
    //            { requestHandler->onStyleSheet(request); });
    initializeRoutes();

    server->onNotFound([](AsyncWebServerRequest *request)
                       { request->send(404); });
    server->begin();
    Serial.println("HTTP server started");
}

void Webserver::initializeRoutes()
{

    // COLOR
    server->on(ENDPOINT_COLORS, HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        Colors colors;
        colors.loadFromConfig(config);
        colors.serializeToJson(json);
        serializeJson(json, *response);
        request->send(response); 
        json.garbageCollect();
    });

    AsyncCallbackJsonWebHandler *handlerColor = new AsyncCallbackJsonWebHandler(ENDPOINT_COLORS, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        Colors colors;
        DynamicJsonDocument doc(WEBSERVER_DEFAULT_DOC_SIZE);
        doc.set(json);
        colors.deserializeFromJSON(doc);
        colors.saveToConfig(config);
        request->send(200);
    });
    server->addHandler(handlerColor);

    // GENERAL
    server->on(ENDPOINT_GENERAL, HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        General general;
        general.loadFromConfig(config);
        general.serializeToJson(json);
        serializeJson(json, *response);
        request->send(response); 
        json.garbageCollect();
    });

    AsyncCallbackJsonWebHandler *handlerGeneral = new AsyncCallbackJsonWebHandler(ENDPOINT_GENERAL, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        General general;
        DynamicJsonDocument doc(WEBSERVER_DEFAULT_DOC_SIZE);
        doc.set(json);
        general.deserializeFromJSON(doc);
        general.saveToConfig(config);
        request->send(200);
    });
    server->addHandler(handlerGeneral);


    // NIGHT TIME
    server->on(ENDPOINT_NIGHT_TIME, HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        NightTime nightTime;
        nightTime.loadFromConfig(config);
        nightTime.serializeToJson(json);
        serializeJson(json, *response);
        request->send(response); 
        json.garbageCollect();
    });

    AsyncCallbackJsonWebHandler *handlerNightTIme = new AsyncCallbackJsonWebHandler(ENDPOINT_NIGHT_TIME, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        NightTime nightTime;
        DynamicJsonDocument doc(WEBSERVER_DEFAULT_DOC_SIZE);
        doc.set(json);
        nightTime.deserializeFromJSON(doc);
        nightTime.saveToConfig(config);
        request->send(200);
    });
    server->addHandler(handlerNightTIme);
}