#pragma once

#include <WiFi.h>
#include <ESP32Ping.h>
#include <vector>
#include "ClockConfig.hpp"
#include <pthread.h>
#include "mutex.hpp"

struct PresenceDevice {
    String address;
    bool online;
};

class PingManager;

struct PingThreadData {
    PingManager* ping;
    ClockConfig* config;
    std::vector<PresenceDevice> devicesList;
    bool anyOnline;
};

class PingManager {
    
    //std::vector<PresenceDevice> devicesLastChecked;
    ClockConfig * config;
    int lastTimeChecked;
    int intervalHms;
    pthread_t pingThreadID;
    bool anyOnline;
    //Mutex m;
public:

    PingManager() = default;
    PingManager(ClockConfig *);
    void updateDevices();
    std::vector<PresenceDevice> getDevices();
    void loop(int currentTimeHms);
    bool isAnyDeviceOnline();
    void registerPings(std::vector<PresenceDevice> *devices);
};

