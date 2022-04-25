#include "Webserver.hpp"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <StreamUtils.h>
#include <SPIFFS.h>

void Webserver::setup(AsyncWebServer *server, ClockConfig *config, WiFiHelper * wifiHelper, Custom* custom)
{
    this->config = config;
    this->server = server;
    this->wifiHelper = wifiHelper;
    this->custom = custom;

    initializeRoutes();

    server->serveStatic("/css/", SPIFFS, "/dist/css/");
    server->serveStatic("/js/", SPIFFS, "/dist/js/");
    server->on("/", HTTP_GET, [=](AsyncWebServerRequest* request) { request->send(SPIFFS, "/dist/index.html", "text/html"); });

    server->onNotFound([](AsyncWebServerRequest *request)
                       { request->send(404); });
    server->begin();
    Serial.println("HTTP server started");
}

void Webserver::initializeRoutes()
{

    // COLOR
    server->on(ENDPOINT_COLORS, HTTP_GET, [this](AsyncWebServerRequest *request) {
        // if (this->isRateLimited(request)) return;
        // Serial.println("-- GET COLORS --");
        // Serial.println(millis());
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        Colors colors;
        colors.loadFromConfig(config);
        colors.serializeToJson(json);
        serializeJson(json, *response);
        request->send(response);
        // Serial.println(millis());
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
        if (this->isRateLimited(request)) return;
        Colors colors;
        DynamicJsonDocument doc(WEBSERVER_DEFAULT_DOC_SIZE);
        doc.set(json);
        colors.deserializeFromJSON(doc);
        colors.saveToConfig(config);
        request->send(200);
        config->flushConfig();
        // doc.garbageCollect();
    });
    server->addHandler(handlerColor);

    // GENERAL
    server->on(ENDPOINT_GENERAL, HTTP_GET, [this](AsyncWebServerRequest *request) {
        // if (this->isRateLimited(request)) return;
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        General general;
        general.loadFromConfig(config);
        general.serializeToJson(json);
        serializeJson(json, *response);
        request->send(response); 
        // json.garbageCollect();
    });

    AsyncCallbackJsonWebHandler *handlerGeneral = new AsyncCallbackJsonWebHandler(ENDPOINT_GENERAL, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        if (this->isRateLimited(request)) return;
        General general;
        DynamicJsonDocument doc(WEBSERVER_DEFAULT_DOC_SIZE);
        doc.set(json);
        general.deserializeFromJSON(doc);
        general.saveToConfig(config);
        request->send(200); 
        config->flushConfig();
        // doc.garbageCollect();
    });
    server->addHandler(handlerGeneral);


    // NIGHT TIME
    server->on(ENDPOINT_NIGHT_TIME, HTTP_GET, [this](AsyncWebServerRequest *request) {
        // if (this->isRateLimited(request)) return;
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        NightTime nightTime;
        nightTime.loadFromConfig(config);
        nightTime.serializeToJson(json);
        serializeJson(json, *response);
        request->send(response); 
        // json.garbageCollect();
    });

    AsyncCallbackJsonWebHandler *handlerNightTIme = new AsyncCallbackJsonWebHandler(ENDPOINT_NIGHT_TIME, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        if (this->isRateLimited(request)) return;
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
        // if (this->isRateLimited(request)) return;
        String ssid = json["ssid"];
        String password = json["password"];
        request->send(200);
        this->wifiHelper->setCredentials(ssid, password);
    });
    server->addHandler(handlerCredentials);

    // WIFI INFO
    server->on(ENDPOINT_WIFI, HTTP_GET, [this](AsyncWebServerRequest *request) {
        // if (this->isRateLimited(request)) return;
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        json["apMode"] = this->wifiHelper->isAPMode();
        // int i = 0;
        // for (auto net : this->wifiHelper->getReachableNets()) {
        //     json["available"][i]["ssid"] = net.ssid;
        //     json["available"][i]["rssi"] = net.rssi;
        //     i++;
        // }
        Network current = this->wifiHelper->getCurrentNetwork();
        json["current"]["ssid"] = current.ssid;
        json["current"]["rssi"] = current.rssi;
        serializeJson(json, *response);
        request->send(response); 
        // json.garbageCollect();
    });

    // WIFI INFO
    server->on(ENDPOINT_NETWORKS, HTTP_GET, [this](AsyncWebServerRequest *request) {
        // if (this->isRateLimited(request)) return;
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument json(WEBSERVER_DEFAULT_DOC_SIZE);
        int i = 0;
        for (auto net : this->wifiHelper->getReachableNets()) {
            json["networks"][i]["ssid"] = net.ssid;
            json["networks"][i]["rssi"] = net.rssi;
            i++;
        }
        serializeJson(json, *response);
        request->send(response); 
    });


    // CUSTOM
    AsyncCallbackJsonWebHandler *handlerCustom = new AsyncCallbackJsonWebHandler(ENDPOINT_CUSTOM, [this](AsyncWebServerRequest *request, JsonVariant &json) {
        // if (this->isRateLimited(request)) return;
        DynamicJsonDocument doc(WEBSERVER_DEFAULT_DOC_SIZE);
        doc.set(json);
        this->custom->deserializeFromJSON(doc);
        request->send(200);
        // doc.garbageCollect();
    });
    server->addHandler(handlerCustom);
}

bool Webserver::isRateLimited(AsyncWebServerRequest* request) {
    bool limited = true;
    if (lastRequestMillis + RATE_LIMIT_MILLIS < millis()) limited = false;
    if (limited) {
        request->send(429);
    }
    lastRequestMillis = millis();
    return limited;
}