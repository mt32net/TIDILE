#pragma once
#include "../definements.hpp"
#include <Arduino.h>
#include <mqtt_client.h>
#include <mqtt_config.h>

//example code https://github.com/espressif/esp-idf/blob/166c30e7b2ed1dcaae56179329540a862915208a/examples/protocols/mqtt/tcp/main/app_main.c
//API Ref https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mqtt.html

/**
 * @brief Helper class for Handling MQTT on the ESP32
 * 
 */
class MQTTHelper
{
public:
    MQTTHelper();
    MQTTHelper(String uri);
    void setURI(String uri);
    void init();

private:
    String uri = "";
    esp_mqtt_client_config_t mqtt_cfg;
    esp_mqtt_client_handle_t client;

private:
    friend static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
};