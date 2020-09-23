#pragma once

#define NUM_LEDS 144
#define LED_PIN 5
#define BLINK_BRIGHTNESS 0.7

#define HTTP_ENDPOINT_PORT 80

#pragma region used storage container
//the only uncommented definement should be the one you`re storing the data in
#define useEEPROM
//#define useSDCard
#pragma endregion

#pragma region display settings
//#define displayHumidity
//#define displayTemperature
//#define displayPressure
#pragma endregion

#pragma region sensors
//#define lightSensor
//#define temperatureSensor
//#define humiditySensor
//#define pressureSensor
#pragma endregion

#pragma region  Time config
#define ntpServer "pool.ntp.org"
#pragma endregion

#pragma region UIkeywords
#define COLORHOURKEYWORD "{{colorHourKeyword}}"
#define COLORMINUTEKEYWORD "{{colorMinuteKeyword}}"
#define BLINKINGENABLEDKEYWORD "{{blinkingEnabledKeyword}}"
#define BRIGHTNESSKEYWORD "{{brightnessKeyword}}"
#define COLORTEMPERATUREKEYWORD "{{colorTemperatureKeyword}}"
#define COLORPRESSUREKEYWORD "{{colorPressureKeyword}}"
#define COLORHUMIDITYKEYWORD "{{colorHumidityKeyword}}"
#pragma endregion
