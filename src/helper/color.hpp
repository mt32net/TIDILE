#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>
#include <ArduinoJson.h>

struct Color
{
    byte red = 0;
    byte green = 0;
    byte blue = 0;

    Color(byte r, byte g, byte b)
    {
        red = r;
        green = g;
        blue = b;
    }

    Color()
    {
    }

    void deserialize(JsonObject *json)
    {
        red = (*json)["r"];
        green = (*json)["g"];
        blue = (*json)["b"];
    }

    void serialize(JsonObject *json)
    {
        (*json)["r"] = red;
        (*json)["g"] = green;
        (*json)["b"] = blue;
    }

    CRGB toCRGB()
    {
        return CRGB(red, green, blue);
    }

    bool operator==(const Color &c)
    {
        return red == c.red && green == c.green && blue == c.blue;
    }

    Color operator*(const int factor)
    {
        red = this->red * factor;
        green = this->green * factor;
        blue = this->blue * factor;
        return *this;
        // TODO check for memory leak
        // ??? delete this;
    }

    String toString()
    {
        return toHex();
    }

    String toHex()
    {
        return "#" + toTwoChars(String(red, HEX)) + toTwoChars(String(green, HEX)) + toTwoChars(String(blue, HEX));
    }

    static Color hexToColor(String input);
    static String colorToHex(Color color);

private:
    String toTwoChars(String s)
    {
        if (s.length() < 2)
            return String("0") + s;
        else
            return s;
    }
};

