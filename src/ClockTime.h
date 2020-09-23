#pragma once

struct ClockTime {
    int seconds;
    int minutes;
    int hours;
};

struct ClockEnv {
    unsigned int humidity;
    unsigned int pressure;
    unsigned int temperature;
};