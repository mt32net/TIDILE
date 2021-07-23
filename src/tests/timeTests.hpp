#pragma once
#include "../config.hpp"
#ifdef RUN_TESTS
#include <Arduino.h>
#include "../uTest.hpp"
#include "../helper/time.hpp"
#include "../ClockConfig.hpp"
#include "../ClockInfo.hpp"

void timeTests()
{
    //timeIntToTimeString
    {
        TEST_INIT(time, convert time int to String);
        EXPECT_EQUALS(String("06:00"), timeIntToTimeString(600));
        EXPECT_EQUALS(String("10:44"), timeIntToTimeString(1044));
        TEST_RESULT_PRINT();
    }
    //timeStringToTimeInt
    {
        TEST_INIT(time, convert time string to simple formatted number);
        EXPECT_EQUALS(600, timeStringToTimeInt("06:00"));
        EXPECT_EQUALS(1044, timeStringToTimeInt("10:44"));
        TEST_RESULT_PRINT();
    }
    //hmsToTimeInt
    {
        TEST_INIT(time, convert time to long simple int);
        EXPECT_EQUALS(124556, hmsToTimeInt({56, 45, 12, 0, 0, 0}));
        EXPECT_EQUALS(60045, hmsToTimeInt({45, 00, 6, 0, 0, 0}));
        ClockTime time1 = {3, 10, 15, 0, 0, 0};
        ClockTime time2 = {0, 0, 0, 0, 0, 0};
        EXPECT_EQUALS(151003, hmsToTimeInt(time1));
        EXPECT_EQUALS(0, hmsToTimeInt(time2));
        TEST_RESULT_PRINT();
    }
    //isNightTime
    {
        TEST_INIT(time, night time test);
        ClockConfig config;
        config.nightTimeBegin = 2300;
        ClockTime midnight = {0, 0, 0, 0, 0, 0};
        ClockTime oneOClock = {0, 0, 1, 0, 0, 0};
        ClockTime twelve = {0, 0, 12, 0, 0, 0};
        ClockTime lunchtime = {0, 0, 18, 0, 0, 0};
        EXPECT_TRUE(isNightTime(config, midnight));
        EXPECT_TRUE(isNightTime(config, oneOClock));
        EXPECT_FALSE(isNightTime(config, lunchtime));
        EXPECT_FALSE(isNightTime(config, twelve));
        TEST_RESULT_PRINT();
    }
}
#endif