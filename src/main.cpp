#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include "TIDILE.hpp"
#include "helper.hpp"
#include "Handler.hpp"
#include "definements.hpp"

#if defined(HUMIDITY_SENSOR) && defined(BME280)
Adafruit_BME280 bmp; // I2C
#endif

CRGB leds[NUM_LEDS];

TIDILE tidile;
AsyncWebServer server(HTTP_ENDPOINT_PORT);

void setup()
{
  Serial.begin(115200);

  Helper.setupWiFi();

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
  tidile.addBMP(&bmp);
}


void loop()
{
  tidile.loop();
}
