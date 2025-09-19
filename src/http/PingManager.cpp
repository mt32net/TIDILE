#include "PingManager.hpp"

#include <time.hpp>

#include "esp_task_wdt.h"
#include "freertos/task.h"
#include "../config/compilation_varying.hpp"
#include "../TIDILE.hpp"

#ifdef FEATURE_PING

struct PingThreadData
{
    PingManager *ping{};
    ClockConfig *config{};
    std::vector<PresenceDevice> devicesList;
    bool anyOnline{};
};

static PingThreadData threadData;

static SemaphoreHandle_t xMutex = nullptr;

void devicesPingThread(void *args)
{
    Serial.println("THREAD: Starting thread...");
    std::vector<PresenceDevice> devices = {};
    Serial.println("THREAD: Device vector initialized.");
    for (String &s : threadData.config->presenceDeviceHostnames)
    {
        bool pingValue = Ping.ping(s.c_str(), 1);
        PresenceDevice d = {s, pingValue};
        devices.push_back(d);
        Serial.print(d.address);
        Serial.print("\t");
        Serial.println(d.online);
    }
    threadData.ping->registerPings(&devices);
    // Set flag if any device online
    bool flag = false;
    for (auto d : threadData.devicesList)
    {
        if (d.online)
        {
            flag = true;
            break;
        }
    }
    threadData.anyOnline = flag;
    Serial.println("THREAD: Stopping thread...");
    vTaskDelete(NULL);
}

void PingManager::updateDevices()
{
    static TaskHandle_t pingThreadHandler;
    Serial.println("Starting Device Presence Update...");
    xTaskCreate(devicesPingThread, "async_ping", 8192 * 2, NULL, 3, &pingThreadHandler);
    // pthread_create(&pingThreadID, NULL, devicesPingThread, NULL);
    Serial.println("Device Presence Update done.");
    // devicesPingThread(&this->threadData);
}

bool PingManager::isAnyDeviceOnline()
{
    /*     if (xSemaphoreTake(xMutex, portTICK_PERIOD_MS * 100) == pdTRUE) {
            for (auto device : threadData.devicesList) {
                if (device.online) return true;
            }
            xSemaphoreGive(xMutex);
        } else {
            Serial.println("Ping devices thread blocked resource for too long....");
        }
        return false; */
    return threadData.anyOnline;
}

void PingManager::registerPings(std::vector<PresenceDevice> *devices)
{
    // m.lock();
    Serial.println("THREAD: Writing to devices list...");
    if (xSemaphoreTake(xMutex, 100) == pdTRUE)
    {
        Serial.println("THREAD: Got mutex to write list...");
        threadData.devicesList.clear();
        for (auto device : *devices)
        {
            threadData.devicesList.push_back(device);
        }
        xSemaphoreGive(xMutex);
    }
    else
    {
        Serial.println("THREAD: Resource is still blocked by web server/main thread.");
    }
    // m.unlock();
}

void PingManager::setup(ClockTime time)
{
    threadData.ping = this;
    threadData.config = this->config;
    threadData.devicesList = {};
    threadData.anyOnline = true;
    xMutex = xSemaphoreCreateMutex();
}

void PingManager::loop(ClockTime time)
{
    tidile->setActive(!config->presenceDetection || isAnyDeviceOnline());
    unsigned long currentTimeHms = hmsToTimeInt(time);
    if (!config->presenceDetection)
        return;
    if (currentTimeHms - lastTimeChecked < intervalHms)
        return;
    // vTaskDelete(pingTask);
    updateDevices();
    lastTimeChecked = currentTimeHms;
}

std::vector<PresenceDevice> PingManager::getDevices()
{
    return threadData.devicesList;
}

#else
PingManager::PingManager(ClockConfig *config)
{
    Serial.println("PingManager disabled.");
}

void PingManager::updateDevices()
{
    Serial.println("PingManager disabled.");
}

void PingManager::loop(int currentTimeHms) {}

bool PingManager::isAnyDeviceOnline() { return true; }

void PingManager::registerPings(std::vector<PresenceDevice> *devices) {}

std::vector<PresenceDevice> PingManager::getDevices()
{
    return {};
}

#endif