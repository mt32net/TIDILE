#pragma once

struct ClockTime {
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
};

struct ClockEnv {
    unsigned int humidity;
    unsigned int pressure;
    unsigned int temperature;
};