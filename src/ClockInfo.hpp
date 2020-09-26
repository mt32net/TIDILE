#pragma once

struct ClockTime {
    int seconds;
    int minutes;
    int hours;
    int day;
    int month;
    int year;
};

struct ClockEnv {
    int temperature;
    int pressure;
};