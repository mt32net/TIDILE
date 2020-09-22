#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include <FastLED.h>
#include "Circle_Clock.hpp"

#include "definements.h"
#include "test.cpp"

CRGB leds[NUM_LEDS];

void testLEDs(CRGB * leds) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White; FastLED.show(); delay(30);
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_AP_STA);
  delay(500);

  WiFi.beginSmartConfig();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    Serial.println(WiFi.smartConfigDone());
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS);
  testLEDs(leds);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
