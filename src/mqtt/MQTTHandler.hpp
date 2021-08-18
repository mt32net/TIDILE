#pragma once
#include <Arduino.h>
#include <WiFiClient.h>
#include <vector>
#include <PubSubClient.h>
#include <Preferences.h>
#include "../config/config_includes.hpp"
#include "ClockConfig.hpp"
#include "ClockInfo.hpp"

class TIDILE;

class MQTTHandler
{
public:
    MQTTHandler();
    void setup(ClockConfig *config, Preferences *preferences, TIDILE *tidile, String uri, int port);
    void callback(char *topic, byte *payload, unsigned int length);

private:
    void subscribeTIDILETopics();
    void loop(ClockTime t);
    void subscribe(String topic);
    void publish(String topic, String payload);
    void reconnect();
    void subscribePrivate(String topic);
    void restore();

    bool handle(String topic, String payload);

private:
    ClockConfig *config;
    TIDILE *tidile;
    String uri;
    int port;
    WiFiClient wifiClient;
    PubSubClient *client;
    std::vector<String> subscribedTopics;
    Preferences *preferences;

    int startupSecs;
    int startupMins;
    int lastUpdateMin;

private:
    friend class TIDILE;
};