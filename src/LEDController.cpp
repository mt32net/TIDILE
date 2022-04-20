#include "LEDController.hpp"

LEDController::LEDController(short numberLEDs, CRGB * leds, ClockConfig * config, short numberZones) {
    this->numberLeds = numberLEDs;
    this->leds = leds;
    this->config = config;
    this->numberZones = numberZones;

    this->multiplier = numberLEDs / numberZones;
}

void LEDController::setZone(short index, Color color) {
    for (int i = 0; i < multiplier; i++) {
        leds[mapIndex(index) + i] = color.toCRGB();
    }
}

short LEDController::mapIndex(short zoneIndex) {
    short i = zoneIndex;
    i *= multiplier;
    if (config->reverseDirection) {
        // TODO check
        i = numberLeds - 1 - i;
    }
    return i;
}