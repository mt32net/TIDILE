#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#define LOG_WIFI_LEVEL ESP_LOG_ERROR
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
// #define FASTLED_RMT_BUILTIN_DRIVER 1
// #define FASTLED_ESP32_I2S 1

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "Arduino.h"
#include <FastLED.h>
#include <http/PingManager.hpp>
#include <http/Webserver.hpp>
#include <mqtt/MQTTHandler.hpp>

#include "TIDILE.hpp"
#include "WiFiHelper.hpp"
#include "config/config_includes.hpp"
#include "LittleFS.h"
#ifdef RUN_TESTS
#include "tests/tests.hpp"
#endif

CRGB *leds;

TIDILE tidile;

#ifdef RUN_TESTS
void tests();
#endif

void setup()
{

  Serial.begin(115200);

  WiFiHelper::getInstance()->connectWiFi(WIFI_NUMBER_TRIES_BEFORE_AP);

  esp_log_level_set("wifi", ESP_LOG_ERROR);
  esp_log_level_set("task_wdt", ESP_LOG_ERROR);

  if (!LittleFS.begin())
  {
    Serial.println("Error starting SPIFFS.");
  }


#ifdef RUN_TESTS
  runTests();
#endif

  leds = new CRGB[LED_COUNT];

#ifdef ESP32
  FastPin<LED_PIN>::lo();
#endif
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
  FastLED.setDither(DISABLE_DITHER);

#ifdef FEATURE_MQTT
  tidile.addPlugin((new MQTTHandler())->setupInternal(MQTT_URI, MQTT_PORT));
#endif
  const auto pm = new PingManager();
  tidile.addPlugin(pm);
  const auto custom = new CustomTopic();
  tidile.addPlugin((new Webserver())->setup(custom, pm));
  tidile.addPlugin(custom);

  tidile.setup(leds, LED_COUNT);
}

void loop()
{
  tidile.update();
  // heap_caps_print_heap_info(0);
}