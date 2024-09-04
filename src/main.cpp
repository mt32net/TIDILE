#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#define LOG_WIFI_LEVEL ESP_LOG_ERROR
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
// #define FASTLED_RMT_BUILTIN_DRIVER 1
// #define FASTLED_ESP32_I2S 1

#include "config/config_includes.hpp"
#include "main.hpp"
#include "tidileError.hpp"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "Arduino.h"
#include <platforms/esp/32/led_strip/led_strip.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <http/PingManager.hpp>
#include <http/Webserver.hpp>
#include <mqtt/MQTTHandler.hpp>
#include <FastLED.h>

#include "TIDILE.hpp"
#include "WiFiHelper.hpp"
#include "LittleFS.h"
#ifdef RUN_TESTS
#include "tests/tests.hpp"
#endif

CRGB* leds;
TIDILE tidile;

#ifdef RUN_TESTS
void tests();
#endif

void setup()
{
  Serial.begin(115200);

  leds = new CRGB[LED_COUNT];
  CFastLED::addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
  FastLED.setDither(DISABLE_DITHER);

  if (!LittleFS.begin())
  {
    FILESYSTEM_INIT_ERROR;
  }

  WiFiHelper::getInstance()->connectWiFi(WIFI_SSID, WIFI_PWD, WIFI_NUMBER_TRIES_BEFORE_AP);

  esp_log_level_set("wifi", ESP_LOG_ERROR);
  esp_log_level_set("task_wdt", ESP_LOG_ERROR);

#ifdef RUN_TESTS
  runTests();
#endif

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

void tidile_error(int errorCode, const char *message) {
  for (int i = 0; i < LED_COUNT; i++)
    leds[i] = CRGB::Black;
  Serial.println(message);
  while(true) {
    leds[errorCode] = CRGB::Red;
    FastLED.show();
    delay(1000);
    leds[errorCode] = CRGB::Black;
    FastLED.show();
    delay(1000);
  }
}