#pragma once

#pragma region UIkeywords
#define COLORHOURKEYWORD "{{colorHourKeyword}}"
#define COLORMINUTEKEYWORD "{{colorMinuteKeyword}}"
#define COLORSECONDSKEYWORD "{{colorSecondKeyword}}"
#define BLINKINGENABLEDKEYWORD "{{blinkingEnabledKeyword}}"
#define BRIGHTNESSKEYWORD "{{brightnessKeyword}}"
#define COLORTEMPERATUREKEYWORD "{{colorTemperatureKeyword}}"
#define COLORPRESSUREKEYWORD "{{colorPressureKeyword}}"
#define SHOWSECONDSKEYWORD "{{secondsEnabledKeyword}}"
#define NIGHTTIMESTARTKEYWORD "{{nighttimeStartKeyword}}"
#define NIGHTTIMEENDKEYWORD "{{nighttimeEndKeyword}}"
#define NIGHTTIMEENABLEDKEYWORD "{{nightTimeEnabledKeyword}}"
#pragma endregion

const char index_html[] = R"rawliteral(
<html>
<head>
  <title>TIDILE Interface</title>
</head>
<style>
  * {
    font-family: 'Calibri', sans-serif;
  }
  form {
    display: grid;
    grid-template-columns: 50% 50%;
    gap: 0.7rem;
  }
  input {
    outline: none;
    border: none;
    background-color: orangered;
    border-radius: 3px;
    font-size: 1.5rem;
  }
  h3, h2, h1 {
    text-decoration: underline;
  }
</style>
<body>
  <h1>
    TIDILE Interface
  </h1>
  <h3>Night Time</h3>
  <form action="/time">
    <label for="timeEnabled">Enable Night Time</label>
    <input id="timeEnabled" type="checkbox" name="time_enabled" {{nightTimeEnabledKeyword}}>
    <label for="timeB">Begin of Night Time</label>
    <div>
      <input id="timeB" type="time" name="begin_time" value="{{nighttimeStartKeyword}}" width="auto">
      <small>Enter the time when the clock should start to turn off if it´s dark in the room</small>
    </div>
    <label for="timeE">Minutes</label>
    <div>
      <input id="timeE" type="time" name="end_time" value="{{nighttimeEndKeyword}}" width="auto">
      <small>Enter the time when the clock should stop to turn off if it´s dark in the room</small>
    </div>
  <input type="submit" value="Set Night Time">
  </form>
  <h3>Colors Time</h3>
  <form action="/colors">
    <label for="cl_h">Hours</label>
    <input id="cl_h" type="color" name="color_hour" value="{{colorHourKeyword}}">
    <label for="cl_m">Minutes</label>
    <input id="cl_m" type="color" name="color_min" value="{{colorMinuteKeyword}}" width="auto">
    <label for="cl_s">Seconds</label>
    <input id="cl_s" type="color" name="color_sec" value="{{colorSecondKeyword}}" width="auto">
    <input type="submit" value="Set Color">
  </form>
  <h3>Blinking & Brightness</h3>
  <form action="/blink">
    <lable>Enabled</lable>
    <input type="checkbox" name="enabled" {{blinkingEnabledKeyword}}>
    <label>Show Seconds</label>
    <input type="checkbox" name="show_seconds" {{secondsEnabledKeyword}}>
    <lable for="bright">Brightness</lable>
    <div>
      <input id="bright" type="number" name="brightness" min="0" max="255" value="2">
      <br>
      <small>Enter a value between 0 and 255 to set the LED brightness (Caution: Higher brightness results in a higher energy consumption. e.g. with 144 LEDS in use and the brightness set to 200 the power consumption is around 2 amps</small>
    </div>
    <input type="submit" value="Set Blinking & Brightness">
  </form>
  <!--- env begin---!>
  <h3>Enviroment Colors</h3>
  <form action="/envcolors">
    <label>Temperature</label>
    <input type="color" name="color_temp" value="{{colorTemperatureKeyword}}">
    <label>Pressure</label>
    <input type="color" name="color_press" value="{{colorPressureKeyword}}">
    <input type="submit" value="Set Colors">
  </form>
  <!--- env end---!>
</body>
</html>
)rawliteral";
