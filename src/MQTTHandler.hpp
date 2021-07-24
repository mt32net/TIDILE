#pragma once
#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "definements.hpp"
#include "ClockConfig.hpp"

class TIDILE;

class MQTTHandler
{
public:
    MQTTHandler();
    void setup(ClockConfig *config, TIDILE *tidile, String uri, int port);
    void callback(char *topic, byte *payload, unsigned int length);
    void loop();
    void subscribe(String topic);
    void publish(String topic, String payload);
private:
    void reconnect();

private:
    ClockConfig *config;
    TIDILE *tidile;
    String uri;
    int port;
    WiFiClient wifiClient;
    PubSubClient *client;
};