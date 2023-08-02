#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>
#include "TIDILE.hpp"
#include "helper/WiFiHelper.hpp"
#include "helper/time.hpp"
#include "helper/color.hpp"
#include "helper/numbers.hpp"

TIDILE::TIDILE() {
    TIDILE::instance = this;
}

TIDILE * TIDILE::instance = nullptr;


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

long lastTime = 0;
bool lastState = false;

void TIDILE::setup(CRGB *leds, int numberLEDs, AsyncWebServer *server, WiFiHelper *wifiHelper)
{
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->server = server;
    this->wifiHelper = wifiHelper;
    this->ledController = LEDController(numberLEDs, leds, &configuration, numberZones);
    
    this->nightButton = new Debouncer((gpio_num_t) NIGHT_BUTTON_PIN, [](){
        TIDILE::instance->handleNightButtonPress();
    });


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

    loadClockConfig();

    configTime(3600, 3600, ntpServer);
    ClockTime time;
    getTime(&time);

    // CAUTION makes GETs increadibly slow
    // server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    // server->serveStatic("/", SPIFFS, "/static");
    webserver.setup(server, &configuration, wifiHelper, &custom);
    // Only start mqtt service when connected to a internet
    if (!wifiHelper->isAPMode())
    {
#ifdef TIDILE_MQTT
        mqtt.setup(&configuration, this, MQTT_URI, MQTT_PORT, time);
#endif
    }

    FastLED.setBrightness(configuration.brightness);

#ifndef FASTSTARTUP
    startupLEDs(STARTUP_ANIMATION_DELAY);
#endif
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
        return;
    }
    // Minutes
    for (int i = 0; i < time.minutes; i++)
        ledController.setZone(i, configuration.colorMinutes);
    // Seconds
    if (configuration.displaySeconds)
    {
        // When seconds are "out of minutes"
        if (time.seconds > time.minutes)
            ledController.setZone(time.seconds - 1, (configuration.dimmSeconds) ? (configuration.colorMinutes) : configuration.colorSeconds);
        // When seconds are inside minutes
        else
            ledController.setZone(time.seconds - 1, (configuration.dimmSeconds) ? Color(0, 0, 0) : configuration.colorSeconds);
    }
    // Hours
    int hours = time.hours;
    if (configuration.format == ClockFormat::Format_12H) hours = time.hours % 12;
    ledController.setZone(map(hours, 0, (long)configuration.format, 0, numberZones) - 1, configuration.colorHours);
}

void TIDILE::displayEnv(ClockEnv env)
{
    // clear();
    // ClockTime time;
    // getTime(&time);
    // if (isNightTime(configuration, time))
    // {
    //     FastLED.show();
    //     return;
    // }
    // for (int i = 0; i < mapToLEDs(env.temperature, 50); i++)
    // {
    //     this->leds[i] = configuration.colorTemperature.toCRGB();
    // }
    // FastLED.show();
    // delay(ENV_DISPLAY_TIME);
    // clear();
    // for (int i = 0; i < mapToLEDs(env.pressure, 10000); i++)
    // {
    //     this->leds[i] = configuration.colorPressure.toCRGB();
    // }
    // FastLED.show();
    // delay(ENV_DISPLAY_TIME);
}

void TIDILE::handleNightButtonPress()
{
    this->configuration.tempOverwriteNightTime = !this->configuration.tempOverwriteNightTime;
}

void TIDILE::update()
{
    // AP MODE

    if (wifiHelper->isAPMode()) {
        for (int i = 0; i < NUMER_STATUS_LEDS; i++)
        {
            this->leds[i] = ((millis() / 1000) % 2 == 0) ? CRGB::White : CRGB::Black;
        }
        FastLED.show();
        return;
    }

    // GET AND DISPLAY TIME

    ClockTime currentTime;
    getTime(&currentTime);

    // Reset mode if time is over
    if (currentTime.unixTime > custom.unixEnd)
        custom.mode = NORMAL;

    switch (custom.mode) {
        case NORMAL: displayTime(currentTime); break;
        case TIME: displayTime(custom.customTime); break;
        case COLOR: ledController.setAll(custom.color); break;
    }

    // LIGHT SENSOR AND ENV STUFF

    // Will remain 1, if light sensor not defined
    double factor = 1;

    // TODO overflows?
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
        factor = influence * lightPercent + (1 - influence);
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

    // LOOPING STUFF
    FastLED.setBrightness(getConfig()->brightness * factor);
#ifdef TIDILE_MQTT
    mqtt.loop(currentTime);
#endif
    FastLED.show();
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