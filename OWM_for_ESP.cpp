// The streaming parser to use is not the Arduino IDE library manager default, but this one which
// is slightly different and renamed to avoid conflicts: https://github.com/Bodmer/JSON_Decoder
#include <JSON_Listener.h>
#include <JSON_Decoder.h>

#include "OWM_for_ESP.h"

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #ifdef SECURE_CONNECTION
    BearSSL::WiFiClientSecure client;
  #else
    WiFiClient client;
  #endif
#elif defined(ESP32)
  #ifdef SECURE_CONNECTION
    #include <WiFiClientSecure.h>
    WiFiClientSecure client;
  #else
    #include <WiFi.h>
    WiFiClient client;
  #endif
#endif

/***************************************************************************************
** Function name:           getWeather
** Description:             Setup the weather forecast request
***************************************************************************************/
// The structures etc are created by the sketch and passed to this function.
// Pass a nullptr for current, hourly or daily pointers to exclude in response.
bool OWM_Weather::getWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily,
                              String api_key, String latitude, String longitude,
                              String units, String language, String type) {

  data_set = "";

  if (type == "full") {
    api_id = 1;
    type = "onecall"; // Change type from human-understandable ("full") to OWT-understandable ("onecall")
  }
  else if (type == "current") {
    api_id = 2;
    type = "weather"; // Change type from human-understandable ("current") to OWT-understandable ("weather")
  }
  else {
    Serial.print("type (");
    Serial.print(type);
    Serial.println(") is incorrect. Set \"current\" or \"full\".");
  return false;
  }

  // Local copies of structure pointers, the structures are filled during parsing
  this->current = current;
  this->hourly  = hourly;
  this->daily   = daily;

  // Exclude some info by passing fn a NULL pointer to reduce memory needed
  String exclude = "";
  if (api_id == 1) {
    exclude += "minutely,alerts";
    if (!current) exclude += ",current";
    if (!hourly)  exclude += ",hourly";
    if (!daily)   exclude += ",daily";
  }
  /*
  //~ Random string in exclude uses small bug in OWM, which makes it possible
  //~ to get the weather at any time, not only once in 2-10 minutes.
  //~ Unfortunately, the data doesn't change anyway and the update doesn't
  //~ happen all the time, so I disabled this function
  if (api_id == 1) exclude += ",";
  exclude += String(ESP.random());
  */

  String url = "https://api.openweathermap.org/data/2.5/" + type + "?lat=" + latitude + "&lon=" + longitude + "&exclude=" + exclude + "&units=" + units + "&lang=" + language + "&appid=" + api_key;

  // Send GET request and feed the parser
  bool result = parseRequest(url);

  // Null out pointers to prevent crashes
  this->current = nullptr;
  this->hourly  = nullptr;
  this->daily   = nullptr;

  return result;
}

/***************************************************************************************
** Function name:           getCurrentWeather
** Description:             Setup the current weather request
***************************************************************************************/
bool OWM_Weather::getCurrentWeather(OWM_current *current, String api_key, String latitude,
                                    String longitude, String units, String language) {
  return getWeather(current, nullptr, nullptr, api_key, latitude, longitude, units, language, "current");
}

/***************************************************************************************
** Function name:           getFullWeather
** Description:             Setup the full weather request
***************************************************************************************/
bool OWM_Weather::getFullWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily,
                                 String api_key, String latitude,
                                 String longitude, String units, String language) {
  return getWeather(current, hourly, daily, api_key, latitude, longitude, units, language, "full");
}

