#include "MQTTHandler.hpp"

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:");
    Serial.write(payload, length);
    Serial.println();
}

MQTTHandler::MQTTHandler()
{
}

void MQTTHandler::setup(ClockConfig *config, TIDILE *tidile, String uri)
{
    this->config = config;
    this->tidile = tidile;
    this->uri = uri;
    this->client = new PubSubClient(wifiClient);

    client->setServer(MQTT_URI, MQTT_PORT);
    client->setCallback(callback);

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
            client->publish("sensors/test", "hello world");
            // ... and resubscribe
            // client.subscribe(MQTT_SERIAL_RECEIVER_CH);
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