#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#define LOG_WIFI_LEVEL ESP_LOG_ERROR
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
// #define FASTLED_RMT_BUILTIN_DRIVER 1
// #define FASTLED_ESP32_I2S 1

#include "esp_system.h"
// #include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "Arduino.h"
#include <FastLED.h>
#include "TIDILE.hpp"
#include "helper/WiFiHelper.hpp"
#include "helper/numbers.hpp"
#include "config/config_includes.hpp"
#include "SPIFFS.h"
#ifdef RUN_TESTS
#include "tests/tests.hpp"
#endif

// TODO update webserver

#if defined(HUMIDITY_SENSOR) && defined(BME280)
Adafruit_BME280 bmp; // I2C
#endif

CRGB *leds;

TIDILE tidile;
AsyncWebServer server(HTTP_ENDPOINT_PORT);
WiFiHelper wifiHelper;

#ifdef RUN_TESTS
void tests();
#endif

void setup()
{

  Serial.begin(115200);

  // initArduino();

  wifiHelper.connectWiFi();

  esp_log_level_set("wifi", ESP_LOG_ERROR);
  esp_log_level_set("task_wdt", ESP_LOG_ERROR);

  if (!SPIFFS.begin())
  {
    Serial.println("Error starting SPIFFS.");
  }


#ifdef RUN_TESTS
  runTests();
#endif

#if defined(TEMPERATURE_SENSOR) || defined(HUMIDITY_SENSOR) || defined(PRESSURE_SENSOR)
  Wire.begin();
#ifdef HUMIDITY_SENSOR
  // BMP280
  bool ok = bmp.begin(0x76);
  if (!ok)
  {
    Serial.println("Could not find a valid BMP280!");
  }
#endif
#endif

  leds = new CRGB[LED_COUNT];

#ifdef ESP32
  FastPin<LED_PIN>::lo();
#endif
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, LED_COUNT);
  FastLED.setDither(DISABLE_DITHER);

  tidile.setup(leds, LED_COUNT, &server, &wifiHelper);
#if defined(HUMIDITY_SENSOR) && defined(BME280)
  tidile.addBMP(&bmp);
#endif
}

void loop()
{
  tidile.update();
  // heap_caps_print_heap_info(0);
}