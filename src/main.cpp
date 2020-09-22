#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>
#include "Circle_Clock.hpp"
#include <FastLED.h>

void setup()
{
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
}

void loop()
{
  // put your main code here, to run repeatedly:
}