/***************************************************************************************
** Function name:           parseRequest
** Description:             Fetches the JSON message and feeds to the parser
***************************************************************************************/
bool OWM_Weather::parseRequest(String url) {

#ifdef LOG_TIME
  uint32_t dt = millis();
#endif

#if defined(ESP8266) and defined(SECURE_CONNECTION)
  client.setInsecure();
#endif

  JSON_Decoder parser;
  parser.setListener(this);

  const char* host = "api.openweathermap.org";

#ifdef LOG_UNNECESSARY_INFO
  Serial.println("URL is " + url);

  Serial.print("Connecting...");
#endif

#ifdef SECURE_CONNECTION
  if (!client.connect(host, 443)) {
#else
 if (!client.connect(host, 80)) {
#endif
#ifdef LOG_ERRORS
    Serial.println(" Error: connection failed.");
#endif
    return false;
  }

  uint32_t timeout = millis();
  char c = 0;
  parseOK = false;

#ifdef SHOW_JSON
  int ccount = 0;
#endif

  // Send GET request
#ifdef LOG_UNNECESSARY_INFO
  Serial.print(" Sending GET request...");
#endif
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

#ifdef ESP8266
  while (client.available() || client.connected()) {
#else
  while (client.connected()) {
#endif
    String line = client.readStringUntil('\n');
    if (line == "\r") {
#ifdef LOG_UNNECESSARY_INFO
      Serial.print(" Header end found.");
#endif
      break;
    }

#ifdef SHOW_HEADER
    Serial.println(line);
#endif

    if ((millis() - timeout) > 5000UL) {
#ifdef LOG_ERRORS
      Serial.println(" Error: HTTP header timeout.");
#endif
      client.stop();
      return false;
    }
  }
#ifdef LOG_UNNECESSARY_INFO
  Serial.print(" Parsing JSON.");
#endif
  
  // Parse the JSON data, available() includes yields
#ifdef ESP8266
  while (client.available() || client.connected())
  {
    while (client.available()) {
#else
  while (client.available() > 0 || client.connected())
  {
    while (client.available() > 0) {
#endif
      c = client.read();
      parser.parse(c);

#ifdef SHOW_JSON
      if (c == '{' || c == '[' || c == '}' || c == ']') Serial.println();
      Serial.print(c); if (ccount++ > 100 && c == ',') {ccount = 0; Serial.println();}
#endif

      if ((millis() - timeout) > 8000UL) {
#ifdef LOG_ERRORS
        Serial.println(" Error: JSON parse client timeout.");
#endif
        parser.reset();
        client.stop();
        return false;
      }
#ifdef ESP32
      yield();
#endif
    }
  }
#ifdef LOG_UNNECESSARY_INFO
  Serial.println("");
#endif

#ifdef LOG_TIME
  Serial.print("Done in "); Serial.print(millis()-dt); Serial.println(" ms");
#endif

  parser.reset();

  client.stop();
  
  // A message has been parsed without error but the data-point correctness is unknown
  return parseOK;
}

/***************************************************************************************
** Function name:           key etc
** Description:             These functions are called while parsing the JSON message
***************************************************************************************/
void OWM_Weather::key(const char *key) {

  currentKey = key;

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Key >>>" + (String)key);
#endif
}

void OWM_Weather::startDocument() {

  currentParent = currentKey = currentSet = "";
  objectLevel = 0;
  valuePath = "";
  arrayIndex = 0;
  arrayLevel = 0;
  parseOK = true;

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Start document >>>");
#endif
}

void OWM_Weather::endDocument() {

  currentParent = currentKey = "";
  objectLevel = 0;
  valuePath = "";
  arrayIndex = 0;
  arrayLevel = 0;

#ifdef SHOW_CALLBACK
  Serial.print("\n<<< End document <<<");
#endif
}

void OWM_Weather::startObject() {

  if (arrayIndex == 0 && objectLevel == 1) currentParent = currentKey;
  currentSet = currentKey;
  objectLevel++;

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Start object level:" + (String) objectLevel + " array level:" + (String) arrayLevel + " array index:" + (String) arrayIndex +" >>>");
#endif
}

void OWM_Weather::endObject() {

  if (arrayLevel == 0) currentParent = "";
  if (arrayLevel == 1  && objectLevel == 2) arrayIndex++;
  objectLevel--;
  

#ifdef SHOW_CALLBACK
  Serial.print("\n<<< End object <<<");
#endif
}

void OWM_Weather::startArray() {

  arrayLevel++;
  valuePath = currentParent + "/" + currentKey; // aka = current Object, e.g. "daily:data"

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Start array " + valuePath + "/" + (String) arrayLevel + "/" + (String) arrayIndex +" >>>");
#endif
}

void OWM_Weather::endArray() {
  if (arrayLevel > 0) arrayLevel--;
  if (arrayLevel == 0) arrayIndex = 0;
  valuePath = "";

#ifdef SHOW_CALLBACK
  Serial.print("\n<<< End array <<<");
#endif
}

void OWM_Weather::whitespace(char c) {
}

void OWM_Weather::error( const char *message ) {
  Serial.print("\nParse error message: ");
  Serial.print(message);
  parseOK = false;
}

/***************************************************************************************
** Function name:           value (full or partial data set)
** Description:             Stores the parsed data in the structures for sketch access
***************************************************************************************/
void OWM_Weather::value(const char *val) {
  fullDataSet(val);
}

/***************************************************************************************
** Function name:           fullDataSet
** Description:             Collects full data set
***************************************************************************************/
void OWM_Weather::fullDataSet(const char *val) {

  String value = val;

  if (api_id == 1) // OneCall
  {
    // Start of JSON
    if (currentParent == "") {
      if (currentKey == "lat") current->lat = value.toFloat();
      else
      if (currentKey == "lon") current->lon = value.toFloat();
      else
      if (currentKey == "timezone") current->timezone = value;
      else
      if (currentKey == "timezone_offset") current->timezone_offset = (uint32_t)value.toInt();
    }

    // Current forecast - no array index - short path
    if (currentParent == "current") {
      data_set = "current";
      if (currentKey == "dt") current->dt = (uint32_t)value.toInt();
      else
      if (currentKey == "sunrise") current->sunrise = (uint32_t)value.toInt();
      else
      if (currentKey == "sunset") current->sunset = (uint32_t)value.toInt();
      else
      if (currentKey == "temp") current->temp = value.toFloat();
      else
      if (currentKey == "feels_like") current->feels_like = value.toFloat();
      else
      if (currentKey == "pressure") current->pressure = value.toFloat();
      else
      if (currentKey == "humidity") current->humidity = value.toInt();
      else
      if (currentKey == "dew_point") current->dew_point = value.toFloat();
      else
      if (currentKey == "uvi") current->uvi = value.toFloat();
      else
      if (currentKey == "clouds") current->clouds = value.toInt();
      else
      if (currentKey == "visibility") current->visibility = value.toInt();
      else
      if (currentKey == "wind_speed") current->wind_speed = value.toFloat();
      else
      if (currentKey == "wind_gust") current->wind_gust = value.toFloat();
      else
      if (currentKey == "wind_deg") current->wind_deg = (uint16_t)value.toInt();
      else

      if (currentSet == "rain") current->rain_1h = value.toFloat();
      else
      if (currentSet == "snow") current->snow_1h = value.toFloat();
      else

      if (currentKey == "id") current->id = value.toInt();
      else
      if (currentKey == "main") current->main = value;
      else
      if (currentKey == "description") current->description = value;
      else
      if (currentKey == "icon") current->icon = value;

      return;
    }

    // Hourly forecast
    if (currentParent == "hourly") {
      data_set = "hourly";
      
      if (arrayIndex >= MAX_HOURS) return;
      
      if (currentKey == "dt") hourly->dt[arrayIndex] = (uint32_t)value.toInt();
      else
      if (currentKey == "temp") hourly->temp[arrayIndex] = value.toFloat();
      else
      if (currentKey == "feels_like") hourly->feels_like[arrayIndex] = value.toFloat();
      else
      if (currentKey == "pressure") hourly->pressure[arrayIndex] = value.toFloat();
      else
      if (currentKey == "humidity") hourly->humidity[arrayIndex] = value.toInt();
      else
      if (currentKey == "dew_point") hourly->dew_point[arrayIndex] = value.toFloat();
      else
      if (currentKey == "clouds") hourly->clouds[arrayIndex] = value.toInt();
      else
      if (currentKey == "wind_speed") hourly->wind_speed[arrayIndex] = value.toFloat();
      else
      if (currentKey == "wind_gust") hourly->wind_gust[arrayIndex] = value.toFloat();
      else
      if (currentKey == "wind_deg") hourly->wind_deg[arrayIndex] = (uint16_t)value.toInt();
      else

      if (currentSet == "rain") hourly->rain[arrayIndex] = value.toFloat();
      else
      if (currentSet == "snow") hourly->snow[arrayIndex] = value.toFloat();
      else

      if (currentKey == "id") hourly->id[arrayIndex] = value.toInt();
      else
      if (currentKey == "main") hourly->main[arrayIndex] = value;
      else
      if (currentKey == "description") hourly->description[arrayIndex] = value;
      else
      if (currentKey == "icon") hourly->icon[arrayIndex] = value;
      else
      if (currentKey == "pop") hourly->pop[arrayIndex] = value.toFloat();

      return;
    }


    // Daily forecast
    if (currentParent == "daily") {
      data_set = "daily";
      
      if (arrayIndex >= MAX_DAYS) return;
      
      if (currentKey == "dt") daily->dt[arrayIndex] = (uint32_t)value.toInt();
      else
      if (currentKey == "sunrise") daily->sunrise[arrayIndex] = (uint32_t)value.toInt();
      else
      if (currentKey == "sunset") daily->sunset[arrayIndex] = (uint32_t)value.toInt();
      else
      if (currentKey == "pressure") daily->pressure[arrayIndex] = value.toFloat();
      else
      if (currentKey == "humidity") daily->humidity[arrayIndex] = value.toInt();
      else
      if (currentKey == "dew_point") daily->dew_point[arrayIndex] = value.toFloat();
      else
      if (currentKey == "clouds") daily->clouds[arrayIndex] = value.toInt();
      else
      if (currentKey == "wind_speed") daily->wind_speed[arrayIndex] = value.toFloat();
      else
      if (currentKey == "wind_gust") daily->wind_gust[arrayIndex] = value.toFloat();
      else
      if (currentKey == "wind_deg") daily->wind_deg[arrayIndex] = (uint16_t)value.toInt();
      else
      if (currentKey == "rain") daily->rain[arrayIndex] = value.toFloat();
      else
      if (currentKey == "snow") daily->snow[arrayIndex] = value.toFloat();
      else

      if (currentKey == "id") daily->id[arrayIndex] = value.toInt();
      else
      if (currentKey == "main") daily->main[arrayIndex] = value;
      else
      if (currentKey == "description") daily->description[arrayIndex] = value;
      else
      if (currentKey == "icon") daily->icon[arrayIndex] = value;
      else
      if (currentKey == "pop") daily->pop[arrayIndex] = value.toFloat();

      if (currentSet == "temp") {
        if (currentKey == "morn") daily->temp_morn[arrayIndex] = value.toFloat();
        else
        if (currentKey == "day") daily->temp_day[arrayIndex] = value.toFloat();
        else
        if (currentKey == "eve") daily->temp_eve[arrayIndex] = value.toFloat();
        else
        if (currentKey == "night") daily->temp_night[arrayIndex] = value.toFloat();
        else
        if (currentKey == "min") daily->temp_min[arrayIndex] = value.toFloat();
        else
        if (currentKey == "max") daily->temp_max[arrayIndex] = value.toFloat();
      }

      if (currentSet == "feels_like") {
        if (currentKey == "morn") daily->feels_like_morn[arrayIndex] = value.toFloat();
        else
        if (currentKey == "day") daily->feels_like_day[arrayIndex] = value.toFloat();
        else
        if (currentKey == "eve") daily->feels_like_eve[arrayIndex] = value.toFloat();
        else
        if (currentKey == "night") daily->feels_like_night[arrayIndex] = value.toFloat();
      }

      return;
    }
  }
  else if (api_id == 2) // Current
  {
    // Start of JSON
    if (currentParent == "") {
      if (currentKey == "visibility") current->visibility = value.toInt();
      else
      if (currentKey == "dt") current->dt = (uint32_t)value.toInt();
      else
      if (currentKey == "timezone") current->timezone_offset = (uint32_t)value.toInt();
      else
      if (currentKey == "id") current->city_id = (uint32_t)value.toInt();
      else
      if (currentKey == "name") current->city_name = value;
      
      return;
    }
    else if (currentParent == "coord") {
      data_set = "coord";

      if (currentKey == "lat") current->lat = value.toFloat();
      else
      if (currentKey == "lon") current->lon = value.toFloat();

      return;
    }
    else if (currentParent == "weather") {
      data_set = "weather";

      if (currentKey == "id") current->id = value.toInt();
      else
      if (currentKey == "main") current->main = value;
      else
      if (currentKey == "description") current->description = value;
      else
      if (currentKey == "icon") current->icon = value;

      return;
    }
    else if (currentParent == "main") {
      data_set = "main";

      if (currentKey == "temp") current->temp = value.toFloat();
      else
      if (currentKey == "feels_like") current->feels_like = value.toFloat();
      else
      if (currentKey == "pressure") current->pressure = value.toFloat();
      else
      if (currentKey == "humidity") current->humidity = value.toInt();
      else
      if (currentKey == "temp_min") current->temp_min = value.toFloat();
      else
      if (currentKey == "temp_max") current->temp_max = value.toFloat();
      else
      if (currentKey == "sea_level") current->sea_level = value.toInt();
      else
      if (currentKey == "grnd_level") current->grnd_level = value.toInt();

      return;
    }
    else if (currentParent == "wind") {
      data_set = "wind";

      if (currentKey == "speed") current->wind_speed = value.toFloat();
      else
      if (currentKey == "deg") current->wind_deg = (uint16_t)value.toInt();
      else
      if (currentKey == "wind_gust") current->wind_gust = value.toFloat();

      return;
    }
    else if (currentParent == "clouds") {
      data_set = "clouds";

      if (currentKey == "clouds") current->clouds = value.toInt();

      return;
    }
    else if (currentParent == "rain") {
      data_set = "rain";

      if (currentKey == "1h") current->rain_1h = value.toFloat();
      else
      if (currentKey == "3h") current->rain_3h = value.toFloat();

      return;
    }
    else if (currentParent == "snow") {
      data_set = "snow";

      if (currentKey == "1h") current->snow_1h = value.toFloat();
      else
      if (currentKey == "3h") current->snow_3h = value.toFloat();

      return;
    }
    else if (currentParent == "sys") {
      data_set = "sys";

      if (currentKey == "sunrise") current->sunrise = (uint32_t)value.toInt();
      else
      if (currentKey == "sunset") current->sunset = (uint32_t)value.toInt();
      else
      if (currentKey == "country") current->country = value;

      return;
    }
  }
}