#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>

#define AP_DEFAULT_SUBNET_MASK IPAddress(255, 255, 255, 0)
#define DEFAULT_DNS_PORT 53
class WiFiHelper
{
public:
    void connectWiFi();
    bool connectedWiFi();
    void setHostname(String name);
    void openAP(String name);
    void setCredentials(String ssid, String password);
    bool isAPMode() { return apMode; };

private:
    IPAddress apIP = IPAddress(192, 168, 4, 1);
    DNSServer dnsServer;
    bool apMode = false;
};
