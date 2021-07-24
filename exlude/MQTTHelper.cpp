#include "MQTTHelper.hpp"

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
}

MQTTHelper::MQTTHelper()
{
}

MQTTHelper::MQTTHelper(String uri)
{
    setURI(uri);
}

void MQTTHelper::setURI(String uri)
{
    if (uri.startsWith("mqtt://"))
    {
        uri = (String("mqtt://") + uri).c_str();
    }
    this->uri = uri;
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = this->uri.c_str(),
        // .user_context = (void *)your_context
    };
    this->mqtt_cfg = mqtt_cfg;
}

void MQTTHelper::init()
{
    this->client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}