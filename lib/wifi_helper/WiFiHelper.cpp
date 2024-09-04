#include "WiFiHelper.hpp"
#include <WiFi.h>

#if SOC_WIFI_SUPPORTED
void WiFiHelper::connectWiFi(const char* ssid, const char* passwd, int maxRetriesBeforeAP)
{
    this->apMode = false;
    Serial.println("Initialising WiFi...");
    WiFi.enableIpV6();
    // WiFi.disconnect();
    WiFi.setAutoReconnect(true);
    WiFi.begin();
    // WiFi.enableLongRange(true);
    delay(500);

    if(ssid == nullptr)
    WiFi.beginSmartConfig();
    else {
        Serial.println("Setting credentials");
        WiFi.begin(ssid, passwd);
    }

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
        if (tries > maxRetriesBeforeAP)
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

void WiFiHelper::setCredentials(String ssid, String password, int maxRetriesBeforeAP) {
    WiFi.persistent(true);
    Serial.println("Trying to connect to " + ssid + "...");
    WiFi.begin(ssid.c_str(), password.c_str());
    int tries = 0;
    // TODO Not working!
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        tries++;
        Serial.print(".");
        if (tries > maxRetriesBeforeAP)
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

std::vector<NetworkInfo> WiFiHelper::getReachableNets() {
    std::vector<NetworkInfo> nets = {};
    int n = WiFi.scanNetworks(false, false, false, 150U);
    for(int i = 0; i < n; i++) {
        auto net = NetworkInfo {\
            WiFi.SSID(i),
            WiFi.RSSI(i),
        };
        nets.push_back(net);
    }
    return nets;
}

NetworkInfo WiFiHelper::getCurrentNetwork() {
    return NetworkInfo {
        WiFi.SSID(),
        WiFi.RSSI(),
    };
}

WiFiHelper * WiFiHelper::instance = nullptr;

WiFiHelper * WiFiHelper::getInstance() {
    if (WiFiHelper::instance == nullptr) {
        WiFiHelper::instance = new WiFiHelper();
    }
    return WiFiHelper::instance;
}

WiFiHelper::WiFiHelper() {
}

#else
void WiFiHelper::connectWiFi(int maxRetriesBeforeAP) {
}

bool WiFiHelper::connectedWiFi() {
    return false;
}

void WiFiHelper::setHostname(String name) {
}

void WiFiHelper::openAP(String name) {
}

void WiFiHelper::setCredentials(String ssid, String password, int maxRetriesBeforeAP) {
}

std::vector<NetworkInfo> WiFiHelper::getReachableNets() {
    return {};
}

NetworkInfo WiFiHelper::getCurrentNetwork() {
    return NetworkInfo{
        "", 0
    };
}
#endif
