#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#define LOG_WIFI_LEVEL ESP_LOG_ERROR

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "Arduino.h"
#include <FastLED.h>
#include "TIDILE.hpp"
#include "RequestHandler.hpp"
#include "definements.hpp"
#include "helper/WiFiHelper.hpp"
#include "helper/numbers.hpp"
#ifdef RUN_TESTS
#include "tests/tests.hpp"
#endif

#if defined(HUMIDITY_SENSOR) && defined(BME280)
Adafruit_BME280 bmp; // I2C
#endif

CRGB leds[NUM_LEDS];

TIDILE tidile;
AsyncWebServer server(HTTP_ENDPOINT_PORT);

#ifdef RUN_TESTS
void tests();
#endif

extern "C" void app_main()
{
  initArduino();

  esp_log_level_set("wifi", ESP_LOG_ERROR);
  esp_log_level_set("task_wdt", ESP_LOG_ERROR);

  Serial.begin(115200);

#ifdef RUN_TESTS
  runTests();
#endif

  connectWiFi();

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

  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  tidile.setup(leds, NUM_LEDS, &server);
#if defined(HUMIDITY_SENSOR) && defined(BME280)
  tidile.addBMP(&bmp);
#endif

  while (true)
  {
    tidile.update();
  }
}