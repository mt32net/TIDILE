#pragma once
#ifndef CONFIDENTIAL_HPP

#pragma error Duplicate the config / example_confidential.hpp to config / confidential.hpp and change the values in there
//in the newly created "confidential.hpp" remove the line above and remove the second (2) and last line in this file

#ifndef CONFIDENTIAL_HPP
#define CONFIDENTIAL_HPP
#endif

#define WIFI_SSID "yourSSID"
#define WIFI_PWD "yourPassword"

#define MQTT_URI "your server url"
#define MQTT_PORT 1883 //default mqtt port
#define MQTT_USER "your mqtt user"
#define MQTT_PWD "the corresponding pwassord"

#define DEVICE_ID "your custom device id"
#endif