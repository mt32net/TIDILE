#include "time.hpp"
#include <ctime>
#include "numbers.hpp"

int timeTries = 0;

String timeIntToTimeString(int timeInt) {
    String time = (timeInt <= 999) ? "0" + String(timeInt) : String(timeInt);
    return time.substring(0, 2) + ":" + time.substring(2, 4);
}

int timeStringToTimeInt(String timeString) {
    timeString.remove(2, 1);
    return timeString.toInt();
}

int hmsToTimeInt(ClockTime time) {
    return (digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes) + digitToTwoCharsDigit(time.seconds)).
            toInt();
}

bool getTime(ClockTime *currentTime) {
    tm timeinfo{};
    time_t now;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        // timeTries++;
        // if (timeTries > 10)
        //     ESP.restart();
        return false;
    }
    time(&now);
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    *currentTime = ClockTime{
        timeinfo.tm_sec,
        timeinfo.tm_min,
        timeinfo.tm_hour,
        timeinfo.tm_mday,
        timeinfo.tm_mon,
        timeinfo.tm_year,
        // TODO impl
        (long) now,
    };
    return true;
}

String getDateTimeToString() {
    ClockTime time{};
    getTime(&time);
    return digitToTwoCharsDigit(time.hours) + ":" + digitToTwoCharsDigit(time.minutes) + ":" +
           digitToTwoCharsDigit(time.seconds) + " " + digitToTwoCharsDigit(time.day) + "/" +
           digitToTwoCharsDigit(time.month) + "/" + digitToTwoCharsDigit(time.year + 1900);
}

bool isNightTime(ClockConfig &config, ClockTime time, bool considerNightOverwrite) {
    // temporal night light overwrite
    if (config.tempOverwriteNightTime && considerNightOverwrite)
        return true;
    if (!config.nightTimeEnabled)
        return false;
    // converting current time to simple time format
    String simpleCurTimeStr = digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes);
    int simpleCurTime = simpleCurTimeStr.toInt();

    // true if timeinterval includes midnight
    bool daySwitchIncluded = config.nightTimeBegin >= config.nightTimeEnd;

    if (daySwitchIncluded) {
        // shifting times to exclude the day switch (going over midnight)
        int diffBeginMidnight = 2400 - config.nightTimeBegin;
        int adjBegin = 0; // config.nightTimeBegin + diffBeginMidnight % 2400
        int adjEnd = (config.nightTimeEnd + diffBeginMidnight) % 2400;
        int adjCurTime = (simpleCurTime + diffBeginMidnight) % 2400;
        return adjCurTime >= adjBegin && adjCurTime <= adjEnd;
    } else {
        return simpleCurTime >= config.nightTimeBegin && simpleCurTime <= config.nightTimeEnd;
    }
    /* return ((digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() > config.nightTimeBegin ||
            (digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() < config.nightTimeEnd ||
            config.tempOverwriteNightTime) &&
           config.nightTimeLight; */
}

