#include "Webserver.hpp"
#include "../topics/colors.hpp"
#include "../topics/general.hpp"
#include "../topics/night_time.hpp"
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include <AsyncMessagePack.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

#include "WiFiHelper.hpp"

#ifdef FEATURE_WEB_SERVER

void Webserver::setup(ClockTime time)
{
    initializeRoutes();

    server->serveStatic("/", LittleFS, "/dist/");
    server->on("/", HTTP_GET, [=](AsyncWebServerRequest *request)
               { request->send(LittleFS, "/dist/index.html", "text/html"); });
    server->on("/index.htm", HTTP_GET, [=](AsyncWebServerRequest *request)
               { request->send(LittleFS, "/dist/index.html", "text/html"); });

    server->onNotFound(
        [](AsyncWebServerRequest *request)
        { request->send(404); });
    server->begin();
    Serial.println("HTTP server started");
}

void Webserver::loop(ClockTime time) {}

Webserver *Webserver::setup(CustomTopic *custom, PingManager *ping)
{
    this->custom = custom;
    this->pingManager = ping;
    this->server = new AsyncWebServer(HTTP_ENDPOINT_PORT);
    return this;
}

JsonDocument parseBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    JsonDocument doc;
    if (data != nullptr)
    {
        Serial.print("PARSING BODY: ");
        Serial.println(String((char *)data).substring(0, len));

        DeserializationError error = deserializeJson(doc, data, len);
        if (error)
        {
            Serial.println("PARSING BODY: ERROR");

            request->send(400);
            return JsonDocument();
        }
    }
    else
    {
        Serial.println("PARSING BODY: NO BODY");

        request->send(400);
        return JsonDocument();
    }
    return doc;
}

