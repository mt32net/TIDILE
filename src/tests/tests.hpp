#pragma once
#include "../config.hpp"
#ifdef RUN_TESTS
#include "timeTests.hpp"
#include "tidileTests.hpp"

void numberTests()
{
    TEST_INIT(numbers, digits to two character string);
    EXPECT_EQUALS(String("00"), digitToTwoCharsDigit(0));
    TEST_RESULT_PRINT();
}

void runTests()
{
    Serial.println("Started performing tests");

    timeTests();
    numberTests();
    tidileTests();

    ALL_TEST_RESULTS_PRINT();
}
#endif