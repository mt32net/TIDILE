#pragma once

#pragma region UIkeywords
#define COLORHOURKEYWORD "'colorHourKeyword'"
#define COLORMINUTEKEYWORD "'colorMinuteKeyword'"
#define COLORSECONDSKEYWORD "'colorSecondKeyword'"
#define DIMMSECONDSKEYWORD "'dimmSecondsKeyword'"
#define BRIGHTNESSKEYWORD "'brightnessKeyword'"
#define COLORTEMPERATUREKEYWORD "'colorTemperatureKeyword'"
#define COLORPRESSUREKEYWORD "'colorPressureKeyword'"
#define SHOWSECONDSKEYWORD "'secondsEnabledKeyword'"
#define NIGHTTIMESTARTKEYWORD "'nighttimeStartKeyword'"
#define NIGHTTIMEENDKEYWORD "'nighttimeEndKeyword'"
#define NIGHTTIMEENABLEDKEYWORD "'nightTimeEnabledKeyword'"
#define INFLUENCEKEYWORD "'envLightInfluence'"
#define CURRENTTIMEKEYWORD "'currentTimeKeyword'"
#define MINUTESKEYWORD "'minutesKeyword'"
#define HOURSKEYWORD "'hoursKeyword'"
#define SECONDSKEYWORD "'secondsKeyword'"
#pragma endregion

const char index_html[] = R"rawliteral(
<html>

<head>
  <title>TIDILE Interface</title>
  <meta charset="UTF-8">
  <link rel="stylesheet" href="styles.css">
</head>

