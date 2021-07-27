#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <FastLED.h>

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

    ///deserializes this Color from startPosition into this object
    ///@param preferences is the preference object
    void deserialize(Preferences *preferences, String name)
    {
        red = preferences->getInt((name + String("_r")).c_str());
        green = preferences->getInt((name + String("_g")).c_str());
        blue = preferences->getInt((name + String("_b")).c_str());
    }

    ///serializes this Color object into the given storage container
    ///@param preferences is the preference object
    void serialize(Preferences *preferences, String name)
    {
        preferences->putInt((name + String("_r")).c_str(), red);
        preferences->putInt((name + String("_g")).c_str(), green);
        preferences->putInt((name + String("_b")).c_str(), blue);
    }

    CRGB toCRGB()
    {
        return CRGB(red, green, blue);
    }

    bool operator==(const Color &c)
    {
        return red == c.red && green == c.green && blue == c.blue;
    }

    String toString()
    {
        return toHex();
    }

    String toHex()
    {
        return "#" + toTwoChars(String(red, HEX)) + toTwoChars(String(green, HEX)) + toTwoChars(String(blue, HEX));
    }

private:
    String toTwoChars(String s)
    {
        if (s.length() < 2)
            return String("0") + s;
        else
            return s;
    }
};

Color hexToColor(String input);
String colorToHex(Color color);