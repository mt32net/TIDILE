#include "helper.hpp"

HelperClass Helper;

HelperClass::HelperClass()
{
}

String HelperClass::timeIntToTimeString(int timeInt)
{
  String time = (timeInt <= 999) ? "0" + String(timeInt) : String(timeInt);
  return time.substring(0, 2) + ":" + time.substring(2, 4);
}

int HelperClass::timeStringToTimeInt(String timeString)
{
  timeString.remove(2, 1);
  return timeString.toInt();
}

// Thank you stack overflow!
Color HelperClass::hexToColor(String input)
{
  long rgb = strtol(input.c_str() + 1, 0, 16); // parse as Hex, skipping the leading '#'
  int r = (rgb >> 16) & 0xFF;
  int g = (rgb >> 8) & 0xFF;
  int b = rgb & 0xFF;
  return Color(r, g, b);
}

String HelperClass::colorToHex(Color color)
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

ClockTime HelperClass::getTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    timeTries++;
    if (timeTries > 10)
      ESP.restart();
    return ClockTime{1, 1, 1, 1, 1, 1};
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

String HelperClass::getDateTimeToString()
{
  ClockTime time = getTime();
  return digitToTwoCharsDigit(time.hours) + ":" + digitToTwoCharsDigit(time.minutes) + ":" + digitToTwoCharsDigit(time.seconds) + " " + digitToTwoCharsDigit(time.day) + "/" + digitToTwoCharsDigit(time.month) + "/" + digitToTwoCharsDigit(time.year + 1900);
}

bool HelperClass::isNightTime(ClockConfig configuration, ClockTime time)
{
  return ((digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() > configuration.nightTimeBegin ||
  (digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() < configuration.nightTimeEnd ||
  configuration.tempOverwriteNightTime) 
  && configuration.nightTimeLight;
}

void HelperClass::setupWiFi()
{
  WiFi.begin();
  delay(200);

  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.mode(WIFI_AP_STA);
    delay(500);
    WiFi.beginSmartConfig();
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
      Serial.println(WiFi.smartConfigDone());
      tries++;
      if (tries > 20)
        ESP.restart();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
}

String HelperClass::digitToTwoCharsDigit(int digit)
{
  return (digit <= 9) ? "0" + String(digit) : String(digit);
}

void HelperClass::resetOverwriteNightTimeIfLegit(ClockConfig configuration, ClockTime time)
{
  if(!isNightTime(configuration, time) && (digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes)).toInt() >= configuration.nightTimeEnd)
    configuration.tempOverwriteNightTime = false;
}

int HelperClass::hmsToTimeInt(ClockTime time){
	return (digitToTwoCharsDigit(time.hours) + digitToTwoCharsDigit(time.minutes) + digitToTwoCharsDigit(time.seconds)).toInt();
}
