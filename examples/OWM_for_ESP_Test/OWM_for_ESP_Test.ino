// ========= User configured stuff starts here =========

// Change to suit your WiFi router
#define WIFI_SSID     "YourSSID"
#define WIFI_PASSWORD "YourPassword"

// OpenWeather API Details, replace x's with your API key
#define API_KEY       "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" // Obtain this from your OpenWeatherMap account

// Set both your longitude and latitude to at least 2 decimal places (the maximum accuracy is 2 digits after the decimal point; it's an OWM limitation)
#define LATITUDE      "51.50" // 90.00 to -90.00 negative for Southern hemisphere
#define LONGITUDE     "-0.12" // 180.00 to -180.00 negative for West

// Other settings can be found in the file "User_Setup.h" in the library folder (libraries/OWM_for_ESP/src/User_Setup.h). 
// You can set logging parameters there, set the maximum number of hours and days, set language and default units or disable secure connection and string variables.

// =========  User configured stuff ends here  =========

// Choose library to load
#if defined ESP8266
  #include <ESP8266WiFi.h>
#elif defined ESP32
  #include <WiFi.h>
#endif

#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder

#include <OWM_for_ESP.h>
OWM_Weather owm(API_KEY); // You can set API key and units here
// Example 1: OWM_Weather owm(); will set OWM_for_ESP without API key and units
// Example 2: OWM_Weather owm(API_KEY); will set OWM_for_ESP with API key and without units
// Example 3: OWM_Weather owm("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", IMPERIAL); will set OWM_for_ESP with API key and IMPERIAL units

uint32_t timezoneOffset; // Sets automatically

void setup() {
  Serial.begin(250000); // 115200 or less is very unstable for esp8266 when displaying a large data stream

  delay(500);
  Serial.printf("\nConnecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Connected!");

  // You can set API key with owm.setApiKey(), units with .setUnits()
  // owm.setApiKey("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  // owm.setUnits(IMPERIAL);

  Serial.println("\nIf you see the value 255 - most likely OWM is not providing this data for your location.");
  Serial.println("You can verify this by requesting it on your browser and analyzing the data yourself.");
  Serial.println("255 shows only in current, zero hour in hourly and zero day in daily.");
}

