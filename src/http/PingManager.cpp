#include "PingManager.hpp"

PingManager::PingManager(ClockConfig * config) {
    this->devicesLastCheck = {};
    this->config = config;
    this->lastTimeChecked = 0;
    this->intervalHms = DEFAULT_PRESENCE_INTERVAL;
    this->threadData = {this, this->config};
}

void pingThread(void* args) {
    std::vector<PresenceDevice> devices = {};
    PingThreadData* data = (PingThreadData*) args; 
    for (String &s : data->config->presenceDeviceHostnames) {
        PresenceDevice d = {
            s,
            Ping.ping(s.c_str(), 1),
        };
        devices.push_back(d);
        Serial.print(d.address);
        Serial.print("\t");
        Serial.println(d.online);
    }
    data->ping->registerPings(devices);
}

void PingManager::updateDevices() {
    if(pingTask != NULL) Serial.println(eTaskGetState(pingTask));
    xTaskCreate(pingThread, "ping thread", 2048, &this->threadData, 6, &pingTask);
    allDevices.clear();
    for (auto& dev : config->presenceDeviceHostnames) {
        allDevices.push_back({ .address = dev, .online = false });
    }*/
    //pingThread(&this->threadData);
}

std::vector<PresenceDevice> PingManager::getDevices() {
    std::vector<PresenceDevice> devices = {};
    // m.lock();
    devices.insert(devices.begin(), devicesLastCheck.begin(), devicesLastCheck.end());
    // m.unlock();
    return devices;
}

void PingManager::loop(int currentTimeHms) {
    if (!config->presenceDetection) return;
    if (currentTimeHms - lastTimeChecked < intervalHms) return;
    //vTaskDelete(pingTask);
    updateDevices();
    lastTimeChecked = currentTimeHms;
}

bool PingManager::isAnyDeviceOnline() {
    for (auto &d : devicesLastCheck) {
        if (d.online) return true;
    }
    return false;
}

void PingManager::registerPings(std::vector<PresenceDevice> &devices) {
    // m.lock();
    this->devicesLastCheck.clear();
    for(auto d : devices) {
        this->devicesLastCheck.push_back(d);
    }
    // m.unlock();
}
