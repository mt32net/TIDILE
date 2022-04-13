#pragma once

#define TIDILE_VERSION "1.0"

#define ntpServer "pool.ntp.org"

#define MT32_MODULE_NAME "tidile"
#define HTTP_ENDPOINT_PORT 80
#define STARTUP_ANIMATION_DELAY 8

#define LED_PIN 5
#define LED_COUNT 144
#define BLINK_BRIGHTNESS 0.7

#define CONFIG_FILE_NAME "/config.json"
#define ACCESS_POINT_SSID "TIDILE-AP"
#define WIFI_NUMBER_TRIES_BEFORE_AP 5

// #define TIDILE_LOGERR

#define LED_COUNT_FOR_ONE_SECOND(ledCount) (int)((double)(ledCount) / 60.0)
// #define DIMM_ADD_VALUE 0.7