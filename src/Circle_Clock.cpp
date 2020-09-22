#include "Circle_Clock.hpp"

CircleClock::CircleClock(CRGB * leds, int numberLEDs, ClockConfig * configuration) {
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->configuration = configuration;
};

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

    //Serial.println("In dispaly method..");

    // Hours
    for (int i = 0; i < mapToLEDs(time.hours, 24); i++) {
        this->leds[i] = this->configuration->colorHours.toCRGB();
        //this->leds[i] = CRGB::Blue;
    }

    //Serial.println("Printed Hours");

    // Minutes
    this->leds[mapToLEDs(time.minutes, 60)] = this->configuration->colorMinutes.toCRGB();
};

void CircleClock::displayEnv(ClockEnv * env) {
    // TODO
}

void CircleClock::displayCustom(int progress, CRGB color, int duration) {
    clear();
    for (int i = 0; i < mapToLEDs(progress, 99); i++) {
        this->leds[i] = this->configuration->colorHours.toCRGB();
    }
}