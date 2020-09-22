#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <FastLED.h>
#include "Circle_Clock.hpp"
#include <time.h>
#include "ClockTime.h"
#include "ClockConfig.hpp"
#include "Handler.hpp"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Time config
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

CRGB leds[NUM_LEDS];
ClockConfig config;
CircleClock *circle;

int lastSec = 0;

// Webserver
AsyncWebServer server(80);
Handler *handler = new Handler(&config, circle);

void startupLEDs(CRGB * leds, int delayEach) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White; FastLED.show(); delay(delayEach);
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black; FastLED.show(); delay(delayEach);
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

#pragma region Connecting to WiFi
  WiFi.begin();
  delay(2000);

  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.mode(WIFI_AP_STA);
    delay(500);
    WiFi.beginSmartConfig();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      Serial.println(WiFi.smartConfigDone());
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#pragma endregion
  
  //register leds
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(config.brightness);

  // Time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  startupLEDs(leds, 16);
  delay(500);

  config.colorHours = Color((byte) 0xFF, (byte) 0x00, (byte) 0x00);
  config.colorMinutes = Color((byte) 0x00, (byte) 0xFF, (byte) 0x00);

  CircleClock* c = new CircleClock(leds, NUM_LEDS, &config);
  circle = c;

  // HTTP
  server.on("/colors", HTTP_PUT, [](AsyncWebServerRequest *request){handler->onColors(request);});
  server.on("/display", HTTP_POST, [](AsyncWebServerRequest *request){handler->onCustom(request);});
  //server.on("/envcolors", HTTP_PUT, [](AsyncWebServerRequest *request){handler->onEnvColors(request);});
  server.on("/blink", HTTP_PUT, [](AsyncWebServerRequest *request){handler->onBlink(request);});
  //server.on("/env", HTTP_POST, [](AsyncWebServerRequest *request){handler->onEnv(request);});
  //server.on("/times", HTTP_PUT, [](AsyncWebServerRequest *request){handler->onTimes(request);});

  server.onNotFound([](AsyncWebServerRequest *request){request->send(404);});
  server.begin();
  Serial.println("HTTP server started");
}

ClockTime getClockTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return ClockTime { 0, 0, 0};
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  return ClockTime {
    seconds: timeinfo.tm_sec,
    minutes: timeinfo.tm_min,
    hours: timeinfo.tm_hour
  };
}

void loop() {
  ClockTime time = getClockTime();
  circle->displayTime(time);
  FastLED.show();
  if (config.blinkingEnabled && time.seconds != lastSec) {
    FastLED.setBrightness(0.25 * config.brightness);
    FastLED.show();
    delay(100);
    FastLED.setBrightness(config.brightness);
  }
  lastSec = time.seconds;
}
