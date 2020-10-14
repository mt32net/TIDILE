#pragma once

const char style_css[] = R"rawliteral(
* {
  font-family: 'Calibri', sans-serif;
  color: rgb(236, 236, 236);
}

body {
  background-color: #080808;
  text-align: center;
}

form {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(160px, 50%));
  gap: 0.7rem;
  margin: auto;
  width: 90%;
  text-align: left;
}

input {
  outline: none;
  border: none;
  background-color: rgb(236, 236, 236);
  border-radius: 3px;
  font-size: 1.1rem;
  color: black;
}

.submit {
  grid-column: 1 / 3;
  justify-self: center;
}

h3,
h2,
h1 {
  text-decoration: underline;
  align-self: center;
}


/*clock elements*/

#clock {
  margin: 2rem auto 1rem auto;
  width: 90%;
  display: block;
}

#minutes {
  padding: 0.5rem;
  background-color: 'colorMinuteKeyword';
  border-radius: 5px;
  filter: blur(5px);
  width: calc(100% * ('minutesKeyword'/60));
}

#hours {
  padding: 0.5rem;
  background-color: 'colorHourKeyword';
  border-radius: 5px;
  filter: blur(5px);
  width: 1%;
  margin-left: calc(100% * ('hoursKeyword'/24));
  margin-top: -1rem;
}

#spacer {
  width: 10%;
}

#seconds {
  padding: 0.5rem;
  background-color: 'colorSecondKeyword';
  border-radius: 5px;
  filter: blur(7px);
  width: 1%;
  margin-top: -1rem;
  margin-left: calc(100% * ( 'secondsKeyword'/60));
}

#clockNightTime {
  visibility: hidden;
}


/*Switch Elements*/


/* The switch - the box around the slider */

.switch {
  position: relative;
  display: inline-block;
  width: 3rem;
  height: 1.5rem;
}


/* Hide default HTML checkbox */

.switch input {
  opacity: 0;
  width: 0;
  height: 0;
}


/* The slider */

.slider {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider:before {
  position: absolute;
  content: "";
  height: 1.3rem;
  width: 1.3rem;
  left: 0.1rem;
  bottom: 0.1rem;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked+.slider {
  background-color: #2196F3;
}

input:focus+.slider {
  box-shadow: 0 0 1px #2196F3;
}

input:checked+.slider:before {
  -webkit-transform: translateX(1.5rem);
  -ms-transform: translateX(1.5rem);
  transform: translateX(1.5rem);
}


/* Rounded sliders */

.slider.round {
  border-radius: 34px;
}

.slider.round:before {
  border-radius: 50%;
}
)rawliteral";