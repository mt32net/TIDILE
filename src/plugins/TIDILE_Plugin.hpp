//
// Created by Tom Arlt on 03.09.24.
//

#pragma once
#include <FastLED.h>
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"
#include "LEDController.hpp"

class TIDILE;

class TIDILE_Plugin {
public:
    virtual ~TIDILE_Plugin() = default;

    /**
     *
     * @return returns true if the leds should be on
     */
    virtual bool displayAnything() { return true; }
    /**
     *
     * @return returns true if the modifyLEDs function should be called of the plugin
     */
    virtual bool displayEnv() { return false; }

    /**
     * @brief modify the leds
     *
     * @param ledController the led controller
     * @param numberLEDs the number of leds
     * @param time the current time
     * @param env the current environment
     */
    virtual void modifyLEDs(LEDController *ledController, int numberLEDs, ClockTime time, ClockEnv env) {
    }

    /**
     * @brief setup function
     *
     * @param time the current time
     */
    virtual void setup(ClockTime time) {
    }

    /**
     * @brief loop function
     *
     * @param time the current time
     */
    virtual void loop(ClockTime time) {
    }

    /**
     * the tidile object
     */
    TIDILE *tidile{};
    /**
     * the config object
     */
    ClockConfig *config{};

private:
    /**
     * @brief initialize the plugin
     *
     * @param tidile the tidile object
     * @param config the config object
     */
    void initialize(TIDILE *tidile, ClockConfig *config) {
        this->tidile = tidile;
        this->config = config;
    }

    friend class TIDILE;
};
