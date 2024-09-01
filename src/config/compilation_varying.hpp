#pragma once

// #define FASTSTARTUP //uncomment when the startup animation shouldn´t be displayed each startup

// #define RUN_TESTS // uncomment when simple test programms should not be performed

// Touch Pin
#define THRESHOLD 25
#define SMOOTH_LOOPS 50
#define TOUCH_PIN 4

// DEBUG AND DEV
// #define PRINT_DEFAULT_CONFIG

// #pragma region display settings
//#define DISPLAY_TEMPERATURE
//#define DISPLAY_HUMIDIY
//#define DISPLAY_PRESSURE
// in ms
#define ENV_DISPLAY_TIME 2000
#define PHOTORESISTOR_PIN 36
#define MIN_LIGHT_PERCENT 10
// #pragma endregion

// #pragma region sensors
#define LIGHT_SENSOR
//#define TEMPERATURE_SENSOR
//#define HUMIDITY_SENSOR
//#define PRESSURE_SENSOR
// #pragma endregion

// #pragma region used sensors
//#define BME280
// #pragma endregion

#if SOC_WIFI_SUPPORTED
#define FEATURE_PING
#define FEATURE_WEB_SERVER
#define FEATURE_MQTT
#endif