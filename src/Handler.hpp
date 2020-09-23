#pragma once

#include "ClockConfig.hpp"
#include "TIDILE.hpp"
#include "ESPAsyncWebServer.h"

class Handler {
    private:
        ClockConfig* config;
        TIDILE* tidile;
        Color hexToColor(String input);
    public:
		Handler(ClockConfig *config, TIDILE *tidile);
		void onColors(AsyncWebServerRequest *request);
        void onIndex(AsyncWebServerRequest *request);
        void onCustom(AsyncWebServerRequest *request);
        //void onEnvColors(AsyncWebServerRequest *request);
        void onBlink(AsyncWebServerRequest *request);
        //void onEnv(AsyncWebServerRequest *request);
        //void onTimes(AsyncWebServerRequest *request);
};
