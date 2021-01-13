// ========= User configured stuff starts here =========

// Change to suit your WiFi router
#define WIFI_SSID     "Your_SSID"
#define WIFI_PASSWORD "Your_password"

// OpenWeather API Details, replace x's with your API key
#define API_KEY       "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" // Obtain this from your OpenWeatherMap account

// Set both your longitude and latitude to at least 4 decimal places
#define LATITUDE      "51.5085" // 90.00 to -90.00 negative for Southern hemisphere
#define LONGITUDE     "-0.1257" // 180.00 to -180.00 negative for West

#define UNITS         "metric"  // See notes tab
#define LANGUAGE      "en"      // See notes tab

// Further configuration settings can be found in the library "User_Setup.h" file

// =========  User configured stuff ends here  =========

// Choose library to load
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else // ESP32
  #include <WiFi.h>
#endif

#include <JSON_Decoder.h>

#include <OWM_for_ESP.h>
OWM_Weather owm; // Weather forecast library instance

uint32_t timezone_offset = 0;

#include <Time.h> // Just using this library for unix time conversion

void setup() { 
  Serial.begin(250000); // Fast to stop it holding up the stream
                        // 115200 or less is very unstable

  Serial.printf("\nConnecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(" Connected!");

  Serial.println("\nIf you see the value 255 - most likely OWM is not providing this data for your location.\nYou can verify this by requesting it on your browser and analyzing the data yourself.");
}

void loop() {
  for (int i = 0; i < 5; i++) {
    printWeather(i); // Check what do "i" in printWeather()
    delay(125 * 1000); // We can make 1000 requests a day. 74880 secs per day / 125 secs = 599 requests per day
  }
}

void printWeather(int type) {
  bool weatherReceieved = false;

  // Create the structures that hold the retrieved weather
  OWM_current *current = new OWM_current;
  OWM_hourly  *hourly  = new OWM_hourly;
  OWM_daily   *daily   = new OWM_daily;

  if (type == 0) {
    Serial.println("\nYou can get current weather with owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, \"current\").");
    Serial.println("Don't forget that current weather updates on server once at 10 minutes. If you want to use it, set big delay or change lat/lon.");
    Serial.println();

    // owm.getWeather() adds data to the structures and outputs true if the weather is received or false if an error occurred
    // Of course, you can use owm.getWeather() without bool variable
    weatherReceieved = owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, "current");
  }
  else if (type == 1) {
    OWM_hourly  *hourlyNull  = nullptr; // you can do smth like that
    OWM_daily   *dailyNull   = nullptr; // but I don't recommend to make variables with nullptr
    
    Serial.println("\nIt's better to check current weather with owm.getWeather(current, nullptr, nullptr, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, \"current\")");
    Serial.println("because you can save more RAM (also it's a bit faster).");
    Serial.println();

    weatherReceieved = owm.getWeather(current, hourlyNull, dailyNull, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, "current");
  }
  else if (type == 2) {    
    Serial.println("\nBest way to get current weather is owm.getCurrentWeather(current, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE).");
    Serial.println("This function is completely identical to owm.getWeather(current, nullptr, nullptr, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, \"current\").");
    Serial.println();

    // You can use owm.getCurrentWeather() without bool variable
    weatherReceieved = owm.getCurrentWeather(current, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE);
  }
  else if (type == 3) {
    
    Serial.println("\nYou can get full weather info with owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, \"full\").");
    Serial.println("But did you know that UNITS, LANGUAGE and type aren't necessary? If you call owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE),");
    Serial.println("it will be completely identical to owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE, \"metric\", \"en\", \"full\").");
    Serial.println();

    weatherReceieved = owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, "full");
    // weatherReceieved = owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE);
  }
  else if (type == 4) {
    Serial.println("\nBest way to get full weather is owm.getFullWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE).");
    Serial.println("This function is completely identical to owm.getWeather(current, hourly, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE, \"current\").");
    Serial.println("Also you should know that UNITS and LANGUAGE aren't necessary for owm.getCurrentWeather() and owm.getFullWeather().");
    Serial.println("Of course you can use nullptr. Check what you will get with owm.getFullWeather(current, nullptr, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE):");
    Serial.println();

    // You can use owm.getFullWeather() without bool variable
    weatherReceieved = owm.getFullWeather(current, nullptr, daily, API_KEY, LATITUDE, LONGITUDE, UNITS, LANGUAGE);
  }

  if (weatherReceieved) {
    Serial.println("\nWeather was received:\n");

    timezone_offset = current->timezone_offset; // It's using to display the time correctly

    // Print receieved data:
    Serial.print("dt (time when the weather updated on the server): "); Serial.print(strTime(current->dt));
    Serial.print("Timezone_offset     : "); Serial.println(current->timezone_offset);
    if (type > 2) { // exclusive info for full request
      Serial.print("Timezone            : "); Serial.println(current->timezone);
    }
    Serial.print("Latitude            : "); Serial.println(current->lat);
    Serial.print("Longitude           : "); Serial.println(current->lon);
    Serial.println();
  
    if (current) {
      Serial.println("############### Current weather ###############\n");
      Serial.print("sunrise          : "); Serial.print(strTime(current->sunrise));
      Serial.print("sunset           : "); Serial.print(strTime(current->sunset));
      Serial.print("temp             : "); Serial.println(current->temp);
      Serial.print("feels_like       : "); Serial.println(current->feels_like);
      Serial.print("pressure         : "); Serial.println(current->pressure);
      Serial.print("humidity         : "); Serial.println(current->humidity);
      Serial.print("clouds           : "); Serial.println(current->clouds);
      Serial.print("visibility       : "); Serial.println(current->visibility);
      Serial.print("wind_speed       : "); Serial.println(current->wind_speed);
      Serial.print("wind_gust        : "); Serial.println(current->wind_gust);
      Serial.print("wind_deg         : "); Serial.println(current->wind_deg);
      Serial.print("rain_1h          : "); Serial.println(current->rain_1h);
      Serial.print("snow_1h          : "); Serial.println(current->snow_1h);
      Serial.println();
      Serial.print("id               : "); Serial.println(current->id);
      Serial.print("main             : "); Serial.println(current->main);
      Serial.print("description      : "); Serial.println(current->description);
      Serial.print("icon             : "); Serial.println(current->icon);
      Serial.println();
      if (type > 2) { // exclusive info for full request
        Serial.print("dew_point        : "); Serial.println(current->dew_point);
        Serial.print("uvi              : "); Serial.println(current->uvi);
      }
      if (type < 3) { // exclusive info for current request
        Serial.print("city_id          : "); Serial.println(current->city_id);
        Serial.print("city_name        : "); Serial.println(current->city_name);
        Serial.print("country          : "); Serial.println(current->country);
        Serial.print("rain_3h          : "); Serial.println(current->rain_3h);
        Serial.print("snow_3h          : "); Serial.println(current->snow_3h);
        Serial.print("temp_min         : "); Serial.println(current->temp_min);
        Serial.print("temp_max         : "); Serial.println(current->temp_max);
        Serial.print("sea_level        : "); Serial.println(current->sea_level);
        Serial.print("grnd_level       : "); Serial.println(current->grnd_level);
      }
  
      Serial.println();
    }
    if (type == 3) {
      if (hourly) { // exclusive info for full request
        Serial.println("############### Hourly weather  ###############\n");
        for (int i = 0; i < MAX_HOURS; i++) {
          Serial.print("Hourly summary  "); if (i < 10) Serial.print(" "); Serial.print(i);
          Serial.println();
          Serial.print("dt (time)        : "); Serial.print(strTime(hourly->dt[i]));
          Serial.print("temp             : "); Serial.println(hourly->temp[i]);
          Serial.print("feels_like       : "); Serial.println(hourly->feels_like[i]);
          Serial.print("pressure         : "); Serial.println(hourly->pressure[i]);
          Serial.print("humidity         : "); Serial.println(hourly->humidity[i]);
          Serial.print("pop              : "); Serial.println(hourly->pop[i]);
          Serial.print("dew_point        : "); Serial.println(hourly->dew_point[i]);
          Serial.print("clouds           : "); Serial.println(hourly->clouds[i]);
          Serial.print("wind_speed       : "); Serial.println(hourly->wind_speed[i]);
          Serial.print("wind_gust        : "); Serial.println(hourly->wind_gust[i]);
          Serial.print("wind_deg         : "); Serial.println(hourly->wind_deg[i]);
          Serial.print("rain             : "); Serial.println(hourly->rain[i]);
          Serial.print("snow             : "); Serial.println(hourly->snow[i]);
          Serial.println();
          Serial.print("id               : "); Serial.println(hourly->id[i]);
          Serial.print("main             : "); Serial.println(hourly->main[i]);
          Serial.print("description      : "); Serial.println(hourly->description[i]);
          Serial.print("icon             : "); Serial.println(hourly->icon[i]);
    
          Serial.println();
        }
      }
    }
    if (type == 3 or type == 4) {
      if (daily) { // exclusive info for full request
        Serial.println("###############  Daily weather  ###############\n");
        for (int i = 0; i < MAX_DAYS; i++) {
          Serial.print("Daily summary   "); if (i < 10) Serial.print(" "); Serial.print(i);
          Serial.println();
          Serial.print("dt (time)        : "); Serial.print(strTime(daily->dt[i]));
          Serial.print("sunrise          : "); Serial.print(strTime(daily->sunrise[i]));
          Serial.print("sunset           : "); Serial.print(strTime(daily->sunset[i]));
    
          Serial.print("temp.morn        : "); Serial.println(daily->temp_morn[i]);
          Serial.print("temp.day         : "); Serial.println(daily->temp_day[i]);
          Serial.print("temp.eve         : "); Serial.println(daily->temp_eve[i]);
          Serial.print("temp.night       : "); Serial.println(daily->temp_night[i]);
          Serial.print("temp.min         : "); Serial.println(daily->temp_min[i]);
          Serial.print("temp.max         : "); Serial.println(daily->temp_max[i]);
    
          Serial.print("feels_like.morn  : "); Serial.println(daily->feels_like_morn[i]);
          Serial.print("feels_like.day   : "); Serial.println(daily->feels_like_day[i]);
          Serial.print("feels_like.eve   : "); Serial.println(daily->feels_like_eve[i]);
          Serial.print("feels_like.night : "); Serial.println(daily->feels_like_night[i]);
    
          Serial.print("pressure         : "); Serial.println(daily->pressure[i]);
          Serial.print("humidity         : "); Serial.println(daily->humidity[i]);
          Serial.print("dew_point        : "); Serial.println(daily->dew_point[i]);
          Serial.print("uvi              : "); Serial.println(daily->uvi[i]);
          Serial.print("clouds           : "); Serial.println(daily->clouds[i]);
          Serial.print("visibility       : "); Serial.println(daily->visibility[i]);
          Serial.print("wind_speed       : "); Serial.println(daily->wind_speed[i]);
          Serial.print("wind_gust        : "); Serial.println(daily->wind_gust[i]);
          Serial.print("wind_deg         : "); Serial.println(daily->wind_deg[i]);
          Serial.print("rain             : "); Serial.println(daily->rain[i]);
          Serial.print("snow             : "); Serial.println(daily->snow[i]);
          Serial.println();
          Serial.print("id               : "); Serial.println(daily->id[i]);
          Serial.print("main             : "); Serial.println(daily->main[i]);
          Serial.print("description      : "); Serial.println(daily->description[i]);
          Serial.print("icon             : "); Serial.println(daily->icon[i]);
          Serial.print("pop              : "); Serial.println(daily->pop[i]);
    
          Serial.println();
        }
      }
    }
  }
  else Serial.println("Weather wasn't received\n");

  // Delete to free up memory - otherwise you will get a memory overflow and ESP will reboot
  delete current;
  delete hourly;
  delete daily;
}

String strTime(time_t unixTime) { // Convert unix time to a time string
  unixTime += timezone_offset;
  return ctime(&unixTime);
}
