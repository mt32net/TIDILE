#include "numbers.hpp"

String digitToTwoCharsDigit(int digit)
{
    return (digit <= 9) ? "0" + String(digit) : String(digit);
}