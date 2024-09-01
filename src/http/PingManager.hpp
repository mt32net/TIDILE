#pragma once

#include "config/compilation_varying.hpp"
#ifdef FEATURE_PING
#include <WiFi.h>
#include <ESP32Ping.h>
#endif
#include <vector>
#include "ClockConfig.hpp"


struct PresenceDevice {
    String address;
    bool online;
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