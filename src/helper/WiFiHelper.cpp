#include "WiFiHelper.hpp"
#include "../config/config_includes.hpp"
#include "esp_wifi.h"
#include <WiFi.h>

void WiFiHelper::connectWiFi()
{

    this->apMode = false;
    Serial.println("Initialising WiFi...");
    // TODO back in
    WiFi.enableIpV6();
    // WiFi.disconnect();
    WiFi.setAutoReconnect(true);
    WiFi.begin();
    // WiFi.enableLongRange(true);
    delay(500);

#ifndef WIFI_SSID
    // WiFi.beginSmartConfig();
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
        // Serial.println(WiFi.smartConfigDone());
#endif
        tries++;
        if (tries > WIFI_NUMBER_TRIES_BEFORE_AP)
        {
            Serial.println();
            Serial.println("Could not establish connection, opening access point...");
            openAP("TIDILEAP");
            return;
        }
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WiFiHelper::setCredentials(String ssid, String password) {
    WiFi.persistent(true);
    Serial.println("Trying to connect to " + ssid + "...");
    WiFi.begin(ssid.c_str(), password.c_str());
    int tries = 0;
    // TODO Not working!
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        tries++;
        Serial.print(".");
        if (tries > WIFI_NUMBER_TRIES_BEFORE_AP)
        {
            Serial.println();
            Serial.println("Could not establish connection, doing nothing...");
            return;
        }
    }
    Serial.println();
    Serial.println("Connected to " + ssid + ". Restarting...");
    delay(100);
    ESP.restart();
}

bool WiFiHelper::connectedWiFi()
{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiHelper::setHostname(String name)
{
    WiFi.setHostname(name.c_str());
}

void WiFiHelper::openAP(String name)
{
    // WiFi.stopSmartConfig();
    this->apMode = true;
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    setHostname("tidile");
    dnsServer.start(DEFAULT_DNS_PORT, "*", apIP);
    WiFi.softAPConfig(apIP, apIP, AP_DEFAULT_SUBNET_MASK);
    WiFi.softAP("TIDILE", NULL);
    delay(500);
    IPAddress ownIp = WiFi.softAPIP();
    Serial.print("TIDILE AP IP: ");
    Serial.println(ownIp);
}

std::vector<Network> WiFiHelper::getReachableNets() {
    std::vector<Network> nets = {};
    int n = WiFi.scanNetworks(false, false, false, 80U);
    for(int i = 0; i < n; i++) {
        auto net = Network {
            ssid: WiFi.SSID(i),
            rssi: WiFi.RSSI(i),
        };
        nets.push_back(net);
    }
    return nets;
}

Network WiFiHelper::getCurrentNetwork() {
    return Network {
        ssid: WiFi.SSID(),
        rssi: (float)WiFi.RSSI(),
    };
}