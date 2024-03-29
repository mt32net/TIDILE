#include "LEDController.hpp"

LEDController::LEDController(short numberLEDs, CRGB * leds, ClockConfig * config, short numberZones) {
    this->numberLeds = numberLEDs;
    this->leds = leds;
    this->config = config;
    this->numberZones = numberZones;

    this->multiplier = numberLEDs / numberZones;
}

void LEDController::setAll(Color color) {
    for (int i = 0; i < this->numberLeds; i++) {
        leds[i] = color.toCRGB();
    }
}

void LEDController::setZone(short index, Color color) {
    if (index < 0) return;
    for (int i = 0; i < multiplier; i++) {
        leds[mapIndex(index) + i] = color.toCRGB();
    }
}

short LEDController::mapIndex(short zoneIndex) {
    short i = zoneIndex;
    if (config->reverseDirection) {
        i = numberZones - 1 - i;
    }
    i *= multiplier;
    return i;
}