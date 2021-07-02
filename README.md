# OpenWeatherMap library for ESP8266/ESP32 and Arduino IDE

### English | [Русский](README_ru.md)

This library works with several types of APIs: [Current](https://openweathermap.org/current) (current weather only) and [One Call](https://openweathermap.org/api/one-call-api) (weather with forecasts).

Learn how to work with the library with the test example `OWM_for_ESP_Test.ino`. You can also change the logging parameters, connection type (*unsecured* HTTP or *secured* HTTPS), number of minutes and hours (for One Call) in the `User_Setup.h` file, located in the library folder.

To use it, you need the JSON parsing library: https://github.com/Bodmer/JSON_Decoder.

ESP8266 and ESP32 don't have hardware cryptography, so secure connection via HTTPS takes 2.5-3.5 times longer. If you are not worried about security, you can enable the unsecured connection mode. Also don't forget that the official documentation [recommended](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/bearssl-client-secure-class.html#cpu-requirements) to use 160 MHZ CPU speed for ESP8266 with a secure connection, which is set by default.

This library is an improved version of [Bodmer's OpenWeather library](https://github.com/Bodmer/OpenWeather), which has more functionality, multiple bugfixes and optimizations. You can find out more info in the Releases page. You can transfer your project from Bodmer's lib to this with minimal changes.


Projects with this library:
* [mxkmn/esp8266_weather_station](https://github.com/mxkmn/esp8266_weather_station) // Weather station using esp8266, lcd1602 display, two buttons for navigation, able to show the weather forecast for the next week and 48 hours, fully prepared for changes to another type of display or navigation. At the moment it is exclusively in Russian, but can be translated into any other language.

If you have published a new project using OWM_for_ESP - put a link in **Issues** marked `new project`, write some info and I will add it to the project list.