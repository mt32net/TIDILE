#include <LittleFS.h>
#include <ArduinoJson.h>
#include "TIDILE.hpp"
#include "WiFiHelper.hpp"
#include "time.hpp"
#include "color.hpp"
#include <Arduino.h>
#include "config/compilation_varying.hpp"

TIDILE::TIDILE() {
    TIDILE::instance = this;
}

TIDILE * TIDILE::instance = nullptr;


void TIDILE::loadClockConfig()
{
    Serial.println("Loading config...");
    String configString = readFile(LittleFS, CONFIG_FILE_NAME);
    Serial.println(configString);
    // I hope this works
    JsonDocument jsonConfig;
    DeserializationError err = deserializeJson(jsonConfig, configString);
    if (err != DeserializationError::Ok)
    {
        Serial.print("Config deserialization error: ");
        Serial.println(err.code());
    }
    JsonObject obj = jsonConfig.as<JsonObject>();
    configuration.deserialize(&obj);
    Serial.println("Config loaded");
    configuration.toSerial();
    //jsonConfig.garbageCollect();
}

long lastTime = 0;
bool lastState = false;

void TIDILE::setup(CRGB *leds, int numberLEDs)
{
    this->leds = leds;
    this->numberLEDs = numberLEDs;
    this->ledController = LEDController(numberLEDs, leds, &configuration, numberZones);

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
    ClockTime time{};
    getTime(&time);

    FastLED.setBrightness(configuration.brightness);

#ifdef FASTSTARTUP
    // loop through all plugins and call setup
    for (TIDILE_Plugin *plugin : plugins)
    {
        plugin->setup(time);
    }
#else
    startupLEDs(STARTUP_ANIMATION_DELAY, time);
#endif
}

void TIDILE::clear()
{
    for (int i = 0; i < numberLEDs; i++)
        leds[i] = CRGB::Black;
}

void TIDILE::startupLEDs(int delayTime, ClockTime time) const {
    Serial.print("Running startup animation");
    int pluginCount = plugins.size();
    int pluginIndex = 0;
    for (int i = 0; i < numberLEDs; i++)
    {
        leds[i] = CRGB::White;
        FastLED.show();
        delay(delayTime);
        if (i % 5 == 0)
            Serial.print(".");
        if(pluginIndex < pluginCount) {
            plugins[pluginIndex]->setup(time);
            pluginIndex++;
        }
    }
    for (int i = 0; i < numberLEDs; i++)
    {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(delayTime);
        if (i % 5 == 0)
            Serial.print(".");
        if(pluginIndex < pluginCount) {
            plugins[pluginIndex]->setup(time);
            pluginIndex++;
        }
    }
    for(int i = pluginIndex; i < pluginCount; i++) {
        plugins[i]->setup(time);
    }
    Serial.println("  Finished");
}

ClockConfig *TIDILE::getConfig()
{
    return &configuration;
}

void TIDILE::displayTime(const ClockTime &time)
{
    clear();
    resetOverwriteNightTimeIfLegit(configuration, time);
    if (isNightTime(configuration, time)) return;
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

void TIDILE::addPlugin(TIDILE_Plugin *plugin) {
    this->plugins.push_back(plugin);
    plugin->initialize(this, &configuration);
}

void TIDILE::update()
{
    // AP MODE

    if (WiFiHelper::getInstance()->isAPMode()) {
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

    // run through all plugins loop
    //Serial.println("Running plugins::loop()");
    for (TIDILE_Plugin *plugin : plugins)
    {
        plugin->loop(currentTime);
    }

    //Serial.println("Running plugins::displayAnything()");
    bool displayAnything = std::all_of(plugins.begin(), plugins.end(), [](TIDILE_Plugin *plugin) { return plugin->displayAnything(); });
    //Serial.println("Running plugins::displayEnv()");
    bool anyDisplayEnv = std::any_of(plugins.begin(), plugins.end(), [](TIDILE_Plugin *plugin) { return plugin->displayEnv(); });

    if(!displayAnything) {
        //Serial.println("Display nothing");
        clear();
        FastLED.show();
        return;
    }
    if(anyDisplayEnv) {
        //Serial.println("Searching for plugin that displays env");
        for (TIDILE_Plugin *plugin : plugins)
        {
            if (plugin->displayEnv())
            {
                //Serial.println("Displaying env");
                plugin->modifyLEDs(&ledController, numberLEDs, currentTime, env);
                break;
            }
        }
    }else {
        //Serial.println("Display time");
        displayTime(currentTime);
    }

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
#if !CONFIG_IDF_TARGET_ESP32H2
        touchAverage += touchRead(TOUCH_PIN);
#endif
        lightAvg += analogRead(PHOTORESISTOR_PIN);
    }

    // LOOPING STUFF
    FastLED.setBrightness(getConfig()->brightness * factor);

    FastLED.show();
    loopI++;
}