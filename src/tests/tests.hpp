#pragma once
#include "../config/config_includes.hpp"
#ifdef RUN_TESTS
#include "timeTests.hpp"
#include "tidileTests.hpp"
#include <WiFiClient.h>

/**
 * @brief tests for checking number operations
 * 
 */
void numberTests()
{
    {
        TEST_INIT(numbers, digits to two character string);
        EXPECT_EQUALS(String("00"), digitToTwoCharsDigit(0));
        TEST_RESULT_PRINT();
    }

    {
        TEST_INIT(numbers, map doubles);
        EXPECT_EQUALS(10, mapd(5, 0, 10, 0, 20));
        EXPECT_EQUALS(5, mapd(10, 0, 20, 0, 10));
        /* for (int i = 0; i < 120; i++)
        {
            Serial.println((int)mapd((int)(i / 2), 0, 60, 0, 143));
        } */
        TEST_RESULT_PRINT();
    }
}

void internetTests()
{
    {
        TEST_INIT(internet, checking connection to homeserver);
        WiFiClient client;
        EXPECT_TRUE(client.connect("mt32.net", 80));
        client.stop();
        TEST_RESULT_PRINT();
    }
}

void colorTests()
{
    {
        TEST_INIT(colors, converting hex values to rgb values);
        EXPECT_EQUALS(hexToColor(String("#ffffff")).toHex(), Color(255, 255, 255).toHex());
        EXPECT_EQUALS(hexToColor(String("#000000")).toHex(), Color(0, 0, 0).toHex());
        TEST_RESULT_PRINT();
    }
}

/**
 * @brief runs all available tests
 * 
 */
void runTests()
{
    Serial.println("Started performing tests");

    timeTests();
    numberTests();
    internetTests();
    colorTests();
    tidileTests();

    ALL_TEST_RESULTS_PRINT();
}
#endif