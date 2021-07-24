#pragma once
#include "Arduino.h"
#include <WiFi.h>
#include "wifi_config.hpp"

void connectWiFi();

bool connectedWiFi();

void setHostname(String name);