#pragma once
#include "../config.hpp"
#ifdef RUN_TESTS
#include "../TIDILE.hpp"

/**
 * @brief friend method of TIDILE to test it
 * 
 */
void tidileTests()
{
    Serial.println("Performing tests on TIDILE class");
}

#endif