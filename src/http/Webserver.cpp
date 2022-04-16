#include "Webserver.hpp"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <StreamUtils.h>
#include <SPIFFS.h>
#include "../topics/topicsInclude.hpp"

void Webserver::setup(AsyncWebServer *server, ClockConfig *config, WiFiHelper * wifiHelper)
{
    this->config = config;
    this->server = server;
    this->wifiHelper = wifiHelper;

    server->serveStatic("/css/", SPIFFS, "/dist/css/");
    server->serveStatic("/js/", SPIFFS, "/dist/js/");
    server->on("/", HTTP_GET, [=](AsyncWebServerRequest* request) { request->send(SPIFFS, "/dist/index.html", "text/html"); });
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

    // server->on(ENDPOINT_COLORS, HTTP_GET, [this](AsyncWebServerRequest *request) {
    //     Serial.println("----- GET ------");
    //     // 0
    //     Serial.println(millis());
    //     AsyncJsonResponse *response = new AsyncJsonResponse();
    //     DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
    //     // 1
    //     Serial.println(millis());
    //     Colors colors;
    //     colors.loadFromConfig(config);
    //     // 2
    //     Serial.println(millis());
    //     colors.serializeToJson(json);
    //     // 3
    //     Serial.println(millis());
    //     response->getRoot().set(json);
    //     response->setLength();
    //     // 4
    //     Serial.println(millis());
    //     request->send(response);
    //     // 5
    //     Serial.println(millis());
    //     //json.garbageCollect();
    // });

    AsyncCallbackJsonWebHandler *handlerColor = new AsyncCallbackJsonWebHandler(ENDPOINT_COLORS, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        Colors colors;
        DynamicJsonDocument doc(WEBSERVER_DEFAULT_DOC_SIZE);
        doc.set(json);
        colors.deserializeFromJSON(doc);
        colors.saveToConfig(config);
        request->send(200);
        config->flushConfig();
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
        config->flushConfig();
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
        config->flushConfig();
    });
    server->addHandler(handlerNightTIme);

    // CREDENTIALS
    AsyncCallbackJsonWebHandler *handlerCredentials = new AsyncCallbackJsonWebHandler(ENDPOINT_CREDENTIALS, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        String ssid = json["ssid"];
        String password = json["password"];
        request->send(200);
        this->wifiHelper->setCredentials(ssid, password);
    });
    server->addHandler(handlerCredentials);

    // WIFI INFO
    server->on(ENDPOINT_WIFI, HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        json["apMode"] = this->wifiHelper->isAPMode();
        int i = 0;
        for (auto net : this->wifiHelper->getReachableNets()) {
            json["available"][i]["ssid"] = net.ssid;
            json["available"][i]["rssi"] = net.rssi;
            i++;
        }
        Network current = this->wifiHelper->getCurrentNetwork();
        json["current"]["ssid"] = current.ssid;
        json["current"]["rssi"] = current.rssi;
        serializeJson(json, *response);
        request->send(response); 
        json.garbageCollect();
    });
}