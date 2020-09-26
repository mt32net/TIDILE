#include "helper.hpp"

HelperClass Helper;

HelperClass::HelperClass(){

}

String HelperClass::timeIntToTimeString(int timeInt){
    String time = (timeInt <= 999) ? "0" + String(timeInt) : String(timeInt);
    return time.substring(0, 2) + ":" + time.substring(2, 4);
}

int HelperClass::timeStringToTimeInt(String timeString){
    timeString.remove(2,1);
    return timeString.toInt();
}

// Thank you stack overflow!
Color HelperClass::hexToColor(String input) {
    long rgb = strtol(input.c_str() + 1, 0, 16); // parse as Hex, skipping the leading '#'
    int r = (rgb >> 16) & 0xFF;
    int g = (rgb >> 8) & 0xFF;
    int b = rgb & 0xFF;
    return Color(r, g, b);
}

String HelperClass::colorToHex(Color color){
    String red = String(color.red, HEX);
    if(red.length() == 1) red = "0" + red;
    String green = String(color.green, HEX);
    if(green.length() == 1) green = "0" + green;
    String blue = String(color.blue, HEX);
    if(blue.length() == 1) blue = "0" + blue;
    return "#" + red + green + blue;
}

ClockTime HelperClass::getTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    timeTries++;
    if(timeTries > 20) ESP.restart();
    return ClockTime{0, 0, 0, 0, 0, 0};
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  return ClockTime{
    seconds : timeinfo.tm_sec,
    minutes : timeinfo.tm_min,
    hours : timeinfo.tm_hour,
    day : timeinfo.tm_mday,
    month : timeinfo.tm_mon,
    year : timeinfo.tm_year
  };
}