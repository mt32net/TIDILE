#include "TIDILE.hpp"

TIDILE::TIDILE() {}

void TIDILE::setup(CRGB leds[NUM_LEDS], int numberLEDs, AsyncWebServer *server)
{
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->server = server;

    configTime(3600, 3600, ntpServer);
    Helper.getTime();

    configuration.deserialize(&preferences);
    handler.setup(&configuration, this, &preferences);
    webserver.setup(&handler, server);

    FastLED.setBrightness(configuration.brightness);

#ifndef FASTSTARTUP
    startupLEDs(STARTUP_ANIMATION_DELAY);
#endif
}

int TIDILE::mapToLEDs(int value, int max)
{
    return map(value, 0, max, 0, this->numberLEDs - 1);
}

void TIDILE::clear()
{
    for (int i = 0; i < numberLEDs; i++)
        leds[i] = CRGB::Black;
}

void TIDILE::startupLEDs(int delayTime)
{
    for (int i = 0; i < numberLEDs; i++)
    {
        leds[i] = CRGB::White;
        FastLED.show();
        delay(delayTime);
    }
    for (int i = 0; i < numberLEDs; i++)
    {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(delayTime);
    }
}

ClockConfig *TIDILE::getConfig()
{
    return &configuration;
}

void TIDILE::displayTime()
{
    ClockTime time = Helper.getTime();
    clear();

    if (Helper.isNightTime(configuration, time))
    {
        FastLED.show();
        return;
    }

    // Minutes
    for (int i = 0; i < mapToLEDs(time.minutes, 59); i++)
        this->leds[i] = configuration.colorMinutes.toCRGB();
    // Seconds
    if (configuration.displaySeconds) {

        if (mapToLEDs(time.seconds, 59) > mapToLEDs(time.minutes, 59)) {
            this->leds[mapToLEDs(time.seconds, 59)] = (configuration.dimmSeconds) ? configuration.colorMinutes.toCRGB().subtractFromRGB(DIMM_ADD_VALUE) : configuration.colorSeconds.toCRGB();
            if (DOUBLE_FOR_EACH) 
                this->leds[mapToLEDs(time.seconds, 59) + 1] = (configuration.dimmSeconds) ? configuration.colorMinutes.toCRGB().subtractFromRGB(DIMM_ADD_VALUE) : configuration.colorSeconds.toCRGB();
        }
        else {
            this->leds[mapToLEDs(time.seconds, 59)] = (configuration.dimmSeconds) ? configuration.colorMinutes.toCRGB().subtractFromRGB(DIMM_VALUE) : configuration.colorSeconds.toCRGB();
            if (DOUBLE_FOR_EACH) 
                this->leds[mapToLEDs(time.seconds, 59) + 1] = (configuration.dimmSeconds) ? configuration.colorMinutes.toCRGB().subtractFromRGB(DIMM_VALUE) : configuration.colorSeconds.toCRGB();
        }
    }
    // Hours
    this->leds[mapToLEDs(time.hours, 23)] = configuration.colorHours.toCRGB();
    if (DOUBLE_FOR_EACH) this->leds[mapToLEDs(time.hours, 23)] = configuration.colorHours.toCRGB();
    FastLED.show();
}

void TIDILE::displayEnv(ClockEnv env)
{
    clear();
    if (Helper.isNightTime(configuration, Helper.getTime()))
    {
        FastLED.show();
        return;
    }
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

void TIDILE::displayCustom(int progress, CRGB color, int duration)
{
    clear();
    if (Helper.isNightTime(configuration, Helper.getTime()))
    {
        FastLED.show();
        return;
    }
    for (int i = 0; i < mapToLEDs(progress, 99); i++)
    {
        this->leds[i] = color;
    }
}

void TIDILE::displayCustom(CRGB *leds, int delayEach)
{
    if (Helper.isNightTime(configuration, Helper.getTime()))
    {
        clear();
        FastLED.show();
        return;
    }
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

void TIDILE::loop()
{
    displayTime();

    if (loopI >= SMOOTH_LOOPS)
    {
#if defined(DISPLAY_HUMIDIY) || defined(DISPLAY_TEMPERATURE) || defined(DISPLAY_PRESSURE)
        if (touchAverage / SMOOTH_LOOPS < THRESHOLD)
        {
            displayEnv(getEnv());
        }
#endif
        lightAvg = lightAvg / SMOOTH_LOOPS;
        double lightPercent = (double)map(lightAvg, 0, 4095, 0, 100) / (double)100;
        double influence = (double)getConfig()->lightInfluence / (double)100;

#ifdef LIGHT_SENSOR
        double factor = influence * lightPercent + (1 - influence);
        FastLED.setBrightness(getConfig()->brightness * factor);
#else
        FastLED.setBrightness(getConfig()->brightness);
#endif
        loopI = 0;
        touchAverage = 0;
        lightAvg = 0;
    }
    else
    {
        touchAverage += touchRead(TOUCH_PIN);
        lightAvg += analogRead(PHOTORESISTOR_PIN);
    }
    loopI++;
}

#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
void TIDILE::addBMP(Adafruit_BME280 *bmp)
{
    this->bmp = bmp;
}

ClockEnv TIDILE::getEnv()
{
    return ClockEnv{
#ifdef BME280
        temperature : bmp->readTemperature(),
        pressure : bmp->readPressure() / 100
#endif
    };
}
#endif