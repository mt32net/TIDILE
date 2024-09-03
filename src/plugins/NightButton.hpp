//
// Created by Tom Arlt on 03.09.24.
//

#pragma once
#include "TIDILE_Plugin.hpp"
#include "debounce.hpp"

class NightButton: public TIDILE_Plugin {
public:
    void setup(ClockTime time) override;
    void loop(ClockTime time) override;

private:
    void handleNightButtonPress();
    Debouncer* nightButton = nullptr;
};
