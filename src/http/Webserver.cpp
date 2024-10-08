#include "Webserver.hpp"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <StreamUtils.h>
#include "../topics/night_time.hpp"
#include "../topics/general.hpp"
#include "../topics/colors.hpp"

#include "WiFiHelper.hpp"

#ifdef FEATURE_WEB_SERVER

void Webserver::setup(ClockTime time) {
    initializeRoutes();

    server->serveStatic("/", LittleFS, "/dist/");
    server->on("/", HTTP_GET, [=](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/dist/index.html", "text/html");
    });

    server->onNotFound([](AsyncWebServerRequest *request) { request->send(404); });
    server->begin();
    Serial.println("HTTP server started");
}

void Webserver::loop(ClockTime time) {
}

Webserver* Webserver::setup(CustomTopic *custom,
                      PingManager *ping) {
    this->custom = custom;
    this->pingManager = ping;
    this->server = new AsyncWebServer(HTTP_ENDPOINT_PORT);
    return this;
}

void Webserver::initializeRoutes() {
    // COLOR
    server->on(ENDPOINT_COLORS, HTTP_GET, [this](AsyncWebServerRequest *request) {
        // if (this->isRateLimited(request)) return;
        // Serial.println("-- GET COLORS --");
        // Serial.println(millis());
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        JsonDocument json;
        ColorsTopic colors;
        colors.loadFromConfig(config);
        colors.serializeToJson(json);
        serializeJson(json, *response);
        request->send(response);
        // Serial.println(millis());
        //json.garbageCollect();
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

    auto *handlerColor = new AsyncCallbackJsonWebHandler(ENDPOINT_COLORS,
                                                         [this](AsyncWebServerRequest *request, JsonVariant &json) {
                                                             if (this->isRateLimited(request))
                                                                 return;
                                                             ColorsTopic colors;
                                                             JsonDocument doc;
                                                             doc.set(json);
                                                             colors.deserializeFromJSON(doc);
                                                             colors.saveToConfig(config);
                                                             request->send(200);
                                                             config->flushConfig();
                                                             // doc.garbageCollect();
                                                         });
    server->addHandler(handlerColor);

    // GENERAL
    server->on(ENDPOINT_GENERAL, HTTP_GET,
               [this](AsyncWebServerRequest *request) {
                   // if (this->isRateLimited(request)) return;
                   AsyncResponseStream *response = request->beginResponseStream("application/json");
                   JsonDocument json;
                   GeneralTopic general;
                   general.loadFromConfig(config);
                   general.serializeToJson(json);
                   serializeJson(json, *response);
                   request->send(response);
                   // json.garbageCollect();
               });

    auto *handlerGeneral = new AsyncCallbackJsonWebHandler(ENDPOINT_GENERAL,
                                                           [this](AsyncWebServerRequest *request, JsonVariant &json) {
                                                               if (this->isRateLimited(request))
                                                                   return;
                                                               GeneralTopic general;
                                                               JsonDocument doc;
                                                               doc.set(json);
                                                               general.deserializeFromJSON(doc);
                                                               general.saveToConfig(config);
                                                               request->send(200);
                                                               config->flushConfig();
                                                               // doc.garbageCollect();
                                                           });
    server->addHandler(handlerGeneral);

    // NIGHT TIME
    server->on(ENDPOINT_NIGHT_TIME, HTTP_GET,
               [this](AsyncWebServerRequest *request) {
                   // if (this->isRateLimited(request)) return;
                   AsyncResponseStream *response = request->beginResponseStream("application/json");
                   JsonDocument json;
                   NightTimeTopic nightTime;
                   nightTime.loadFromConfig(config);
                   nightTime.serializeToJson(json);
                   serializeJson(json, *response);
                   request->send(response);
                   // json.garbageCollect();
               });

    auto *handlerNightTIme = new AsyncCallbackJsonWebHandler(ENDPOINT_NIGHT_TIME,
                                                             [this](AsyncWebServerRequest *request, JsonVariant &json) {
                                                                 if (this->isRateLimited(request)) return;
                                                                 NightTimeTopic nightTime;
                                                                 JsonDocument doc;
                                                                 doc.set(json);
                                                                 nightTime.deserializeFromJSON(doc);
                                                                 nightTime.saveToConfig(config);
                                                                 request->send(200);
                                                                 config->flushConfig();
                                                             });
    server->addHandler(handlerNightTIme);

    // CREDENTIALS
    auto *handlerCredentials = new AsyncCallbackJsonWebHandler(ENDPOINT_CREDENTIALS,
                                                               [this](AsyncWebServerRequest *request,
                                                                      JsonVariant &json) {
                                                                   // if (this->isRateLimited(request)) return;
                                                                   String ssid = json["ssid"];
                                                                   String password = json["password"];
                                                                   request->send(200);
                                                                   WiFiHelper::getInstance()->setCredentials(ssid, password);
                                                               });
    server->addHandler(handlerCredentials);

    // WIFI INFO
    server->on(ENDPOINT_WIFI, HTTP_GET,
               [this](AsyncWebServerRequest *request) {
                   // if (this->isRateLimited(request)) return;
                   AsyncResponseStream *response = request->beginResponseStream("application/json");
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

    // WIFI INFO
    server->on(ENDPOINT_NETWORKS, HTTP_GET,
               [this](AsyncWebServerRequest *request) {
                   // if (this->isRateLimited(request)) return;
                   AsyncResponseStream *response = request->beginResponseStream("application/json");
                   JsonDocument json;
                   int i = 0;
                   for (const auto &net: WiFiHelper::getInstance()->getReachableNets()) {
                       json["networks"][i]["ssid"] = net.ssid;
                       json["networks"][i]["rssi"] = net.rssi;
                       i++;
                   }
                   serializeJson(json, *response);
                   request->send(response);
               });

    // CUSTOM
    auto *handlerCustom = new AsyncCallbackJsonWebHandler(ENDPOINT_CUSTOM,
                                                          [this](AsyncWebServerRequest *request, JsonVariant &json) {
                                                              // if (this->isRateLimited(request)) return;
                                                              JsonDocument doc;
                                                              doc.set(json);
                                                              this->custom->deserializeFromJSON(doc);
                                                              request->send(200);
                                                              // doc.garbageCollect();
                                                          });
    server->addHandler(handlerCustom);

    server->on(ENDPOINT_CUSTOM, HTTP_GET,
               [this](AsyncWebServerRequest *request) {
                   // if (this->isRateLimited(request)) return;
                   AsyncResponseStream *response = request->beginResponseStream("application/json");
                   JsonDocument json;
                   this->custom->serializeToJson(json);
                   serializeJson(json, *response);
                   request->send(response);
               });

    // PRESENCE

    server->on(ENDPOINT_PRESENCE, HTTP_GET,
               [this](AsyncWebServerRequest *request) {
                   AsyncResponseStream *response = request->beginResponseStream("application/json");
                   JsonDocument json;
                   json["enabled"] = this->config->presenceDetection;
                   const JsonArray devices = json["devices"].to<JsonArray>();
                   for (PresenceDevice &d: this->pingManager->getDevices()) {
                       JsonObject o = devices.add<JsonObject>();
                       o["address"] = d.address;
                       o["online"] = d.online;
                   }
                   serializeJson(json, *response);
                   request->send(response);
               });

    auto *handlerPresence = new AsyncCallbackJsonWebHandler(ENDPOINT_PRESENCE,
                                                            [this](AsyncWebServerRequest *request, JsonVariant &json) {
                                                                if (this->isRateLimited(request)) return;
                                                                this->config->presenceDetection = json["enabled"];
                                                                this->config->presenceDeviceHostnames.clear();
                                                                for (auto a: json["devices"].as<JsonArray>()) {
                                                                    this->config->presenceDeviceHostnames.push_back(
                                                                        a.as<String>());
                                                                }
                                                                this->config->flushConfig();
                                                                this->pingManager->updateDevices();
                                                                request->send(200);
                                                            });
    server->addHandler(handlerPresence);
}

bool Webserver::isRateLimited(AsyncWebServerRequest *request) {
    bool limited = true;
    if (lastRequestMillis + RATE_LIMIT_MILLIS < millis())
        limited = false;
    if (limited) {
        request->send(429);
    }
    lastRequestMillis = millis();
    return limited;
}

#else
void Webserver::setup(AsyncWebServer *server, ClockConfig *config, WiFiHelper *wifiHelper, Custom *custom,
    PingManager *ping) {
}

void Webserver::initializeRoutes() {
}

bool Webserver::isRateLimited(AsyncWebServerRequest *request) {
    return false;
}
#endif

