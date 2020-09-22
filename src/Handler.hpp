#pragma once

#include "ClockConfig.hpp"
#include "ESPAsyncWebServer.h"

class Handler {
    private:
        ClockConfig* config;
    public:
		Handler(ClockConfig *config);
		void onColors(AsyncWebServerRequest *request);
};
