#pragma once
#include "definements.hpp"
#include "ClockConfig.hpp"
#include "TIDILE.hpp"
#include "ESPAsyncWebServer.h"

class Handler {
    public:
		Handler(ClockConfig *config, TIDILE *tidile);
		void onColors(AsyncWebServerRequest *request);
        void onIndex(AsyncWebServerRequest *request);
        void onCustom(AsyncWebServerRequest *request);
        //void onEnvColors(AsyncWebServerRequest *request);
        void onBlink(AsyncWebServerRequest *request);
        //void onEnv(AsyncWebServerRequest *request);
        //void onTimes(AsyncWebServerRequest *request);
    private:
        ClockConfig* config;
        TIDILE* tidile;
        int* adressPos = 0;
        Color hexToColor(String input);
        String colorToHEX(Color color);
};
