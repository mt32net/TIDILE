#pragma once

#include <WiFi.h>
#include <ESP32Ping.h>
#include <vector>
#include "ClockConfig.hpp"
#include <pthread.h>
#include "helper/mutex.hpp"

struct PresenceDevice {
    String address;
    bool online;
};

class PingManager;

struct PingThreadData {
    PingManager* ping;
    ClockConfig* config;
};

class PingManager {
    
    PresenceDevice* devicesLastChecked;
    int devicesLastCheckedSize;
    ClockConfig * config;
    int lastTimeChecked;
    int intervalHms;
    pthread_t pingThreadID;
    //Mutex m;
    SemaphoreHandle_t xMutex = NULL;
public:

    PingManager() = default;
    PingManager(ClockConfig *);
    void updateDevices();
    std::vector<PresenceDevice> getDevices();
    void loop(int currentTimeHms);
    bool isAnyDeviceOnline();
    void registerPings(std::vector<PresenceDevice> *devices);
};

