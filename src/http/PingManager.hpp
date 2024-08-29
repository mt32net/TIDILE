#pragma once

#include <WiFi.h>
#include <ESP32Ping.h>
#include <vector>
#include "ClockConfig.hpp"
#include "freertos/task.h"
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
    
    std::vector<PresenceDevice> devicesLastCheck;
    ClockConfig * config;
    int lastTimeChecked;
    int intervalHms;
    TaskHandle_t pingTask = NULL;
    // Mutex m;
public:

    PingManager() = default;
    PingManager(ClockConfig *);
    void updateDevices();
    std::vector<PresenceDevice> getDevices();
    void loop(int currentTimeHms);
    bool isAnyDeviceOnline();
    void registerPings(std::vector<PresenceDevice> &devices);
private:
    PingThreadData threadData = {nullptr, nullptr};
};

