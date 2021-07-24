#ifndef CONFIG_DEF
#define CONFIG_DEF

#define RUN_TESTS

#define NUM_LEDS 49
#define LED_PIN 5
#define BLINK_BRIGHTNESS 0.7

#define LED_COUNT_FOR_ONE_SECOND (int)((double)NUM_LEDS / 60.0)
#define DIMM_VALUE 0xFF
#define DIMM_ADD_VALUE 0xAA

//#define FASTSTARTUP //uncomment when the startup animation shouldn´t be displayed each startup

// Touch Pin
#define THRESHOLD 25
#define SMOOTH_LOOPS 50
#define TOUCH_PIN 4

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
//#define LIGHT_SENSOR
//#define TEMPERATURE_SENSOR
//#define HUMIDITY_SENSOR
//#define PRESSURE_SENSOR
// #pragma endregion

// #pragma region used sensors
//#define BME280
// #pragma endregion

#endif
