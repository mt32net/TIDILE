#include "PingManager.hpp"
#include "esp_task_wdt.h"
#include "freertos/task.h"

static PingThreadData threadData;

PingManager::PingManager(ClockConfig * config) {
    this->devicesLastCheckedSize = config->presenceDeviceHostnames.size();
    this->devicesLastChecked = (PresenceDevice*) calloc(this->devicesLastCheckedSize, sizeof(PresenceDevice));
    this->config = config;
    this->lastTimeChecked = 0;
    this->intervalHms = DEFAULT_PRESENCE_INTERVAL;
    threadData = {this, this->config};
    xMutex = xSemaphoreCreateMutex();
}

void devicesPingThread(void* args) {
    Serial.println("THREAD: Starting thread...");
    std::vector<PresenceDevice> devices = {};
    Serial.println("THREAD: Device vector initialized.");
    for (String &s : threadData.config->presenceDeviceHostnames) {
        // TODO revert
        bool pingValue = true; //Ping.ping(s.c_str(), 1);
        PresenceDevice d = { s, pingValue };
        devices.push_back(d);
        Serial.print(d.address);
        Serial.print("\t");
        Serial.println(d.online);
    }
    threadData.ping->registerPings(&devices);
    Serial.println("THREAD: Stopping thread...");
    vTaskDelete(NULL);
}

void PingManager::updateDevices() {
    static TaskHandle_t pingThreadHandler;
    Serial.println("Starting Device Presence Update...");
    xTaskCreate(devicesPingThread, "async_ping", 8192 * 2, NULL, 3, &pingThreadHandler);
    //pthread_create(&pingThreadID, NULL, devicesPingThread, NULL);
    Serial.println("Device Presence Update done.");
    //devicesPingThread(&this->threadData);
}

void PingManager::loop(int currentTimeHms) {
    if (!config->presenceDetection) return;
    if (currentTimeHms - lastTimeChecked < intervalHms) return;
    //vTaskDelete(pingTask);
    updateDevices();
    lastTimeChecked = currentTimeHms;
}

bool PingManager::isAnyDeviceOnline() {
    //m.lock();
    for (int i = 0; i < this->devicesLastCheckedSize; i++) {
        if (devicesLastChecked[i].online) return true;
    }
    //m.unlock();
    return false;
}

void PingManager::registerPings(std::vector<PresenceDevice>* devices) {
    //m.lock();
    //this->devicesLastCheck.clear();
    //delete devicesLastChecked;
    //devicesLastCheckedSize = devices->size();
    //devicesLastChecked = (PresenceDevice*) calloc(devicesLastCheckedSize, sizeof(PresenceDevice));
    for(int i = 0; i < devicesLastCheckedSize; i++) {
        //this->devicesLastCheck.push_back(d);
        devicesLastChecked[i] = (*devices)[i];
    }
    //m.unlock();
}

std::vector<PresenceDevice> PingManager::getDevices() {
    return std::vector<PresenceDevice>(this->devicesLastChecked, this->devicesLastChecked + this->devicesLastCheckedSize);

}