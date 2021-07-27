#include "MQTTHandler.hpp"
#include "../TIDILE.hpp"
#include <ArduinoJson.h>
#include "topics/topicsInclude.hpp"

MQTTHandler::MQTTHandler()
{
}

void MQTTHandler::setup(ClockConfig *config, TIDILE *tidile, String uri, int port)
{
    this->config = config;
    this->tidile = tidile;
    this->uri = uri;
    this->port = port;
    this->client = new PubSubClient(wifiClient);

    client->setServer(uri.c_str(), (uint16_t)port);
    auto that = this;
    client->setCallback([that](char *topic, byte *payload, unsigned int length) { that->callback(topic, payload, length); });

    reconnect();
    subscribeTIDILETopics();
}

void MQTTHandler::reconnect()
{
    // Loop until we're reconnected
    while (!client->connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        // Attempt to connect
        if (client->connect(DEVICE_ID, MQTT_USER, MQTT_PWD))
        {
            Serial.println("connected");
            restore();
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void MQTTHandler::restore()
{
    for (auto &topic : this->subscribedTopics)
    {
        subscribePrivate(topic);
    }
}

void MQTTHandler::callback(char *topic, byte *payload, unsigned int length)
{
    String payloadStr = "";
    for (int i = 0; i < length; i++)
        payloadStr += payload[i];
    if (!handle(topic, String(payloadStr)))
        tidile->mqttCallback(topic, payload, length);
}

void MQTTHandler::loop()
{
    if (!client->connected())
    {
        reconnect();
    }
    client->loop();

    publish(String(MQTT_TOPIC_META_RUNTIME), String(millis()));
    publish(String(MQTT_TOPIC_META_VERSION), String(TIDILE_VERSION));
}

void MQTTHandler::subscribe(String topic)
{
    subscribedTopics.push_back(topic);
    subscribePrivate(topic);
}

void MQTTHandler::subscribePrivate(String topic)
{
    client->subscribe(topic.c_str());
}

void MQTTHandler::publish(String topic, String payload)
{
    /*  Serial.print("Publishing to ");
    Serial.print(topic);
    Serial.print("\t\t");
    Serial.println(payload); */
    client->publish(topic.c_str(), payload.c_str());
}

bool MQTTHandler::handle(String topic, String payload)
{
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload.c_str());
    if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME))
    {
        NightTime night;
        night.deserializeFromJSON(doc);
        night.saveToConfig(&tidile->configuration);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_COLORS))
    {
        Colors colors;
        colors.deserializeFromJSON(doc);
        colors.saveToConfig(&tidile->configuration);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_GENERAL))
    {
        General gen;
        gen.deserializeFromJSON(doc);
        gen.saveToConfig(&tidile->configuration);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_NIGHT_BEGIN_NOW))
    {
        tidile->configuration.tempOverwriteNightTime = true;
    }
    else
    {
        tidile->configuration.serialize(&tidile->preferences);
        return false;
    }
    return true;
}

void MQTTHandler::subscribeTIDILETopics()
{
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME));
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_COLORS));
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_GENERAL));

    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_NIGHT_BEGIN_NOW));
}
