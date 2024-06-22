# OpenWeatherMap library for Espressif microcontrollers and Arduino IDE/PlatformIO

OWM_for_ESP is a simple and optimized library to work with OpenWeatherMap. It works with several types of APIs: [Current](https://openweathermap.org/current) (current weather only) and [One Call](https://openweathermap.org/api/one-call-api) (weather with forecast).

To use it, you also need to download the JSON parsing library: https://github.com/Bodmer/JSON_Decoder.

## Secure connection

This library supports secure connection using HTTPS, which is enabled by default.

Since ESPs don't have hardware cryptography, it takes 2.5-3.5 times longer. If you are not worried about security, you can enable the unsecured connection mode.

## Library usage

With the `User_Setup.h` file you can change language of condition info, connection type (*unsecured* HTTP or *secured* HTTPS), number of minutes and hours (for One Call), exclude strings with description from API (it can be got with `getStatus`), setup logging parameters.

Available methods:

* `.getWeather(*current, *hourly, *daily, latitude, longitude, type)` takes **OWM_current**, **OWM_hourly** and **OWM_daily** variables, user latitude and longitude, API type, which is used to form the request. It adds all received data to the structures from which you can easily retrieve the necessary data later.
* `.getCurrentWeather(*current, latitude, longitude)` similar to `.getWeather(*current, nullptr, nullptr, latitude, longitude, CURRENT)`.
* `.getFullWeather(*current, *hourly, *daily, latitude, longitude)` similar to `.getWeathergetFullWeather(*current, *hourly, *daily, latitude, longitude, FULL)`.

* `.setApiKey(apiKey)` saves your key for future weather retrieval.
* `.setUnits(units)` saves the preferred type of units.

* `.getIconCode(iconId, begin, end)` is used to get the code of an icon from its internal library ID. This method was added to save RAM.
* `.getShortStatus(id)` is used to get a simplified description of the weather from its official ID. This method was added to save RAM and to easily edit states if the official ones don't satisfy you.
* `.getStatus(id)` is used to get a full description of the weather from its official ID. This method was added to save RAM and to easily edit states if the official ones don't satisfy you.
* `.getStatus16(id)` similar to `.getStatus(id)`, but outputs shortened states to 16 characters especially for displays.

You can also check the example `OWM_for_ESP_Test.ino`.

## Weather receiving speed comparison in milliseconds

Tested with PlatformIO Espressif 32 platform v5.3.0, max CPU and flash speed, secure connection, no strings.

### Current

Board | Connection | Header | JSON parsing | *Full receieve time*
-- | -- | -- | -- | --
ESP8266 | 2163 | 157 | 11 | 2331
ESP32 | 1217 | 222 | 14 | 1453
ESP32-C3 | 1056 | 207 | 11 | 1274
ESP32-S2 | 1060 | 160 | 15 | **1235**

### Full (One Call): current, hourly (48 hours), daily (8 days)

Board | Connection | Header | JSON parsing | *Full receieve time*
-- | -- | -- | -- | --
ESP8266 | 2298 | 1842 | 365 | 4505
ESP32 | 1205 | 844 | 333 | 2382
ESP32-C3 | 1157 | 752 | 295 | **2206**
ESP32-S2 | 1139 | 489 | 588 | 2217

### Full (One Call): current, without hourly, daily (8 days)

Board | Connection | Header | JSON parsing | *Full receieve time*
-- | -- | -- | -- | --
ESP8266 | 2300 | 738 | 90 | 3128
ESP32 | 1225 | 373 | 89 | 1687
ESP32-C3 | 1157 | 272 | 75 | 1505
ESP32-S2 | 1119 | 183 | 143 | **1446**

## Projects with this library

* [mxkmn/esp8266_weather_station](https://github.com/mxkmn/esp8266_weather_station) // Weather station using esp8266, lcd1602 display, two buttons for navigation, able to show the weather forecast for the next week and 48 hours, fully prepared for changes to another type of display or navigation. At the moment it is exclusively in Russian, but can be translated into any other language.

If you have published a new project using OWM_for_ESP - put a link in **Issues** marked `new project`, write some info and I will add it to the project list.