void Webserver::initializeRoutes()
{
    // COLOR
    {
        server->on(ENDPOINT_COLORS, HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
                       // if (this->isRateLimited(request)) return;
                       // Serial.println("-- GET COLORS --");
                       // Serial.println(millis());
                       AsyncResponseStream *response =
                           request->beginResponseStream("application/json");
                       JsonDocument json;
                       ColorsTopic colors;
                       colors.loadFromConfig(config);
                       colors.serializeToJson(json);
                       serializeJson(json, *response);
                       request->send(response);
                       // Serial.println(millis());
                       // json.garbageCollect();
                   });

        server->on(ENDPOINT_COLORS, HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                   {
                       if(this->isRateLimited(request)) return;
                       ColorsTopic colors;
                          JsonDocument doc = parseBody(request, data, len, index, total);
                       if (doc.isNull()) return;
                       colors.deserializeFromJSON(doc);
                       colors.saveToConfig(config);
                       request->send(200);
                       config->flushConfig(); });
    }

    // GENERAL
    {
        server->on(ENDPOINT_GENERAL, HTTP_GET,
                   [this](AsyncWebServerRequest *request)
                   {
                       // if (this->isRateLimited(request)) return;
                       AsyncResponseStream *response =
                           request->beginResponseStream("application/json");
                       JsonDocument json;
                       GeneralTopic general;
                       general.loadFromConfig(config);
                       general.serializeToJson(json);
                       serializeJson(json, *response);
                       request->send(response);
                       // json.garbageCollect();
                   });

        server->on(ENDPOINT_GENERAL, HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                   {
                       if(this->isRateLimited(request)) return;
                       GeneralTopic general;
                       JsonDocument doc = parseBody(request, data, len, index, total);
                       if (doc.isNull()) return;
                       general.deserializeFromJSON(doc);
                       general.saveToConfig(config);
                       request->send(200);
                       config->flushConfig(); });
    }

    // NIGHT TIME
    {
        server->on(ENDPOINT_NIGHT_TIME, HTTP_GET,
                   [this](AsyncWebServerRequest *request)
                   {
                       // if (this->isRateLimited(request)) return;
                       AsyncResponseStream *response =
                           request->beginResponseStream("application/json");
                       JsonDocument json;
                       NightTimeTopic nightTime;
                       nightTime.loadFromConfig(config);
                       nightTime.serializeToJson(json);
                       serializeJson(json, *response);
                       request->send(response);
                       // json.garbageCollect();
                   });

        server->on(ENDPOINT_NIGHT_TIME, HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                   {
                       if(this->isRateLimited(request)) return;
                       NightTimeTopic nightTime;

                          JsonDocument doc = parseBody(request, data, len, index, total);
                       if (doc.isNull()) return;
                       nightTime.deserializeFromJSON(doc);
                       nightTime.saveToConfig(config);
                       request->send(200);
                       config->flushConfig(); });
    }

    // CREDENTIALS
    {
        server->on(ENDPOINT_CREDENTIALS, HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                   {
                          if (this->isRateLimited(request))
                            return;
                          JsonDocument doc = parseBody(request, data, len, index, total);
                          if (doc.isNull())
                            return;
                          String ssid = doc["ssid"];
                          String password = doc["password"];
                          request->send(200);
                          WiFiHelper::getInstance()->setCredentials(ssid, password); });
    }

    // WIFI INFO
    {
        server->on(ENDPOINT_WIFI, HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
                       // if (this->isRateLimited(request)) return;
                       AsyncResponseStream *response =
                           request->beginResponseStream("application/json");
                       JsonDocument json;
                       json["apMode"] = WiFiHelper::getInstance()->isAPMode();
                       // int i = 0;
                       // for (auto net : this->wifiHelper->getReachableNets()) {
                       //     json["available"][i]["ssid"] = net.ssid;
                       //     json["available"][i]["rssi"] = net.rssi;
                       //     i++;
                       // }
                       NetworkInfo current = WiFiHelper::getInstance()->getCurrentNetwork();
                       json["current"]["ssid"] = current.ssid;
                       json["current"]["rssi"] = current.rssi;
                       serializeJson(json, *response);
                       request->send(response);
                       // json.garbageCollect();
                   });
    }

    // Networks INFO
    {
        server->on(
            ENDPOINT_NETWORKS, HTTP_GET, [this](AsyncWebServerRequest *request)
            {
        // if (this->isRateLimited(request)) return;
        AsyncResponseStream *response =
            request->beginResponseStream("application/json");
        JsonDocument json;
        int i = 0;
        for (const auto &net : WiFiHelper::getInstance()->getReachableNets()) {
          json["networks"][i]["ssid"] = net.ssid;
          json["networks"][i]["rssi"] = net.rssi;
          i++;
        }
        serializeJson(json, *response);
        request->send(response); });
    }

    // CUSTOM
    {
        server->on(ENDPOINT_CUSTOM, HTTP_GET, [this](AsyncWebServerRequest *request)
                   {
            // if (this->isRateLimited(request)) return;
            AsyncResponseStream *response =
                request->beginResponseStream("application/json");
            JsonDocument json;
            this->custom->serializeToJson(json);
            serializeJson(json, *response);
            request->send(response); });

        server->on(ENDPOINT_CUSTOM, HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                   {
                       if (this->isRateLimited(request))
                           return;
                       JsonDocument doc = parseBody(request, data, len, index, total);
                       if (doc.isNull())
                           return;
                       this->custom->deserializeFromJSON(doc);
                       request->send(200);
                       // doc.garbageCollect();
                   });
    }

    // PRESENCE
    {
        server->on(ENDPOINT_PRESENCE, HTTP_GET,
                   [this](AsyncWebServerRequest *request)
                   {
                       AsyncResponseStream *response =
                           request->beginResponseStream("application/json");
                       JsonDocument json;
                       json["enabled"] = this->config->presenceDetection;
                       const JsonArray devices = json["devices"].to<JsonArray>();
                       for (PresenceDevice &d : this->pingManager->getDevices())
                       {
                           JsonObject o = devices.add<JsonObject>();
                           o["address"] = d.address;
                           o["online"] = d.online;
                       }
                       serializeJson(json, *response);
                       request->send(response);
                   });

        server->on(ENDPOINT_PRESENCE, HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                   {
            if (this->isRateLimited(request))
                return;
            JsonDocument doc = parseBody(request, data, len, index, total);
            if (doc.isNull())
                return;
            this->config->presenceDetection = doc["enabled"];
            this->config->presenceDeviceHostnames.clear();
            for (auto a : doc["devices"].as<JsonArray>())
            {
                this->config->presenceDeviceHostnames.push_back(a.as<String>());
            }
            this->config->flushConfig();
            this->pingManager->updateDevices();
            request->send(200); });
    }
}

bool Webserver::isRateLimited(AsyncWebServerRequest *request)
{
    bool limited = true;
    if (lastRequestMillis + RATE_LIMIT_MILLIS < millis())
        limited = false;
    if (limited)
    {
        request->send(429);
    }
    lastRequestMillis = millis();
    return limited;
}

#else
void Webserver::setup(AsyncWebServer *server, ClockConfig *config,
                      WiFiHelper *wifiHelper, Custom *custom,
                      PingManager *ping) {}

void Webserver::initializeRoutes() {}

bool Webserver::isRateLimited(AsyncWebServerRequest *request) { return false; }
#endif
