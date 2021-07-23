#pragma once
#include "Arduino.h"
#include "definements.hpp"
#include "ClockInfo.hpp"
#include "ClockConfig.hpp"

/**
 * @brief Convert simple time format to full time string
 * 0600 -> 06:00
 * @param timeInt the simple formatted time
 * @return common time format as String
*/
String timeIntToTimeString(int timeInt);

/**
 * @brief Complement to timeIntToTimeString, converts time string to time number 
 * 06:00 -> 600
 * 
 * @param timeString the time string
 * @return simple formatted time 
 */
int timeStringToTimeInt(String timeString);
/**
 * @brief Convert time to long simple formatted int
 * 06:45:30 -> 64530
 * 
 * @param time the time to convert
 * @return long simple formattted time
 */
int hmsToTimeInt(ClockTime time);
/**
 * @brief Get the Time
 * 
 * @return ClockTime 
 */
ClockTime getTime();
/**
 * @brief Get the Date Time as String
 * 
 * @return data time from getTime() as String 
 */
String getDateTimeToString();
/**
 * @brief Checks wether a time is in the night time interval
 * 
 * @param config clock configuration for nighttime config
 * @param time time to "night time check"
 * @return true if time is in night time interval
 * @return false if time is outside of night time interval
 */
bool isNightTime(ClockConfig config, ClockTime time);
/**
 * @brief passive method to run regulary, checks wether the temporary overwrite of night time can be reset
 * 
 * @param config clock config for nighttime and temporary overwrite
 * @param time current time
 */
void resetOverwriteNightTimeIfLegit(ClockConfig config, ClockTime time);