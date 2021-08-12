#include "MQTTHandler.hpp"
#include "../TIDILE.hpp"
#include <ArduinoJson.h>
#include "topics/topicsInclude.hpp"
#include "../helper/time.hpp"

MQTTHandler::MQTTHandler()
{
}

void MQTTHandler::setup(ClockConfig *config, Preferences *preferences, TIDILE *tidile, String uri, int port)
{
    this->config = config;
    this->tidile = tidile;
    this->uri = uri;
    this->port = port;
    this->client = new PubSubClient(wifiClient);
    this->preferences = preferences;
    ClockTime t = getTime();
    this->startupMins = t.minutes;
    this->startupSecs = t.seconds;

    client->setBufferSize(512);

    client->setServer(uri.c_str(), (uint16_t)port);
    auto that = this;
    client->setCallback([that](char *topic, byte *payload, unsigned int length)
                        { that->callback(topic, payload, length); });

    reconnect();
    subscribeTIDILETopics();
    // register device
    DynamicJsonDocument doc(1024);
    doc["device_id"] = DEVICE_ID;
    doc["module_type"] = MT32_MODULE_NAME;
    doc["version"] = TIDILE_VERSION;
    char buffer[256];
    serializeJson(doc, buffer);
    client->publish(MQTT_TOPIC_DEVICES_REGISTER, buffer);
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
    /* String payloadStr = "";
    for (int i = 0; i < length; i++)
        payloadStr += String((char)payload[i]); */
    String payloadStr = String((char *)payload);
    if (!handle(topic, String(payloadStr)))
        tidile->mqttCallback(topic, payload, length);
}

void MQTTHandler::loop(ClockTime t)
{
    if (!client->connected())
    {
        reconnect();
    }
    client->loop();

    if ((t.minutes + startupMins) % 5 == 0 && (t.seconds + startupSecs) % 60 == 0)
    {
        publish(String(MQTT_TOPIC_META_RUNTIME), String(millis()));
        publish(String(MQTT_TOPIC_META_VERSION), String(TIDILE_VERSION));
    }
    else
    {
        delay(200);
    }
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
    client->publish(topic.c_str(), payload.c_str());
}

bool MQTTHandler::handle(String topic, String payload)
{
    if (tidile == nullptr)
    {
        Serial.println("TIDILE was null");
        return false;
    }
    if (payload.startsWith(MQTT_PAYLOAD_IGNORE_PREFIX))
    {
        Serial.println("MQTT payload ignored:");
        Serial.println(payload);
        Serial.println("---------------------");
        return true;
    }
    if (!payload.startsWith("{"))
    {
        Serial.println("JSON not recognized:");
        Serial.println(payload);
        return false;
    }
    DynamicJsonDocument doc(payload.length() * 1.5);
    deserializeJson(doc, payload.c_str());
    if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME))
    {
        NightTime night;
        night.deserializeFromJSON(doc);
        night.saveToConfig(config);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_COLORS))
    {
        Colors colors;
        colors.deserializeFromJSON(doc);
        colors.saveToConfig(config);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_GENERAL))
    {
        General gen;
        gen.deserializeFromJSON(doc);
        gen.saveToConfig(config);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_NIGHT_BEGIN_NOW))
    {
        config->tempOverwriteNightTime = true;
    }
    else
        return false;
    Serial.println("Save config");
    config->serialize(&tidile->preferences);
    //publish(topic, String(MQTT_PAYLOAD_IGNORE_PREFIX) + String(" ok"));
}

void MQTTHandler::subscribeTIDILETopics()
{
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME));
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_COLORS));
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_GENERAL));

    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_NIGHT_BEGIN_NOW));
}