<body>
  <h1>TIDILE Interface</h1>
  <div>Last ESP Time (d/m/y): 'currentTimeKeyword'</div>
  <div id="curTime"></div>
  <div id="clock">
    <div id="minutes"></div>
    <div id="hours"></div>
    <div id="seconds"></div>
  </div>
  <div id="clockNightTime">Night time enabled</div>
  <h3>Night Time</h3>
  <form action="/time">
    <label>Enable Night Time <small>Info: Between both times should be 12pm</small></label>
    <label class="switch">
      <input id="nightTimeEnabled" type="checkbox" name="time_enabled" 'nightTimeEnabledKeyword'>
      <span class="slider round"></span>
    </label>
    <label for="timeB">Begin of Night Time at</label>
    <div>
      <input id="timeB" type="time" name="begin_time" value="'nighttimeStartKeyword'" width="auto">
      <small>Enter the time when the clock should start to turn off if it´s dark in the room</small>
    </div>
    <label for="timeE">End of Night Time at</label>
    <div>
      <input id="timeE" type="time" name="end_time" value="'nighttimeEndKeyword'" width="auto">
      <small>Enter the time when the clock should stop to turn off if it´s dark in the room</small>
    </div>
    <div class="submit">
      <input type="submit" name="settings" value="Set Night Time">
      <input type="submit" name="nightTimeTilMorning" value="Set Night Time until Morning">
    </div>
  </form>
  <h3>Colors for Time</h3>
  <form action="/colors">
    <label for="cl_h">Hours</label>
    <input id="cl_h" type="color" name="color_hour" value="'colorHourKeyword'">
    <label for="cl_m">Minutes</label>
    <input id="cl_m" type="color" name="color_min" value="'colorMinuteKeyword'" width="auto">
    <label id="dimm_label" for="cl_s">Seconds</label>
    <input id="cl_s" type="color" name="color_sec" value="'colorSecondKeyword'" width="auto">
    <label for="dim_sec">Dimm Seconds instead of using different color</label>
    <label class="switch">
      <input id="dim_sec" type="checkbox" name="dimm_seconds" 'dimmSecondsKeyword' onchange="onDimmSecondsChange()">
      <span class="slider round"></span>
    </label>
    <input type="submit" class="submit" value="Set Color">
  </form>
  <script>
    function onDimmSecondsChange() {
      var checked = document.getElementById("dim_sec").checked;
      var colorSelector = document.getElementById("cl_s");
      var secondsLight = document.getElementById("seconds");
      var dimmLabel = document.getElementById("dimm_label");
      if (checked) {
        colorSelector.style.visibility = "hidden";
        dimmLabel.style.visibility = "hidden";
        secondsLight.style.visibility = "hidden";
      } else {
        colorSelector.style.visibility = "visible";
        dimmLabel.style.visibility = "visible";
        if (document.getElementById("clock").style.visibility == "hidden")
          secondsLight.style.visibility = "visible";
      }
    };

    function setTimeBar() {
      var time = new Date();
      var minutes = document.getElementById("minutes");
      var hours = document.getElementById("hours");
      var seconds = document.getElementById("seconds");
      minutes.style.width = (100 * (time.getMinutes() / 60)) + "%";
      hours.style.marginLeft = (100 * (time.getHours() / 24)) + "%";
      seconds.style.marginLeft = (100 * (time.getSeconds() / 60)) + "%";
      document.getElementById("curTime").innerHTML = "Current System Time: " + time.toGMTString();
    };

    function loop() {
      setTimeBar();
      var time = new Date();
      var elementNighTimeBegin = document.getElementById("timeB");
      var elementNighTimeEnd = document.getElementById("timeE");
      var elementClockNightTime = document.getElementById("clockNightTime");
      var elementClock = document.getElementById("clock");

      var nightTimeBegin = new Date();
      nightTimeBegin.setDate(0);
      nightTimeBegin.setHours(elementNighTimeBegin.value.split(":")[0]);
      nightTimeBegin.setMinutes(elementNighTimeBegin.value.split(":")[1]);
      nightTimeBegin.setSeconds(0);

      var nightTimeEnd = new Date();
      nightTimeEnd.setDate(0);
      nightTimeEnd.setHours(elementNighTimeEnd.value.split(":")[0]);
      nightTimeEnd.setMinutes(elementNighTimeEnd.value.split(":")[1]);
      nightTimeEnd.setSeconds(0);
      time.setDate(0);

      if (time.getTime() > nightTimeBegin.getTime() || time.getTime() < nightTimeEnd.getTime() && document.getElementById("nightTimeEnabled").checked) {
        elementClock.style.visibility = "hidden";
        elementClockNightTime.style.visibility = "visible";
      } else {
        elementClock.style.visibility = "visible";
        elementClockNightTime.style.visibility = "hidden";
      }
    };
    onDimmSecondsChange();
    var timeUpdater = setInterval(() => loop(), 990);
  </script>
  <h3>Blinking & Brightness</h3>
  <form action="/blink">
    <label>Show Seconds</label>
    <label class="switch">
      <input type="checkbox" name="show_seconds" 'secondsEnabledKeyword'>
      <span class="slider round"></span>
    </label>
    <lable for="bright">Brightness</lable>
    <div>
      <input id="bright" type="number" name="brightness" min="0" max="255" value="'brightnessKeyword'">
      <br>
      <small>Enter a value between 0 and 255 to set the LED brightness (Caution: Higher brightness results in a higher energy consumption. e.g. with 144 LEDS in use and the brightness set to 200 the power consumption is around 2 amps</small>
    </div>
    <label>Enviroment Light Influence</label>
    <input id="influence" type="number" name="influence" min="0" max="100" value="'envLightInfluence'">
    <input type="submit" class="submit" value="Set Blinking & Brightness">
  </form>
  <!--- env begin --->
  <h3>Enviroment Colors</h3>
  <form action="/envcolors">
    <label>Temperature</label>
    <input type="color" name="color_temp" value="'colorTemperatureKeyword'">
    <label>Pressure</label>
    <input type="color" name="color_press" value="'colorPressureKeyword'">
    <input type="submit" class="submit" value="Set Colors">
  </form>
  <!--- env end --->
</body>

</html>
)rawliteral";
