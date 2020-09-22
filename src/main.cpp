#include <Arduino.h>
#include <FastLED.h>

#include "definements.h"

CRGB leds[NUM_LEDS];

void startupLEDs(CRGB * leds) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White; FastLED.show(); delay(30);
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black; FastLED.show(); delay(30);
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS);
  startupLEDs(leds);
}

void loop() {
  // put your main code here, to run repeatedly:
}