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
    grid-template-columns: 10rem auto;
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
  <h3>Colors Time</h3>
  <form action="/colors">
    <label for="cl_h">Hours</label>
    <input id="cl_h" type="color" name="color_hour" value="{{colorHourKeyword}}">
    <label>Minutes</label>
    <input type="color" name="color_min" value="{{colorMinuteKeyword}}" width="auto">
    <input type="submit" value="Set Color">
  </form>
  <h3>Blinking & Brightness</h3>
  <form action="/blink">
    <lable>Enabled</lable>
    <input type="checkbox" name="enabled" {{blinkingEnabledKeyword}}>
    <label>Show Seconds</label>
    <input type="checkbox" name="show_seconds" {{secondsEnabledKeyword}}>
    <lable>Brightness</label>
    <input type="number" name="brightness" min="0" max="255" value="{{brightnessKeyword}}">
    <input type="submit" value="Set Blinking & Brightness">
  </form>
  <h3>Enviroment Colors</h3>
  <form>
    <label>Temperature</label>
    <input type="color" name="color_temp" value="{{colorTemperatureKeyword}}">
    <label>Pressure</label>
    <input type="color" name="color_press" value="{{colorPressureKeyword}}">
    <label>Humidity</label>
    <input type="color" name="color_hum" value="{{colorHumidityKeyword}}">
    <input type="submit" value="Set Colors">
  </form>
</body>
</html>
)rawliteral";
