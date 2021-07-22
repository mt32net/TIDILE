#pragma once
#include <Arduino.h>

String digitToTwoCharsDigit(int digit)
{
    return (digit <= 9) ? "0" + String(digit) : String(digit);
}