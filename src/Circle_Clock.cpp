#include "Circle_Clock.hpp"

CircleClock::CircleClock() {
    
}

void CircleClock::setup(CRGB leds[NUM_LEDS], int numberLEDs, ClockConfig *configuration){
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->configuration = configuration;
    
}

int CircleClock::mapToLEDs(int value, int max) {
    return map(value, 0, max, 0, this->numberLEDs);
}

void CircleClock::clear() {
    for (int i = 0; i < this->numberLEDs; i++) {
        this->leds[i] = CRGB::Black;
    }
}

void CircleClock::displayTime(ClockTime time) {

    clear();

    // Minutes
    for (int i = 0; i < mapToLEDs(time.minutes, 60); i++) {
        this->leds[i] = this->configuration->colorMinutes.toCRGB();
        //this->leds[i] = CRGB::Blue;
    }

    // Hours
    this->leds[mapToLEDs(time.hours, 24)] = this->configuration->colorHours.toCRGB();
};

void CircleClock::displayEnv(ClockEnv * env) {
    // TODO
}

void CircleClock::displayCustom(int progress, CRGB color, int duration) {
    clear();
    for (int i = 0; i < mapToLEDs(progress, 99); i++) {
        this->leds[i] = color;
    }
}