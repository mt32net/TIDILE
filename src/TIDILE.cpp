#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include "TIDILE.hpp"
#include "helper/WiFiHelper.hpp"
#include "helper/time.hpp"
#include "helper/color.hpp"
#include "helper/numbers.hpp"

TIDILE::TIDILE() {}

void TIDILE::loadClockConfig()
{
    Serial.println("Loading config...");
    String configString = SPIFFS.open(CONFIG_FILE_NAME).readString();
    // I hope this works
    DynamicJsonDocument jsonConfig(2000);
    DeserializationError err = deserializeJson(jsonConfig, configString);
    if (err != DeserializationError::Ok)
    {
        Serial.print("Config deserialization error: ");
        Serial.println(err.code());
    }
    JsonObject obj = jsonConfig.as<JsonObject>();
    configuration.deserialize(&obj);
    jsonConfig.garbageCollect();
}

void TIDILE::setup(CRGB *leds, int numberLEDs, AsyncWebServer *server, WiFiHelper *wifiHelper)
{
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->server = server;
    this->wifiHelper = wifiHelper;

#ifdef PRINT_DEFAULT_CONFIG
    Serial.println("-------------- Default Config -------------");
    DynamicJsonDocument doc(2000);
    JsonObject obj = doc.to<JsonObject>();
    ClockConfig().serialize(&obj);
    serializeJsonPretty(obj, Serial);
    doc.garbageCollect();
    Serial.println();
    Serial.println("-------------------------------------------");
#endif

    customDisplayTil = {0, 0, 0, 0, 0, 0};
    loadClockConfig();

    configTime(3600, 3600, ntpServer);
    ClockTime time;
    getTime(&time);

    // server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    server->serveStatic("/", SPIFFS, "/static");
    requestHandler.setup(&configuration, this);
    webserver.setup(&requestHandler, server);
    // Only start mqtt service when connected to a internet
    if (!wifiHelper->isAPMode())
    {
        mqtt.setup(&configuration, &preferences, this, MQTT_URI, MQTT_PORT, time);
    }

    FastLED.setBrightness(configuration.brightness);

#ifndef FASTSTARTUP
    startupLEDs(STARTUP_ANIMATION_DELAY);
#endif
}

int TIDILE::mapToLEDs(int value, int max)
{
    return (int)mapd(value, 0, max, 0, this->numberLEDs - 1);
}

void TIDILE::clear()
{
    for (int i = 0; i < numberLEDs; i++)
        leds[i] = CRGB::Black;
}

void TIDILE::startupLEDs(int delayTime)
{
    Serial.print("Running startup animation");
    for (int i = 0; i < numberLEDs; i++)
    {
        leds[i] = CRGB::White;
        FastLED.show();
        delay(delayTime);
        if (i % 5 == 0)
            Serial.print(".");
    }
    for (int i = 0; i < numberLEDs; i++)
    {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(delayTime);
        if (i % 5 == 0)
            Serial.print(".");
    }
    Serial.println("  Finished");
}

ClockConfig *TIDILE::getConfig()
{
    return &configuration;
}

void TIDILE::displayTime(ClockTime time)
{
    clear();
    resetOverwriteNightTimeIfLegit(configuration, time);
    if (isNightTime(configuration, time))
    {
        FastLED.show();
        return;
    }
    // Minutes
    for (int i = 0; i < mapToLEDs(time.minutes, 59); i++)
        this->leds[i] = configuration.colorMinutes.toCRGB();
    // Seconds
    if (configuration.displaySeconds)
    {

        // When seconds are "out of minutes" (?)
        if (mapToLEDs(time.seconds, 59) > mapToLEDs(time.minutes, 59))
        {
            for (int i = 0; i < LED_COUNT_FOR_ONE_SECOND(configuration.ledCount); i++)
            {
                this->leds[mapToLEDs(time.seconds, 59) + i] = (configuration.dimmSeconds) ? (configuration.colorMinutes).toCRGB() : configuration.colorSeconds.toCRGB();
            }
        }
        // When seconds are inside minutes (?)
        else
        {
            for (int i = 0; i < LED_COUNT_FOR_ONE_SECOND(configuration.ledCount); i++)
            {
                this->leds[mapToLEDs(time.seconds, 59) + i] = (configuration.dimmSeconds) ? CRGB::Black : configuration.colorSeconds.toCRGB();
            }
        }
    }
    // Hours
    int hours = (configuration.format == ClockFormat::Format_24H || time.hours < 12) ? time.hours : time.hours - 12;
    for (int i = 0; i < LED_COUNT_FOR_ONE_SECOND(configuration.ledCount); i++)
        this->leds[mapToLEDs(hours, configuration.format) + i] = configuration.colorHours.toCRGB();

    // Update leds
    FastLED.show();
}

void TIDILE::displayEnv(ClockEnv env)
{
    clear();
    ClockTime time;
    getTime(&time);
    if (isNightTime(configuration, time))
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

void TIDILE::displaCustom(Color colorCode, ClockTime until)
{
    this->customDisplayTil = until;
    this->lmapColor = colorCode;
    FastLED.show();
}

void TIDILE::update()
{
    ClockTime currentTime;
    // TODO check
    if (wifiHelper->isAPMode())
        return;
    mqtt.loop(currentTime);

    int timeTil = hmsToTimeInt(customDisplayTil);
    int curr = hmsToTimeInt(currentTime);
    if (curr >= timeTil && this->clockMode)
    {
        displayTime(currentTime);
    }
    else
    {
        clear();
        for (int i = 0; i < this->numberLEDs; i++)
        {
            this->leds[i] = this->lmapColor.toCRGB();
        }
        FastLED.show();
    }

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

void TIDILE::mqttCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:");
    Serial.write(payload, length);
    Serial.println();
}

void TIDILE::flushConfig()
{
    // TODO calc size
    DynamicJsonDocument doc(2000);
    JsonObject json = doc.to<JsonObject>();
    configuration.serialize(&json);
    File configFile = SPIFFS.open(CONFIG_FILE_NAME, FILE_WRITE);
    WriteBufferingStream bufferedFile(configFile, 64);
    // TODO change back to file
    serializeJson(json, bufferedFile);
    bufferedFile.flush();
    configFile.close();
}