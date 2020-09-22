#include <Arduino.h>
#include <FastLED.h>

#include "definements.h"
#include "test.cpp"

CRGB leds[NUM_LEDS];

void testLEDs(CRGB * leds) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White; FastLED.show(); delay(30);
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS);
  testLEDs(leds);
}

void loop() {
  // put your main code here, to run repeatedly:
}