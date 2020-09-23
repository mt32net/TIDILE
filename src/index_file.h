const char index_html[] = R"rawliteral(
<html>
<head>
  <title>Circle Clock Interface</title>
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
    Circle Clock Interface
  </h1>
  <h3>Colors Time</h3>
  <form method="post">
    <label for="cl_h">Hours</label>
    <input id="cl_h" type="color" name="color_hour">
    <label>Minutes</label>
    <input type="color" name="color_min">
    <input type="submit" value="Set Color">
  </form>
  <h3>Blinking</h3>
  <form action="/blink" method="post">
    <lable>Enabled</lable>
    <input type="checkbox" name="enabled">
    <input type="submit" value="Set Blinking">
  </form>
  <h3>Enviroment Colors</h3>
  <form>
    <label>Temperature</label>
    <input type="color" name="color_temp">
    <label>Pressure</label>
    <input type="color" name="color_press">
    <label>Humidity</label>
    <input type="color" name="color_hum">
    <input type="submit" value="Set Colors">
  </form>
</body>
</html>
)rawliteral";
