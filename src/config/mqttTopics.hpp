#pragma once

#define __MQTT_TOPIC_DEVICES "devices"
#define MQTT_TOPIC_DEVICES_REGISTER __MQTT_TOPIC_DEVICES "/register"

#define __MQTT_TOPIC_META "meta"
#define __MQTT_TOPIC_META_DEVICES __MQTT_TOPIC_META "/devices"
#define __MQTT_TOPIC_META_DEVICES_ID __MQTT_TOPIC_META_DEVICES "/" DEVICE_ID
#define MQTT_TOPIC_META_RUNTIME __MQTT_TOPIC_META_DEVICES_ID "/runtime"
#define MQTT_TOPIC_META_VERSION __MQTT_TOPIC_META_DEVICES_ID "/version"

#define __MQTT_TOPIC_MODULES "modules"
#define __MQTT_TOPIC_MODULES_TIDILE __MQTT_TOPIC_MODULES "/tidile"
#define __MQTT_TOPIC_MODULES_TIDILE_DEVICES __MQTT_TOPIC_MODULES_TIDILE "/devices"
#define __MQTT_TOPIC_MODULES_TIDILE_DEVICES_ID __MQTT_TOPIC_MODULES_TIDILE_DEVICES "/" DEVICE_ID
#define __MQTT_TOPIC_MODULES_TIDILE_DEVICES_ID_CONFIG __MQTT_TOPIC_MODULES_TIDILE_DEVICES_ID "/config"

#define MQTT_TOPIC_TIDILE_CONFIG_NIGHTTIME __MQTT_TOPIC_MODULES_TIDILE_DEVICES_ID_CONFIG "/nighttime"
#define MQTT_TOPIC_TIDILE_CONFIG_NIGHT_BEGIN_NOW __MQTT_TOPIC_MODULES_TIDILE_DEVICES_ID_CONFIG "/beginNightNow"
#define MQTT_TOPIC_TIDILE_CONFIG_COLORS __MQTT_TOPIC_MODULES_TIDILE_DEVICES_ID_CONFIG "/colors"
#define MQTT_TOPIC_TIDILE_CONFIG_GENERAL __MQTT_TOPIC_MODULES_TIDILE_DEVICES_ID_CONFIG "/general"