#pragma once
#include "definements.hpp"
#include "config.h"
#include "ClockConfig.hpp"
#include "TIDILE.hpp"
#include "ESPAsyncWebServer.h"

class Handler {
    public:
		Handler(ClockConfig *config, TIDILE *tidile, Preferences* preferneces);
		void onColors(AsyncWebServerRequest *request);
        void onIndex(AsyncWebServerRequest *request);
        void onCustom(AsyncWebServerRequest *request);
        void onEnvColors(AsyncWebServerRequest *request);
        void onBlink(AsyncWebServerRequest *request);
        void onNightTime(AsyncWebServerRequest *request);
        //void onEnv(AsyncWebServerRequest *request);
        //void onTimes(AsyncWebServerRequest *request);
    private:
        ClockConfig* config;
        TIDILE* tidile;
        Preferences* preferences;
        Color hexToColor(String input);
        String colorToHex(Color color);
        String timeIntToTimeString(int timeInt);
        int timeStringToTimeInt(String timeString);
};
