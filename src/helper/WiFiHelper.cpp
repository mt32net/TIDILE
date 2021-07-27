#include "WiFiHelper.hpp"
#include "../config/config_includes.hpp"
#include <WiFi.h>

void connectWiFi()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Initialising WiFi config");
        WiFi.mode(WIFI_STA);
        WiFi.enableIpV6();
        WiFi.disconnect();
        WiFi.setAutoReconnect(true);
        WiFi.enableLongRange(true);
        delay(500);

#ifndef WIFI_SSID
        WiFi.beginSmartConfig();
#else
        Serial.println("Setting credentials");
        WiFi.begin(WIFI_SSID, WIFI_PWD);
#endif

        Serial.println("Waiting for connection");
        int tries = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
#ifndef WIFI_SSID
            Serial.print(".");
            Serial.println(WiFi.smartConfigDone());
#endif
            tries++;
            if (tries > 20)
            {
                Serial.println("Could not establish connection, restarting...");
                ESP.restart();
            }
        }
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

bool connectedWiFi()
{
    return WiFi.status() == WL_CONNECTED;
}

void setHostname(String name)
{
    WiFi.setHostname(name.c_str());
}