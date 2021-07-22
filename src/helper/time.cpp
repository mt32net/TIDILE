#include "time.hpp"
#include <time.h>
#include "numbers.hpp"

int timeTries = 0;

String timeIntToTimeString(int timeInt)
{
    String time = (timeInt <= 999) ? "0" + String(timeInt) : String(timeInt);
    return time.substring(0, 2) + ":" + time.substring(2, 4);
}

int timeStringToTimeInt(String timeString)
{
    timeString.remove(2, 1);
    return timeString.toInt();
}

ClockTime getTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        timeTries++;
        if (timeTries > 10)
            ESP.restart();
        return ClockTime{1, 1, 1, 1, 1, 1};
    }
    //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    return ClockTime{
        seconds : timeinfo.tm_sec,
        minutes : timeinfo.tm_min,
        hours : timeinfo.tm_hour,
        day : timeinfo.tm_mday,
        month : timeinfo.tm_mon,
        year : timeinfo.tm_year
    };
}

String getDateTimeToString()
{
    ClockTime time = getTime();
    return digitToTwoCharsDigit(time.hours) + ":" + digitToTwoCharsDigit(time.minutes) + ":" + digitToTwoCharsDigit(time.seconds) + " " + digitToTwoCharsDigit(time.day) + "/" + digitToTwoCharsDigit(time.month) + "/" + digitToTwoCharsDigit(time.year + 1900);
}

bool isNightTime(ClockConfig configuration, ClockTime time)
{
    return ((digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() > configuration.nightTimeBegin ||
            (digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() < configuration.nightTimeEnd ||
            configuration.tempOverwriteNightTime) &&
           configuration.nightTimeLight;
}

void resetOverwriteNightTimeIfLegit(ClockConfig configuration, ClockTime time)
{
    if (!isNightTime(configuration, time) && (digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() >= configuration.nightTimeEnd)
        configuration.tempOverwriteNightTime = false;
}
