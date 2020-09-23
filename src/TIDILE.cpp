#include "TIDILE.hpp"

TIDILE::TIDILE() {
    
}

void TIDILE::setup(CRGB leds[NUM_LEDS], int numberLEDs, ClockConfig *configuration){
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->configuration = configuration;
}

int TIDILE::mapToLEDs(int value, int max) {
    return map(value, 0, max, 0, this->numberLEDs);
}

void TIDILE::clear() {
    for (int i = 0; i < this->numberLEDs; i++) {
        this->leds[i] = CRGB::Black;
    }
}

void TIDILE::displayTime(ClockTime time) {

    clear();

    // Minutes
    for (int i = 0; i < mapToLEDs(time.minutes, 60); i++) {
        this->leds[i] = this->configuration->colorMinutes.toCRGB();
        //this->leds[i] = CRGB::Blue;
    }

    // Seconds
    if (configuration->displaySeconds) this->leds[mapToLEDs(time.seconds, 60)] = this->configuration->colorMinutes.toCRGB().subtractFromRGB(0xBB);
    
    // Hours
    this->leds[mapToLEDs(time.hours, 24)] = this->configuration->colorHours.toCRGB();

};

void TIDILE::displayEnv(ClockEnv * env) {
    // TODO
}

void TIDILE::displayCustom(int progress, CRGB color, int duration) {
    clear();
    for (int i = 0; i < mapToLEDs(progress, 99); i++) {
        this->leds[i] = color;
    }
}