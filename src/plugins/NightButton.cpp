//
// Created by Tom Arlt on 03.09.24.
//
#include "NightButton.hpp"

#include <time.hpp>
#include "config/general.hpp"

void NightButton::loop(ClockTime time) {
    resetOverwriteNightTimeIfLegit(*config, time);
}

void NightButton::setup(ClockTime time) {
    auto callback = [this](){
        this->handleNightButtonPress();
    };
    this->nightButton = new Debouncer((gpio_num_t) NIGHT_BUTTON_PIN, callback);
}

void NightButton::handleNightButtonPress()
{
    this->config->tempOverwriteNightTime = !this->config->tempOverwriteNightTime;
}

void resetOverwriteNightTimeIfLegit(ClockConfig &config, ClockTime time) {
    int currentTime = hmsToTimeInt(time);
    // only check on different seconds value
    if (currentTime == config.lastNightTimeOverwriteCheckTime) return;
    // (*100 to accommodate different precisions)
    if (currentTime >= config.nightTimeEnd * 100 && config.lastNightTimeOverwriteCheckTime - 2 < config.nightTimeEnd *
        100) {
        config.tempOverwriteNightTime = false;
        }
    config.lastNightTimeOverwriteCheckTime = currentTime;
}
