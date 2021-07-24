#include "MQTTHandler.hpp"
#include "TIDILE.hpp"

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
    client->setCallback([that](char *topic, byte *payload, unsigned int length)
                        { that->callback(topic, payload, length); });

    reconnect();
}

void MQTTHandler::reconnect()
{
    // Loop until we're reconnected
    while (!client->connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client->connect(clientId.c_str(), MQTT_USER, MQTT_PWD))
        {
            Serial.println("connected");
            //Once connected, publish an announcement...
            publish("sensors/test", "hello world");
            // ... and resubscribe
            subscribe("sensors/test");
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

void MQTTHandler::callback(char *topic, byte *payload, unsigned int length)
{
    tidile->mqttCallback(topic, payload, length);
}

void MQTTHandler::loop()
{
    if (!client->connected())
    {
        reconnect();
    }
    client->loop();
}

void MQTTHandler::subscribe(String topic)
{
    client->subscribe(topic.c_str());
}

void MQTTHandler::publish(String topic, String payload)
{
    client->publish(topic.c_str(), payload.c_str());
}