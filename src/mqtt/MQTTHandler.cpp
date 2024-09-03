#include "MQTTHandler.hpp"
#include "../TIDILE.hpp"
//#define ARDUINOJSON_ENABLE_COMMENTS 1
#include <ArduinoJson.h>

#include <utility>
#include "../topics/topicsInclude.hpp"

MQTTHandler::MQTTHandler() = default;

#ifdef FEATURE_MQTT

MQTTHandler *MQTTHandler::setupInternal(String uri, int port)
{
    this->uri = std::move(uri);
    this->port = port;
    return this;
}

void MQTTHandler::setup(ClockTime currentTime) {
    this->client = new PubSubClient(wifiClient);
    ClockTime t = currentTime;
    this->startupMins = t.minutes;
    this->startupSecs = t.seconds;
    this->connectionAttempts = 0;

    client->setBufferSize(512);

    client->setServer(uri.c_str(), (uint16_t)port);
    auto that = this;
    client->setCallback([that](char *topic, byte *payload, unsigned int length)
                        { that->callback(topic, payload, length); });

    reconnect();
    subscribeTIDILETopics();
    // register device
    JsonDocument doc;
    doc[JSON_NAME_META_DEVICE_ID] = DEVICE_ID;
    doc[JSON_NAME_META_MODULE_TYPE] = MT32_MODULE_NAME;
    doc[JSON_NAME_META_VERSION] = TIDILE_VERSION;
    String registerStr;
    serializeJsonPretty(doc, registerStr);
    publish(MQTT_TOPIC_DEVICES_REGISTER, registerStr);
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
            Serial.print(" try again in 0.1 seconds with try nr. ");
            this->connectionAttempts++;
            Serial.println(this->connectionAttempts);
            // Wait 5 seconds before retrying
            delay(100);
        }
        if (connectionAttempts >= 10)
        {
            Serial.println("Could not connect to MQTT server");
            break;
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
    if (!handle(topic, String(payloadStr))) {
        Serial.println("-------new message from broker-----");
        Serial.print("channel:");
        Serial.println(topic);
        Serial.print("data:");
        Serial.write(payload, length);
        Serial.println();
    }
}

void MQTTHandler::loop(ClockTime t)
{
    if (!client->connected())
    {
        reconnect();
    }
    client->loop();

    if ((t.minutes + startupMins) % MQTT_META_PUSH_MINUTE_INTERVAL == 0 && (t.seconds + startupSecs) % 60 == 0 && lastUpdateMin != t.minutes)
    {
        JsonDocument metaData;
        metaData[JSON_NAME_META_DEVICE_ID] = DEVICE_ID;
        metaData[JSON_NAME_META_VERSION] = TIDILE_VERSION;
        metaData[JSON_NAME_META_UPTIME] = millis();

        String out;
        serializeJsonPretty(metaData, out);
        publish(String(MQTT_TOPIC_META), out);
        lastUpdateMin = t.minutes;
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
    JsonDocument doc;
    deserializeJson(doc, payload.c_str());
    if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME))
    {
        NightTimeTopic night;
        night.deserializeFromJSON(doc);
        night.saveToConfig(config);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_COLORS))
    {
        ColorsTopic colors;
        colors.deserializeFromJSON(doc);
        colors.saveToConfig(config);
    }
    else if (topic == String(MQTT_TOPIC_TIDILE_CONFIG_GENERAL))
    {
        GeneralTopic gen;
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
    // config->serialize(&tidile->preferences);
    // publish(topic, String(MQTT_PAYLOAD_IGNORE_PREFIX) + String(" ok"));

    return true;
}

void MQTTHandler::subscribeTIDILETopics()
{
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME));
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_COLORS));
    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_GENERAL));

    subscribe(String(MQTT_TOPIC_TIDILE_CONFIG_NIGHT_BEGIN_NOW));
}
#else
MQTTHandler * MQTTHandler::setupInternal(String uri, int port) {
}

void MQTTHandler::setup(ClockTime time) {
    TIDILE_Plugin::setup(time);
}

void MQTTHandler::callback(char *topic, byte *payload, unsigned int length) {
}

void MQTTHandler::subscribeTIDILETopics() {
}

void MQTTHandler::loop(ClockTime t) {;
}

void MQTTHandler::subscribe(String topic) {
}

void MQTTHandler::publish(String topic, String payload) {
}

void MQTTHandler::reconnect() {
}

void MQTTHandler::subscribePrivate(String topic) {
}

void MQTTHandler::restore() {
}

bool MQTTHandler::handle(String topic, String payload) {
    return false;
}
#endif
