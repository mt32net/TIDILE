#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include "vector"


#define AP_DEFAULT_SUBNET_MASK IPAddress(255, 255, 255, 0)
#define DEFAULT_DNS_PORT 53

struct NetworkInfo {
    String ssid;
    int32_t rssi;
};

class WiFiHelper
{
public:
    void connectWiFi();
    bool connectedWiFi();
    void setHostname(String name);
    void openAP(String name);
    void setCredentials(String ssid, String password);
    bool isAPMode() { return apMode; };
    std::vector<NetworkInfo> getReachableNets();
    NetworkInfo getCurrentNetwork();

private:
    IPAddress apIP = IPAddress(192, 168, 4, 1);
    DNSServer dnsServer;
    bool apMode = false;
};
