# OpenWeatherMap library for ESP8266/ESP32 and Arduino IDE

### English | [Русский](README_ru.md)

#### OWM_for_ESP is a simple and optimized library to work with OpenWeatherMap. It works with several types of APIs: [Current](https://openweathermap.org/current) (current weather only) and [One Call](https://openweathermap.org/api/one-call-api) (weather with forecast).

To use it, you need the JSON parsing library: https://github.com/Bodmer/JSON_Decoder.

Library usage
=====
* `.getWeather(*current, *hourly, *daily, latitude, longitude, type)` takes structures of types **OWM_current**, **OWM_hourly** and **OWM_daily**, user latitude and longitude and API type, which is used to form the request. It adds all received data to the structures from which you can easily retrieve the necessary data later.
* `.getCurrentWeather(*current, latitude, longitude)` similar to `.getWeather(*current, nullptr, nullptr, latitude, longitude, CURRENT)`.
* `.getFullWeather(*current, *hourly, *daily, latitude, longitude)` similar to `.getWeathergetFullWeather(*current, *hourly, *daily, latitude, longitude, FULL)`.

* `.setApiKey(apiKey)` saves your key for future weather retrieval.
* `.setUnits(units)` saves the preferred type of units.

* `.getIconCode(iconId, begin, end)` is used to get the code of an icon from its internal library ID. This method was added to save RAM.
* `.getShortStatus(id)` is used to get a simplified description of the weather from its official ID. This method was added to save RAM and to easily edit states if the official ones don't satisfy you.
* `.getStatus(id)` is used to get a full description of the weather from its official ID. This method was added to save RAM and to easily edit states if the official ones don't satisfy you.
* `.getStatus16(id)` similar to `.getShortStatus(id)`, but outputs shortened states to 16 characters especially for displays.

Learn how to work with the library with the test example `OWM_for_ESP_Test.ino`. You can also change the logging parameters, connection type (*unsecured* HTTP or *secured* HTTPS), include string variables, number of minutes and hours (for One Call) or change language for conditions translations in the `User_Setup.h` file, located in the library folder.

ESP8266 and ESP32 don't have hardware cryptography, so secure connection via HTTPS takes 2.5-3.5 times longer. If you are not worried about security, you can enable the unsecured connection mode. Also don't forget that the official documentation [recommended](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/bearssl-client-secure-class.html#cpu-requirements) to use 160 MHZ CPU speed for ESP8266 with a secure connection, which is set by default.
***
#### Projects with this library:
* [mxkmn/esp8266_weather_station](https://github.com/mxkmn/esp8266_weather_station) // Weather station using esp8266, lcd1602 display, two buttons for navigation, able to show the weather forecast for the next week and 48 hours, fully prepared for changes to another type of display or navigation. At the moment it is exclusively in Russian, but can be translated into any other language.

If you have published a new project using OWM_for_ESP - put a link in **Issues** marked `new project`, write some info and I will add it to the project list.