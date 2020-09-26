#include "TIDILE.hpp"

TIDILE::TIDILE() {
    
}

void TIDILE::setup(CRGB leds[NUM_LEDS], int numberLEDs, ClockConfig *configuration){
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->configuration = configuration;

    configTime(3600, 3600, ntpServer);
}

int TIDILE::mapToLEDs(int value, int max) {
    return map(value, 0, max, 0, this->numberLEDs);
}

void TIDILE::clear() {
    for (int i = 0; i < this->numberLEDs; i++) {
        this->leds[i] = CRGB::Black;
    }
}

ClockTime TIDILE::displayTime() {
    ClockTime time = Helper.getTime();

    clear();

    // Minutes
    for (int i = 0; i < mapToLEDs(time.minutes, 60); i++) {
        this->leds[i] = this->configuration->colorMinutes.toCRGB();
        //this->leds[i] = CRGB::Blue;
    }

    // Seconds
    if (configuration->displaySeconds) this->leds[mapToLEDs(time.seconds, 60)] = this->configuration->colorSeconds.toCRGB();
    
    // Hours
    this->leds[mapToLEDs(time.hours, 24)] = this->configuration->colorHours.toCRGB();
    return time;
}

void TIDILE::displayEnv(ClockEnv env) {
    clear();
    for (int i = 0; i < mapToLEDs(env.temperature, 50); i++) {
        this->leds[i] = configuration->colorTemperature.toCRGB();
    }
    FastLED.show();
    delay(ENV_DISPLAY_TIME);
    clear();
    for (int i = 0; i < mapToLEDs(env.pressure, 10000); i++) {
        this->leds[i] = configuration->colorPressure.toCRGB();
    }
    FastLED.show();
    delay(ENV_DISPLAY_TIME);
}

void TIDILE::displayCustom(int progress, CRGB color, int duration) {
    clear();
    for (int i = 0; i < mapToLEDs(progress, 99); i++) {
        this->leds[i] = color;
    }
}

bool TIDILE::inTimeWindow(ClockTime time){
    return (String(time.hours) + String(time.minutes)).toInt() > configuration->nightTimeBegin || (String(time.hours) + String(time.minutes)).toInt() < configuration->nightTimeEnd;
}