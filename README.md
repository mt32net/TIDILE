# TIDILE - Time Indication Device through Indirect Light Emittation

Or to put it simple: A clock with leds shining on your wall.

*Work in progess*

## Features

### Displays Enviroment Variables

The Clock can display the temperature, pressure and humidity with a simple tap. It has a build in sensor and you can also activate it remotely via HTTP.

### Web Configuration

The TIDILE features a web interface to quickly configure color, brightness and other settings on your phone or computer. And it looks like you configure your clock to look like!

### Automatic Deactivation At Preconfigured Times

If you want your TIDILE to not shine in the night, no problem! Configure activation and deactivation time.

### HTTP API for IoT

You can use the TIDILE for IoT applications and display custom information on it with a simple HTTP request.

## Build

You can build a TIDILE in a circular or linear shape (See pictures).

TIDILE uses WS2812b LEDS, also known as Neopixel. You can get a 1m strip with 144leds for around 15-20 Euros. Using 120 Leds (60 * 2), your clock will have a (perimeter) length of about 0.8m. You also need an ESP32 (around 5 Euros) and optionaly an BMP280 sensor (around 3 Euros). Some soldering (or jumper cables ;) ) is also required to connect the components together.

In case you buy a 144leds/m strip and use 120leds (recommended), your clock will have a theoretical (perimeter) length of 0,83m. In reallty though I would'nd trust these strips to be exactly 1m long. Dealing with this is easy when you build a linear TIDILE, but for the circular version you want your strip to end exactly at the other end.

## Implementation Details

### Enviroment Brightness Influence
