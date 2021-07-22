#include "color.hpp"

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
    String red = String(color.red, HEX);
    if (red.length() == 1)
        red = "0" + red;
    String green = String(color.green, HEX);
    if (green.length() == 1)
        green = "0" + green;
    String blue = String(color.blue, HEX);
    if (blue.length() == 1)
        blue = "0" + blue;
    return "#" + red + green + blue;
}