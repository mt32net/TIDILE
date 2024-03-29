#pragma once
#include "confidential.hpp"
#include "general.hpp"

#define MQTT_TOPIC_DEVICES_REGISTER "mt32/devices/register"
#define MQTT_TOPIC_META "mt32/devices/meta"

#define __MQTT_TOPIC_TIDILE_PREFIX "mt32/" DEVICE_ID "/" MT32_MODULE_NAME
#define __MQTT_TOPIC_TIDILE_CONFIG_PREFIX __MQTT_TOPIC_TIDILE_PREFIX "/config"

#define MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME __MQTT_TOPIC_TIDILE_CONFIG_PREFIX "/nighttime"
#define MQTT_TOPIC_TIDILE_CONFIG_NIGHT_BEGIN_NOW __MQTT_TOPIC_TIDILE_CONFIG_PREFIX "/beginNightNow"
#define MQTT_TOPIC_TIDILE_CONFIG_COLORS __MQTT_TOPIC_TIDILE_CONFIG_PREFIX "/colors"
#define MQTT_TOPIC_TIDILE_CONFIG_GENERAL __MQTT_TOPIC_TIDILE_CONFIG_PREFIX "/general"

#define MQTT_PAYLOAD_IGNORE_PREFIX "ignore"

#define MQTT_META_PUSH_MINUTE_INTERVAL 5