#pragma once

#include "config/compilation_varying.hpp"
#ifdef FEATURE_PING
#include <WiFi.h>
#include <ESP32Ping.h>
#endif

#include <vector>
#include <plugins/TIDILE_Plugin.hpp>
#include "ClockConfig.hpp"


struct PresenceDevice {
    String address;
    bool online;
};

class PingManager: public TIDILE_Plugin {
public:
    bool displayAnything() override;

private:
    //std::vector<PresenceDevice> devicesLastChecked;
    unsigned long lastTimeChecked = 0;
    unsigned long intervalHms = DEFAULT_PRESENCE_INTERVAL;
    pthread_t pingThreadID{};
    bool anyOnline{};
    //Mutex m;
public:

    PingManager() = default;
    void updateDevices();
    std::vector<PresenceDevice> getDevices();
    bool isAnyDeviceOnline();
    void registerPings(std::vector<PresenceDevice> *devices);

    void setup(ClockTime time) override;
    void loop(ClockTime time) override;
};