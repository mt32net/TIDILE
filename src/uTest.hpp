#pragma once

int testsPass = 0;
int testsFail = 0;
int currTestNr = 0;
bool globalResult = true;

bool curTestAdded = false;
int curTestCaseAdded = 0; //0: pass, 1: fail

#if defined(ESP32) || defined(ARDUINO) || defined(ESP_PLATFORM)
#define OUTPUT_METHOD(out) Serial.print(String(out));
#elif defined(_GLIBCXX_IOSTREAM)
#include <iostream>
#define OUTPUT_METHOD(out) std::cout << out;
#endif

#define TEST_INIT(component, case)                                                          \
    const char *testComponent = #component;                                                 \
    const char *testCase = #case;                                                           \
    bool testResult = true;                                                                 \
    int testNr = currTestNr;                                                                \
    currTestNr++;                                                                           \
    curTestAdded = false;                                                                   \
    OUTPUT_METHOD(String("(") + String(testNr) + String(")\tStarting to test component ") + \
                  String(#component) + " in test case " +                                   \
                  String(#case) + "\n")

#define EXPECT_EQUALS(value1, value2)                           \
    {                                                           \
        bool newValueAdd = (value1 == value2);                  \
        testResult = testResult && newValueAdd;                 \
        /*Testcase false*/                                      \
        if (!newValueAdd)                                       \
        {                                                       \
            /*  Testcase not yet accounted for*/                \
            if (!curTestAdded)                                  \
            {                                                   \
                testsFail++;                                    \
                curTestCaseAdded = 1;                           \
                curTestAdded = true;                            \
            }                                                   \
            /* Testcase switched from pass to fail */           \
            else if (curTestCaseAdded == 0)                     \
            {                                                   \
                testsPass--;                                    \
                testsFail++;                                    \
                curTestCaseAdded = 1;                           \
            }                                                   \
            OUTPUT_METHOD(String("\tTest ") + testComponent +   \
                          " " + testCase +                      \
                          " failed. Expected value " +          \
                          String(value1) + ", got " +           \
                          String(value2) + "\n\t\t function " + \
                          __func__ + " at line " +              \
                          __LINE__ + " in File " +              \
                          __FILE__ + "\n");                     \
        }                                                       \
        else                                                    \
        /* Testcase passed */                                   \
        {                                                       \
            /*  Testcase not yet accounted for*/                \
            if (!curTestAdded)                                  \
            {                                                   \
                testsPass++;                                    \
                curTestCaseAdded = 0;                           \
                curTestAdded = true;                            \
            }                                                   \
        }                                                       \
    }                                                           \
    globalResult = globalResult && testResult

#define EXPECT_ZERO(value) EXPECT_EQUALS(0, value)
#define EXPECT_TRUE(value) EXPECT_EQUALS(true, value)
#define EXPECT_FALSE(value) EXPECT_EQUALS(false, value)

#define ADD_CUSTOM_TEST_RESULT(bool_value) EXPECT_EQUALS(true, bool_value)

#define PRINT_ADDITIONAL_TEST_INFO(string) OUTPUT_METHOD(String("\t[INFO] ") + String(string))

#define TEST_RESULT_BOOL() testResult
#define TEST_RESULT_STRING() ((TEST_RESULT_BOOL()) ? String("[PASSED] ") : String("[FAILED] ")) + "in \"" + testComponent + "\" in component \"" + testCase + "\""

#define TEST_RESULT_PRINT() OUTPUT_METHOD(TEST_RESULT_STRING() + "\n")

#define ALL_TEST_RESULTS_BOOL() globalResult
#define ALL_TEST_RESULTS_STRING()                          \
    String("==================================\n") +       \
        "=>\t" + String(testsPass) + " Test(s) passed\n" + \
        "\t" + String(testsFail) + " Test(s) failed"

#define ALL_TEST_RESULTS_PRINT() OUTPUT_METHOD(ALL_TEST_RESULTS_STRING() + "\n")
