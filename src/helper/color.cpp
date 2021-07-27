#include "color.hpp"
#include "../config/config_includes.hpp"

// Thank you stack overflow!
Color hexToColor(String input)
{
    long rgb = strtol(input.c_str() + 1, 0, 16); // parse as Hex, skipping the leading '#'
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;
    return Color(r, g, b);
}

String colorToHex(Color color)
{
    return color.toHex();
}