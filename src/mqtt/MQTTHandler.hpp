#pragma once
#include <Arduino.h>
#include <WiFiClient.h>
#include <vector>
#include <plugins/TIDILE_Plugin.hpp>

#include "ClockConfig.hpp"
#include "ClockInfo.hpp"
#include "config/compilation_varying.hpp"

#ifdef FEATURE_MQTT
#include <PubSubClient.h>
#else
class PubSubClient;
#endif

class TIDILE;

class MQTTHandler: public TIDILE_Plugin
{
public:
    MQTTHandler();
    MQTTHandler* setupInternal(String uri, int port);
    void setup(ClockTime time) override;
    void callback(char *topic, byte *payload, unsigned int length);

private:
    void subscribeTIDILETopics();
    void loop(ClockTime t) override;
    void subscribe(String topic);
    void publish(String topic, String payload);
    void reconnect();
    void subscribePrivate(String topic);
    void restore();

    bool handle(String topic, String payload);

    String uri;
    int port{};
    WiFiClient wifiClient;
    PubSubClient *client{};
    std::vector<String> subscribedTopics;

    int startupSecs{};
    int startupMins{};
    int lastUpdateMin{};

    int connectionAttempts{};

private:
    friend class TIDILE;
};