#pragma once
#include <time.h>

/**
 * @brief Simple time struct
 */
struct ClockTime
{
    int seconds;
    int minutes;
    int hours;
    int day;
    int month;
    int year;
    long unixTime;

    /*bool operator <(const ClockTime& t) {
        return year > t.year || month > t.month || day > t.day || hours > t.hours || minutes > t.minutes || seconds > t.seconds;
    }

    bool operator <(const ClockTime& t) {
        return year < t.year || month < t.month || day < t.day || hours < t.hours || minutes < t.minutes || seconds < t.seconds;
    }

    bool operator ==(const ClockTime& t) {
        return year == t.year && month == t.month && day == t.day && hours == t.hours && minutes == t.minutes && seconds == t.seconds;
    }*/
};

struct ClockEnv
{
    int temperature;
    int pressure;
};