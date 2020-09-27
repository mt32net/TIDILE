#include "TIDILE.hpp"

TIDILE::TIDILE()
{
}

void TIDILE::setup(CRGB leds[NUM_LEDS], int numberLEDs, AsyncWebServer* server)
{
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->server = server;

    configTime(3600, 3600, ntpServer);

    configuration.deserialize(&preferences);
    handler.setup(&configuration, this, &preferences);
    webserver.setup(&handler, server);

    FastLED.setBrightness(configuration.brightness);

    Helper.getTime();

#ifndef FASTSTARTUP
    startupLEDs(15);
#endif
}

int TIDILE::mapToLEDs(int value, int max)
{
    return map(value, 0, max, 0, this->numberLEDs);
}

void TIDILE::clear()
{
    for(int i = 0; i < numberLEDs; i++) {
        leds[i] = CRGB::Black;
    }
}

void TIDILE::startupLEDs(int delayTime)
{
    for(int i = 0; i < numberLEDs; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
        delay(delayTime);
    }
    for(int i = 0; i < numberLEDs; i++) {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(delayTime);
    }
}

ClockConfig * TIDILE::getConfig() {
    return &configuration;
}

void TIDILE::displayTime()
{
    ClockTime time = Helper.getTime();

    clear();

    if (isNightTime(Helper.getTime())) {
        FastLED.show();
        return;
    }

    // Minutes
    for (int i = 0; i < mapToLEDs(time.minutes, 60); i++)
    {
        this->leds[i] = configuration.colorMinutes.toCRGB();
        //this->leds[i] = CRGB::Blue;
    }

    // Seconds
    if (configuration.displaySeconds)
        this->leds[mapToLEDs(time.seconds, 60)] = (configuration.dimmSeconds) ? configuration.colorMinutes.toCRGB().subtractFromRGB(0xBB) : configuration.colorSeconds.toCRGB();

    // Hours
    this->leds[mapToLEDs(time.hours, 24)] = configuration.colorHours.toCRGB();

    if (configuration.blinkingEnabled && time.seconds != lastSec)
    {
        FastLED.setBrightness(BLINK_BRIGHTNESS * configuration.brightness);
        FastLED.show();
        delay(100);
    }

    FastLED.show();
}

void TIDILE::displayEnv(ClockEnv env)
{
    clear();
    if (!isNightTime(Helper.getTime()))
    {
        for (int i = 0; i < mapToLEDs(env.temperature, 50); i++)
        {
            this->leds[i] = configuration.colorTemperature.toCRGB();
        }
        FastLED.show();
        delay(ENV_DISPLAY_TIME);
        clear();
        for (int i = 0; i < mapToLEDs(env.pressure, 10000); i++)
        {
            this->leds[i] = configuration.colorPressure.toCRGB();
        }
        FastLED.show();
        delay(ENV_DISPLAY_TIME);
    }
}

void TIDILE::displayCustom(int progress, CRGB color, int duration)
{
    clear();
    if (!isNightTime(Helper.getTime()))
    {
        for (int i = 0; i < mapToLEDs(progress, 99); i++)
        {
            this->leds[i] = color;
        }
    }
}

bool TIDILE::isNightTime(ClockTime time)
{
    return ((String(time.hours) + String(time.minutes)).toInt() > configuration.nightTimeBegin || (String(time.hours) + String(time.minutes)).toInt() < configuration.nightTimeEnd) && configuration.nightTimeLight;
}

void TIDILE::displayCustom(CRGB *leds, int delayEach)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::White;
    FastLED.show();
    delay(delayEach);
  }
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(delayEach);
  }
}