int type = 0; // You can set this value from 0 to 4 for convenience. If weather receieved, this variable changes
void loop() {
  bool weatherReceieved = false;

  // Creating the structures that hold the retrieved weather
  OWM_current *current = new OWM_current;
  OWM_hourly  *hourly  = new OWM_hourly;
  OWM_daily   *daily   = new OWM_daily;

  if (type == 0) {
    Serial.println("\nYou can get current weather with owm.getWeather(current, hourly, daily, LATITUDE, LONGITUDE, CURRENT).");
    Serial.println("Don't forget that current weather updates on server once at 10 minutes. If you want to use it, set big delay or change lat/lon.");
    Serial.println();

    // owm.getWeather() adds data to the structures and outputs true if the weather is received or false if an error occurred
    // Of course, you can use owm.getWeather() without bool variable. It's used only to check success
    weatherReceieved = owm.getWeather(current, hourly, daily, LATITUDE, LONGITUDE, CURRENT);
  }
  else if (type == 1) {
    // You can do smth like that:
    OWM_hourly  *hourlyNull  = nullptr; 
    OWM_daily   *dailyNull   = nullptr;
    // but it's better to insert nullptr directly into the function. Check type 4 for more info.

    Serial.println("\nIt's better to check current weather with owm.getWeather(current, nullptr, nullptr, LATITUDE, LONGITUDE, CURRENT)");
    Serial.println("because you can save more RAM (also it's a bit faster):");
    Serial.println();

    weatherReceieved = owm.getWeather(current, hourlyNull, dailyNull, LATITUDE, LONGITUDE, CURRENT);
  }
  else if (type == 2) {    
    Serial.println("\nBest way to get current weather is owm.getCurrentWeather(current, LATITUDE, LONGITUDE).");
    Serial.println("This function is completely identical to owm.getWeather(current, nullptr, nullptr, LATITUDE, LONGITUDE, CURRENT):");
    Serial.println();

    weatherReceieved = owm.getCurrentWeather(current, LATITUDE, LONGITUDE);
  }
  else if (type == 3) {
    Serial.println("\nYou can get full weather info with owm.getWeather(current, hourly, daily, LATITUDE, LONGITUDE, FULL):");
    Serial.println();

    weatherReceieved = owm.getWeather(current, hourly, daily, LATITUDE, LONGITUDE, FULL);
  }
  else if (type == 4) {
    Serial.println("\nBest way to get full weather is owm.getFullWeather(current, hourly, daily, LATITUDE, LONGITUDE).");
    Serial.println("This function is completely identical to owm.getWeather(current, hourly, daily, LATITUDE, LONGITUDE, FULL).");
    Serial.println("Of course you can use nullptr. Check what you will get with owm.getFullWeather(current, nullptr, daily, LATITUDE, LONGITUDE):");
    Serial.println();

    // You can use owm.getFullWeather() without bool variable
    weatherReceieved = owm.getFullWeather(current, nullptr, daily, LATITUDE, LONGITUDE);
  }



  if (weatherReceieved) { // weatherReceieved == false if an error occurred
    Serial.println("\n\nWeather was received:");

    timezoneOffset = current->timezoneOffset; // It's using to display the time correctly

    // Print receieved data:
    Serial.print("Latitude              : "); Serial.println(current->lat);
    Serial.print("Longitude             : "); Serial.println(current->lon);
    if (type < 3) { // Exclusive info for current request
      Serial.print("cityId                : "); Serial.println(current->cityId);
     #ifdef ENABLE_STRINGS
      Serial.print("cityName              : "); Serial.println(current->cityName);
      Serial.print("countryCode           : "); Serial.println(current->countryCode);
     #endif
    }
    else { // Exclusive info for full request
     #ifdef ENABLE_STRINGS
      Serial.print("timezoneName          : "); Serial.println(current->timezoneName);
     #endif
    }
    Serial.println();

    Serial.print("timezoneOffset        : "); Serial.println(current->timezoneOffset);
    Serial.print("serverUpdateTime      : "); Serial.print(strTime(current->serverUpdateTime));
    Serial.println();

    if (current) {
      Serial.println("############# Current weather #############\n");

      Serial.print("sunriseTime           : "); Serial.print(strTime(current->sunriseTime));
      Serial.print("sunsetTime            : "); Serial.print(strTime(current->sunsetTime));
      Serial.println();

      Serial.print("mainTemp              : "); Serial.println(current->mainTemp);
      Serial.print("feelsLikeTemp         : "); Serial.println(current->feelsLikeTemp);
      if (type < 3) { // Exclusive info for current request
        Serial.print("minTemp               : "); Serial.println(current->minTemp);
        Serial.print("maxTemp               : "); Serial.println(current->maxTemp);
      }
      Serial.println();

      Serial.print("humidity (%)          : "); Serial.println(current->humidity);
      Serial.print("pressureSeaLevel      : "); Serial.println(current->pressureSeaLevel);
      if (type < 3) { // Exclusive info for current request
        Serial.print("pressureGroundLevel   : "); Serial.println(current->pressureGroundLevel);
      }
      Serial.print("windSpeed             : "); Serial.println(current->windSpeed);
      Serial.print("windGust              : "); Serial.println(current->windGust);
      Serial.print("windDirection         : "); Serial.println(current->windDirection);
      Serial.println();

      if (type > 2) { // Exclusive info for full request
        Serial.print("dewPoint              : "); Serial.println(current->dewPoint);
      }
      Serial.print("rainForLast1h         : "); Serial.println(current->rainForLast1h);
      Serial.print("snowForLast1h         : "); Serial.println(current->snowForLast1h);
      if (type < 3) { // Exclusive info for current request
        Serial.print("rainForLast3h         : "); Serial.println(current->rainForLast3h);
        Serial.print("snowForLast3h         : "); Serial.println(current->snowForLast3h);\
      }
      Serial.println();

      Serial.print("cloudiness (%)        : "); Serial.println(current->cloudiness);
      Serial.print("visibility            : "); Serial.println(current->visibility);
      if (type > 2) { // Exclusive info for full request
        Serial.print("ultravioletIndex      : "); Serial.println(current->ultravioletIndex);
      }
      Serial.println();

      Serial.print("weatherCondId         : "); Serial.println(current->weatherCondId);
      Serial.print("Short status          : "); Serial.println(owm.getShortStatus(current->weatherCondId));
      Serial.print("Full status           : "); Serial.println(owm.getStatus(current->weatherCondId));
      Serial.print("Status in 16 symbols  : "); Serial.println(owm.getStatus16(current->weatherCondId));
      Serial.print("weatherIconId         : "); Serial.println(current->weatherIconId);
      Serial.print("Weather icon code     : "); Serial.println(owm.getIconCode(current->weatherIconId));
      Serial.print("Weather icon file     : "); Serial.println(owm.getIconCode(current->weatherIconId, "icon", ".bmp"));
      Serial.println();

    }
    if (type == 3) {
      if (hourly) { // Exclusive info for full request
        Serial.println("############# Hourly weather  #############\n");
        for (int i = 0; i < MAX_HOURS; i++) {
          Serial.print("Hour "); Serial.print(i); Serial.println(" summary:\n");

          Serial.print("time                  : "); Serial.print(strTime(hourly->time[i]));
          Serial.println();

          Serial.print("mainTemp              : "); Serial.println(hourly->mainTemp[i]);
          Serial.print("feelsLikeTemp         : "); Serial.println(hourly->feelsLikeTemp[i]);
          Serial.println();

          Serial.print("humidity (%)          : "); Serial.println(hourly->humidity[i]);
          Serial.print("pressureSeaLevel      : "); Serial.println(hourly->pressureSeaLevel[i]);
          Serial.print("windSpeed             : "); Serial.println(hourly->windSpeed[i]);
          Serial.print("windGust              : "); Serial.println(hourly->windGust[i]);
          Serial.print("windDirection         : "); Serial.println(hourly->windDirection[i]);
          Serial.println();

          Serial.print("dewPoint              : "); Serial.println(hourly->dewPoint[i]);
          Serial.print("rainForLast1h         : "); Serial.println(hourly->rainForLast1h[i]);
          Serial.print("snowForLast1h         : "); Serial.println(hourly->snowForLast1h[i]);
          Serial.println();

          Serial.print("cloudiness (%)        : "); Serial.println(hourly->cloudiness[i]);
          Serial.print("visibility            : "); Serial.println(hourly->visibility[i]);
          Serial.print("ultravioletIndex      : "); Serial.println(hourly->ultravioletIndex[i]);
          Serial.print("Probability of prec.  : "); Serial.println(hourly->pop[i]);
          Serial.println();

          Serial.print("weatherCondId         : "); Serial.println(hourly->weatherCondId[i]);
          Serial.print("Short status          : "); Serial.println(owm.getShortStatus(hourly->weatherCondId[i]));
          Serial.print("Full status           : "); Serial.println(owm.getStatus(hourly->weatherCondId[i]));
          Serial.print("Status in 16 symbols  : "); Serial.println(owm.getStatus16(hourly->weatherCondId[i]));
          Serial.print("weatherIconId         : "); Serial.println(hourly->weatherIconId[i]);
          Serial.print("Weather icon code     : "); Serial.println(owm.getIconCode(hourly->weatherIconId[i]));
          Serial.print("Weather icon file     : "); Serial.println(owm.getIconCode(hourly->weatherIconId[i], "icon", ".bmp"));
          Serial.println("\n");

        }
      }
    }
    if (type == 3 or type == 4) {
      if (daily) { // Exclusive info for full request
        Serial.println("#############  Daily weather  #############\n");
        for (int i = 0; i < MAX_DAYS; i++) {
          Serial.print("Day "); Serial.print(i); Serial.println(" summary:\n");

          Serial.print("time                  : "); Serial.print(strTime(daily->time[i]));
          Serial.print("sunriseTime           : "); Serial.print(strTime(daily->sunriseTime[i]));
          Serial.print("sunsetTime            : "); Serial.print(strTime(daily->sunsetTime[i]));
          Serial.print("moonriseTime          : "); Serial.print(strTime(daily->moonriseTime[i]));
          Serial.print("moonsetTime           : "); Serial.print(strTime(daily->moonsetTime[i]));
          Serial.println();

          Serial.print("minTemp               : "); Serial.println(daily->minTemp[i]);
          Serial.print("maxTemp               : "); Serial.println(daily->maxTemp[i]);
          Serial.println();

          Serial.print("morningTemp           : "); Serial.println(daily->morningTemp[i]);
          Serial.print("dayTemp               : "); Serial.println(daily->dayTemp[i]);
          Serial.print("eveningTemp           : "); Serial.println(daily->eveningTemp[i]);
          Serial.print("nightTemp             : "); Serial.println(daily->nightTemp[i]);
          Serial.println();

          Serial.print("morningFeelsLikeTemp  : "); Serial.println(daily->morningFeelsLikeTemp[i]);
          Serial.print("dayFeelsLikeTemp      : "); Serial.println(daily->dayFeelsLikeTemp[i]);
          Serial.print("eveningFeelsLikeTemp  : "); Serial.println(daily->eveningFeelsLikeTemp[i]);
          Serial.print("nightFeelsLikeTemp    : "); Serial.println(daily->nightFeelsLikeTemp[i]);
          Serial.println();

          Serial.print("humidity (%)          : "); Serial.println(daily->humidity[i]);
          Serial.print("pressureSeaLevel      : "); Serial.println(daily->pressureSeaLevel[i]);
          Serial.print("windSpeed             : "); Serial.println(daily->windSpeed[i]);
          Serial.print("windGust              : "); Serial.println(daily->windGust[i]);
          Serial.print("windDirection         : "); Serial.println(daily->windDirection[i]);
          Serial.println();

          Serial.print("dewPoint              : "); Serial.println(daily->dewPoint[i]);
          Serial.print("rain                  : "); Serial.println(daily->rain[i]);
          Serial.print("snow                  : "); Serial.println(daily->snow[i]);
          Serial.println();

          Serial.print("cloudiness (%)        : "); Serial.println(daily->cloudiness[i]);
          Serial.print("ultravioletIndex      : "); Serial.println(daily->ultravioletIndex[i]);
          Serial.print("pop                   : "); Serial.println(daily->pop[i]);
          Serial.println();

          Serial.print("moonPhase             : "); Serial.println(daily->moonPhase[i]);
          Serial.println();

          Serial.print("weatherCondId         : "); Serial.println(daily->weatherCondId[i]);
          Serial.print("Short status          : "); Serial.println(owm.getShortStatus(daily->weatherCondId[i]));
          Serial.print("Full status           : "); Serial.println(owm.getStatus(daily->weatherCondId[i]));
          Serial.print("Status in 16 symbols  : "); Serial.println(owm.getStatus16(daily->weatherCondId[i]));
          Serial.print("weatherIconId         : "); Serial.println(daily->weatherIconId[i]);
          Serial.print("Weather icon code     : "); Serial.println(owm.getIconCode(daily->weatherIconId[i]));
          Serial.print("Weather icon file     : "); Serial.println(owm.getIconCode(daily->weatherIconId[i], "icon", ".bmp"));
          Serial.println("\n");

        }
      }
    }

    type = (type+1) % 5;
    delay(125 * 1000); // We can make 1000 requests a day. 74880 secs per day / 125 secs = 599 requests per day
  }
  else {
    Serial.println("Weather wasn't received\n");
    delay(5 * 1000); // Trying to get the data again
  }

  // Deleting structures to free up memory - otherwise you will get a RAM overflow and ESP will crash and reboot
  delete current;
  delete hourly;
  delete daily;
}

String strTime(time_t unixTime) { // This function converts unix time to a time string
  unixTime += timezoneOffset;
  return ctime(&unixTime);
}