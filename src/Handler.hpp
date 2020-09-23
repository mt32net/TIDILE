#pragma once

#include "ClockConfig.hpp"
#include "Circle_Clock.hpp"
#include "ESPAsyncWebServer.h"

class Handler {
    private:
        ClockConfig* config;
        CircleClock* clock;
    public:
		Handler(ClockConfig *config, CircleClock *clock);
		void onColors(AsyncWebServerRequest *request);
        void onIndex(AsyncWebServerRequest *request);
        void onCustom(AsyncWebServerRequest *request);
        //void onEnvColors(AsyncWebServerRequest *request);
        void onBlink(AsyncWebServerRequest *request);
        //void onEnv(AsyncWebServerRequest *request);
        //void onTimes(AsyncWebServerRequest *request);
};
