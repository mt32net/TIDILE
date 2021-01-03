#pragma once
#include "definements.hpp"
#include "ClockConfig.hpp"
#include "ESPAsyncWebServer.h"
#include "helper.hpp"

class TIDILE;

class Handler {
    public:
		Handler();
        void setup(ClockConfig *config, TIDILE *tidile, Preferences* preferences);
		void onColors(AsyncWebServerRequest *request);
        void onIndex(AsyncWebServerRequest *request);
        void onEnvColors(AsyncWebServerRequest *request);
        void onOther(AsyncWebServerRequest *request);
        void onNightTime(AsyncWebServerRequest *request);
        void onStyleSheet(AsyncWebServerRequest *request);
        void onManual(AsyncWebServerRequest *request);
        void onLamp(AsyncWebServerRequest *request);

        //void onEnv(AsyncWebServerRequest *request);
        //void onTimes(AsyncWebServerRequest *request);
    private:
        ClockConfig* config;
        TIDILE* tidile;
        Preferences* preferences;
        
};
