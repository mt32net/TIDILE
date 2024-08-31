#include "numbers.hpp"

String digitToTwoCharsDigit(int digit)
{
    return (digit <= 9) ? "0" + String(digit) : String(digit);
}

double mapd(double val, double inputRangeBegin, double inputRangeEnd, double outputRangeBegin, double outputRangeEnd)
{
    return outputRangeBegin + ((val - inputRangeBegin) * (outputRangeEnd - outputRangeBegin)) / (inputRangeEnd - inputRangeBegin);
}
