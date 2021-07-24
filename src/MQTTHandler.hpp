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
    void setup(ClockConfig *config, TIDILE *tidile, String uri);

private:
    void reconnect();

private:
    ClockConfig *config;
    TIDILE *tidile;
    String uri;
    WiFiClient wifiClient;
    PubSubClient *client;
};