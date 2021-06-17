// The streaming parser to use is not the Arduino IDE library manager default, but this one which
// is slightly different and renamed to avoid conflicts: https://github.com/Bodmer/JSON_Decoder
#include <JSON_Listener.h>
#include <JSON_Decoder.h>

#include <sntp.h> // only to get time (used for secure connection)

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

const char* host = "api.openweathermap.org";

// This certificate will be valid until 2038, then it should be replaced with a new one.
// If you want to get new certificates, check the python code: https://youtu.be/Wm1xKj4bKsY?t=869
const char* ROOT_CA = \ 
"-----BEGIN CERTIFICATE-----\n" \ 
"MIIF3jCCA8agAwIBAgIQAf1tMPyjylGoG7xkDjUDLTANBgkqhkiG9w0BAQwFADCB\n" \ 
"iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \ 
"cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \ 
"BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTAw\n" \ 
"MjAxMDAwMDAwWhcNMzgwMTE4MjM1OTU5WjCBiDELMAkGA1UEBhMCVVMxEzARBgNV\n" \ 
"BAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0plcnNleSBDaXR5MR4wHAYDVQQKExVU\n" \ 
"aGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNVBAMTJVVTRVJUcnVzdCBSU0EgQ2Vy\n" \ 
"dGlmaWNhdGlvbiBBdXRob3JpdHkwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIK\n" \ 
"AoICAQCAEmUXNg7D2wiz0KxXDXbtzSfTTK1Qg2HiqiBNCS1kCdzOiZ/MPans9s/B\n" \ 
"3PHTsdZ7NygRK0faOca8Ohm0X6a9fZ2jY0K2dvKpOyuR+OJv0OwWIJAJPuLodMkY\n" \ 
"tJHUYmTbf6MG8YgYapAiPLz+E/CHFHv25B+O1ORRxhFnRghRy4YUVD+8M/5+bJz/\n" \ 
"Fp0YvVGONaanZshyZ9shZrHUm3gDwFA66Mzw3LyeTP6vBZY1H1dat//O+T23LLb2\n" \ 
"VN3I5xI6Ta5MirdcmrS3ID3KfyI0rn47aGYBROcBTkZTmzNg95S+UzeQc0PzMsNT\n" \ 
"79uq/nROacdrjGCT3sTHDN/hMq7MkztReJVni+49Vv4M0GkPGw/zJSZrM233bkf6\n" \ 
"c0Plfg6lZrEpfDKEY1WJxA3Bk1QwGROs0303p+tdOmw1XNtB1xLaqUkL39iAigmT\n" \ 
"Yo61Zs8liM2EuLE/pDkP2QKe6xJMlXzzawWpXhaDzLhn4ugTncxbgtNMs+1b/97l\n" \ 
"c6wjOy0AvzVVdAlJ2ElYGn+SNuZRkg7zJn0cTRe8yexDJtC/QV9AqURE9JnnV4ee\n" \ 
"UB9XVKg+/XRjL7FQZQnmWEIuQxpMtPAlR1n6BB6T1CZGSlCBst6+eLf8ZxXhyVeE\n" \ 
"Hg9j1uliutZfVS7qXMYoCAQlObgOK6nyTJccBz8NUvXt7y+CDwIDAQABo0IwQDAd\n" \ 
"BgNVHQ4EFgQUU3m/WqorSs9UgOHYm8Cd8rIDZsswDgYDVR0PAQH/BAQDAgEGMA8G\n" \ 
"A1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQEMBQADggIBAFzUfA3P9wF9QZllDHPF\n" \ 
"Up/L+M+ZBn8b2kMVn54CVVeWFPFSPCeHlCjtHzoBN6J2/FNQwISbxmtOuowhT6KO\n" \ 
"VWKR82kV2LyI48SqC/3vqOlLVSoGIG1VeCkZ7l8wXEskEVX/JJpuXior7gtNn3/3\n" \ 
"ATiUFJVDBwn7YKnuHKsSjKCaXqeYalltiz8I+8jRRa8YFWSQEg9zKC7F4iRO/Fjs\n" \ 
"8PRF/iKz6y+O0tlFYQXBl2+odnKPi4w2r78NBc5xjeambx9spnFixdjQg3IM8WcR\n" \ 
"iQycE0xyNN+81XHfqnHd4blsjDwSXWXavVcStkNr/+XeTWYRUc+ZruwXtuhxkYze\n" \ 
"Sf7dNXGiFSeUHM9h4ya7b6NnJSFd5t0dCy5oGzuCr+yDZ4XUmFF0sbmZgIn/f3gZ\n" \ 
"XHlKYC6SQK5MNyosycdiyA5d9zZbyuAlJQG03RoHnHcAP9Dc1ew91Pq7P8yF1m9/\n" \ 
"qS3fuQL39ZeatTXaw2ewh0qpKJ4jjv9cJ2vhsE/zB+4ALtRZh8tSQZXq9EfX7mRB\n" \ 
"VXyNWQKV3WKdwrnuWih0hKWbt5DHDAff9Yk2dDLWKMGwsAvgnEzDHNb842m1R0aB\n" \ 
"L6KCq9NjRHDEjf8tM7qtj3u1cIiuPhnPQCjY/MiQu12ZIvVS5ljFH4gxQ+6IHdfG\n" \ 
"jjxDah2nGN59PRbxYvnKkKj9\n" \ 
"-----END CERTIFICATE-----\n" ;


/***************************************************************************************
** Function name:           getWeather
** Description:             Setup the weather forecast request
***************************************************************************************/
// The structures etc are created by the sketch and passed to this function.
// Pass a nullptr for current, hourly or daily pointers to exclude in response.
bool OWM_Weather::getWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily,
                              String api_key, String latitude, String longitude,
                              String units, String language, String type) {

  if (type == "full") {
    _apiId = 1;
    type = "onecall"; // Change type from human-understandable ("full") to OWT-understandable ("onecall")
  }
  else if (type == "current") {
    _apiId = 2;
    type = "weather"; // Change type from human-understandable ("current") to OWT-understandable ("weather")
  }
  else {
    Serial.println("type (" + type + ") is incorrect. Set \"current\" or \"full\".");
    return false;
  }

  // Local copies of structure pointers, the structures are filled during parsing
  this->current = current;
  this->hourly  = hourly;
  this->daily   = daily;

  // Exclude some info by passing fn a NULL pointer to reduce memory needed
  String exclude = "";
  if (type == "onecall") {
    exclude += "minutely,alerts";
    if (!current) exclude += ",current";
    if (!hourly)  exclude += ",hourly";
    if (!daily)   exclude += ",daily";
  }
  /*~ Random string in exclude uses small bug in OWM, which makes it possible
  //~ to get the weather at any time, not only once in 2-10 minutes.
  //~ Unfortunately, the data doesn't change anyway and the update doesn't
  //~ happen all the time, so I disabled this function
  if (type == "onecall") exclude += ",";
  exclude += String(ESP.random());
  */

  String url = "https://api.openweathermap.org/data/2.5/" + type + "?lat=" + latitude + "&lon=" + longitude + "&exclude=" +  exclude + "&units=" + units + "&lang=" + language + "&appid=" + api_key;

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
  uint32_t timer;
  #ifdef LOG_TIME
    uint32_t startTimer = millis();
    timer = millis();
  #endif

  JSON_Decoder parser;
  parser.setListener(this);
  _parseOk = false;

  #ifdef LOG_UNNECESSARY_INFO
    Serial.println("Connecting to " + url);
  #endif

  #ifdef SECURE_CONNECTION
    #ifdef ESP8266
      if (time(nullptr) < 250000) { // Set current time for HTTPS connection
        #ifdef LOG_UNNECESSARY_INFO
          Serial.print("Setting time using SNTP... ");
        #endif

        sntp_stop();
        sntp_setservername(0, "time.nist.gov");
        sntp_init();

        uint8_t tickCounter = 0;
        while (time(nullptr) < 250000) {
          if (tickCounter > 49) {
            #ifdef LOG_UNNECESSARY_INFO
              if (tickCounter == 50) Serial.print("Please wait... ");
              else if (tickCounter == 60) Serial.print("Oh, so long... ");
              else if (tickCounter == 70) Serial.print("Once the time will load... ");
            #endif
            if (tickCounter == 140) { // ~100 seconds
              #ifdef LOG_ERRORS
                Serial.println("Error: getting time failed.");
              #endif
              return false;
            }
            delay(1000);
          }
          else delay(150);

          tickCounter++;
        }

        #ifdef LOG_TIME
          Serial.print("Time received in "); Serial.print(millis() - timer); Serial.print(" ms. ");
          timer = millis();
        #endif
      }
      
      BearSSL::X509List cert(ROOT_CA);
      client.setTrustAnchors(&cert);
    #else // ESP32
      client.setCACert(ROOT_CA);
    #endif
  if (!client.connect(host, 443)) {
  #else
  if (!client.connect(host, 80)) {
  #endif
  #ifdef LOG_ERRORS
    Serial.println("Error: connection failed.");
  #endif
    return false;
  }
#ifdef LOG_TIME
  Serial.print("Connected in "); Serial.print(millis() - timer); Serial.print(" ms. ");
#endif
  timer = millis();

#ifdef SHOW_JSON
  int ccount = 0;
#endif

  // Send GET request
#ifdef LOG_UNNECESSARY_INFO
  Serial.print("Sending GET request... ");
#endif
  client.print("GET " + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

#ifdef ESP8266
  while (client.connected() or client.available()) {
#else
  while (client.connected()) {
#endif
    String line = client.readStringUntil('\n');
#ifdef SHOW_HEADER
    Serial.println(line);
#endif
    if (line == "\r") {
#ifdef LOG_UNNECESSARY_INFO
      Serial.print("Header end found. ");
#endif
      break;
    }

    if ((millis() - timer) > 2500) {
#ifdef LOG_ERRORS
      Serial.println("Error: HTTP header timeout.");
#endif
      client.stop();
      return false;
    }
  }
#ifdef LOG_TIME
  Serial.print("Got header in "); Serial.print((millis() - timer)); Serial.print(" ms. ");
#endif
#ifdef LOG_UNNECESSARY_INFO
  Serial.print("Parsing JSON. ");
#endif
#ifdef SHOW_JSON
      Serial.println();
#endif
  timer = millis();
  char c = 0;
  // Parse the JSON data, available() includes yields
  while (client.available() || client.connected()) {
    while (client.available()) {
      c = client.read();
      parser.parse(c);

#ifdef SHOW_JSON
      Serial.print(c);
#endif

      if ((millis() - timer) > 5000) {
#ifdef LOG_ERRORS
        Serial.println("Error: JSON parse client timeout.");
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
#ifdef SHOW_JSON
  Serial.println();
#endif

  if (_objectLevel != 0) {
    #ifdef LOG_ERRORS
      Serial.println("Error: JSON end not received.");
    #endif
    parser.reset();
    client.stop();
    return false;
  }

#ifdef LOG_TIME
  Serial.print("JSON receieved and parsed in "); Serial.print((millis() - timer)); Serial.print(" ms. ");
  Serial.print("Success in "); Serial.print(millis()-startTimer); Serial.println(" ms.");
#endif

  parser.reset();
  client.stop();

  // A message has been parsed without error but the data-point correctness is unknown
  return _parseOk;
}

/***************************************************************************************
** Function name:           key etc
** Description:             These functions are called while parsing the JSON message
***************************************************************************************/
void OWM_Weather::key(const char *key) {
  _currentKey = key;

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Key >>>" + (String)key);
#endif
}

void OWM_Weather::startDocument() {
  _currentParent = _currentKey = _currentSet = "";
  _objectLevel = 0;
  _valuePath = "";
  _arrayIndex = 0;
  _arrayLevel = 0;
  _parseOk = true;

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Start document >>>");
#endif
}

void OWM_Weather::endDocument() {
  _currentParent = _currentKey = "";
  _objectLevel = 0;
  _valuePath = "";
  _arrayIndex = 0;
  _arrayLevel = 0;

#ifdef SHOW_CALLBACK
  Serial.println("\n<<< End document <<<");
#endif
}

void OWM_Weather::startObject() {
  if (_arrayIndex == 0 && _objectLevel == 1) _currentParent = _currentKey;
  _currentSet = _currentKey;
  _objectLevel++;

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Start object level:" + (String) _objectLevel + " array level:" + (String) _arrayLevel + " array index:" + (String) _arrayIndex +" >>>");
#endif
}

void OWM_Weather::endObject() {
  if (_arrayLevel == 0) _currentParent = "";
  if (_arrayLevel == 1  && _objectLevel == 2) _arrayIndex++;
  _objectLevel--;
  
#ifdef SHOW_CALLBACK
  Serial.print("\n<<< End object <<<");
#endif
}

void OWM_Weather::startArray() {
  _arrayLevel++;
  _valuePath = _currentParent + "/" + _currentKey; // aka = current Object, e.g. "daily:data"

#ifdef SHOW_CALLBACK
  Serial.print("\n>>> Start array " + _valuePath + "/" + (String) _arrayLevel + "/" + (String) _arrayIndex +" >>>");
#endif
}

void OWM_Weather::endArray() {
  if (_arrayLevel > 0) _arrayLevel--;
  if (_arrayLevel == 0) _arrayIndex = 0;
  _valuePath = "";

#ifdef SHOW_CALLBACK
  Serial.print("\n<<< End array <<<");
#endif
}

void OWM_Weather::error(const char *message) {
  Serial.print("\nParse error message: ");
  Serial.print(message);
  _parseOk = false;
}

/***************************************************************************************
** Function name:           value (full or partial data set)
** Description:             Stores the parsed data in the structures for sketch access
***************************************************************************************/
void OWM_Weather::value(const char *val) {
  String value = val;

  if (_apiId == 1) { // OneCall
    if (_currentParent == "") { // Start of JSON
      if (_currentKey == "lat") current->lat = value.toFloat();
      else if (_currentKey == "lon") current->lon = value.toFloat();
      else if (_currentKey == "timezone") current->timezone = value;
      else if (_currentKey == "timezone_offset") current->timezone_offset = value.toInt();
    }

    else if (_currentParent == "current") { // Current forecast - no array index - short path
      if (_currentKey == "dt") current->dt = value.toInt();
      else if (_currentKey == "sunrise") current->sunrise = value.toInt();
      else if (_currentKey == "sunset") current->sunset = value.toInt();
      else if (_currentKey == "temp") current->temp = value.toFloat();
      else if (_currentKey == "feels_like") current->feels_like = value.toFloat();
      else if (_currentKey == "pressure") current->pressure = value.toFloat();
      else if (_currentKey == "humidity") current->humidity = value.toInt();
      else if (_currentKey == "dew_point") current->dew_point = value.toFloat();
      else if (_currentKey == "uvi") current->uvi = value.toFloat();
      else if (_currentKey == "clouds") current->clouds = value.toInt();
      else if (_currentKey == "visibility") current->visibility = value.toInt();
      else if (_currentKey == "wind_speed") current->wind_speed = value.toFloat();
      else if (_currentKey == "wind_gust") current->wind_gust = value.toFloat();
      else if (_currentKey == "wind_deg") current->wind_deg = value.toInt();

      else if (_currentKey == "id") current->id = value.toInt();
      else if (_currentKey == "main") current->main = value;
      else if (_currentKey == "description") current->description = value;
      else if (_currentKey == "icon") current->icon = value;

      else if (_currentSet == "rain") current->rain_1h = value.toFloat();
      else if (_currentSet == "snow") current->snow_1h = value.toFloat();

    }

    else if (_currentParent == "hourly") { // Hourly forecast
      if (_arrayIndex >= MAX_HOURS) return;
      
      if (_currentKey == "dt") hourly->dt[_arrayIndex] = value.toInt();
      else if (_currentKey == "temp") hourly->temp[_arrayIndex] = value.toFloat();
      else if (_currentKey == "feels_like") hourly->feels_like[_arrayIndex] = value.toFloat();
      else if (_currentKey == "pressure") hourly->pressure[_arrayIndex] = value.toInt();
      else if (_currentKey == "humidity") hourly->humidity[_arrayIndex] = value.toInt();
      else if (_currentKey == "dew_point") hourly->dew_point[_arrayIndex] = value.toFloat();
      else if (_currentKey == "uvi") hourly->uvi[_arrayIndex] = value.toFloat();
      else if (_currentKey == "clouds") hourly->clouds[_arrayIndex] = value.toInt();
      else if (_currentKey == "visibility") hourly->visibility[_arrayIndex] = value.toInt();
      else if (_currentKey == "wind_speed") hourly->wind_speed[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_gust") hourly->wind_gust[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_deg") hourly->wind_deg[_arrayIndex] = value.toInt();
      else if (_currentKey == "pop") hourly->pop[_arrayIndex] = value.toFloat();

      else if (_currentSet == "rain") hourly->rain[_arrayIndex] = value.toFloat();
      else if (_currentSet == "snow") hourly->snow[_arrayIndex] = value.toFloat();

      else if (_currentSet == "weather") {
        if (_currentKey == "id") hourly->id[_arrayIndex] = value.toInt();
        else if (_currentKey == "main") hourly->main[_arrayIndex] = value;
        else if (_currentKey == "description") hourly->description[_arrayIndex] = value;
        else if (_currentKey == "icon") hourly->icon[_arrayIndex] = value;
      }
    }

    else if (_currentParent == "daily") { // Daily forecast
      if (_arrayIndex >= MAX_DAYS) return;

      // There are some bugs with _currentSet, e.g. dt has _currentSet=="daily" or _currentSet=="uvi",
      // that's why you should be careful when you add keys by yourself. Use next line to debug:
      // Serial.print("_currentKey="); Serial.print(_currentKey); Serial.print(", _currentSet="); Serial.println(_currentSet);

      if (_currentKey == "dt") daily->dt[_arrayIndex] = value.toInt();
      else if (_currentKey == "sunrise") daily->sunrise[_arrayIndex] = value.toInt();
      else if (_currentKey == "sunset") daily->sunset[_arrayIndex] = value.toInt();
      else if (_currentKey == "moonrise") daily->moonrise[_arrayIndex] = value.toInt();
      else if (_currentKey == "moonset") daily->moonset[_arrayIndex] = value.toInt();
      else if (_currentKey == "moon_phase") daily->moon_phase[_arrayIndex] = value.toFloat();

      else if (_currentKey == "pressure") daily->pressure[_arrayIndex] = value.toInt();
      else if (_currentKey == "humidity") daily->humidity[_arrayIndex] = value.toInt();
      else if (_currentKey == "dew_point") daily->dew_point[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_speed") daily->wind_speed[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_gust") daily->wind_gust[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_deg") daily->wind_deg[_arrayIndex] = value.toInt();
      else if (_currentKey == "rain") daily->rain[_arrayIndex] = value.toFloat();
      else if (_currentKey == "snow") daily->snow[_arrayIndex] = value.toFloat();
      else if (_currentKey == "clouds") daily->clouds[_arrayIndex] = value.toInt();
      else if (_currentKey == "pop") daily->pop[_arrayIndex] = value.toFloat();
      else if (_currentKey == "uvi") daily->uvi[_arrayIndex] = value.toFloat();

      else if (_currentSet == "temp") {
        if (_currentKey == "morn") daily->temp_morn[_arrayIndex] = value.toFloat();
        else if (_currentKey == "day") daily->temp_day[_arrayIndex] = value.toFloat();
        else if (_currentKey == "eve") daily->temp_eve[_arrayIndex] = value.toFloat();
        else if (_currentKey == "night") daily->temp_night[_arrayIndex] = value.toFloat();
        else if (_currentKey == "min") daily->temp_min[_arrayIndex] = value.toFloat();
        else if (_currentKey == "max") daily->temp_max[_arrayIndex] = value.toFloat();
      }

      else if (_currentSet == "feels_like") {
        if (_currentKey == "morn") daily->feels_like_morn[_arrayIndex] = value.toFloat();
        else if (_currentKey == "day") daily->feels_like_day[_arrayIndex] = value.toFloat();
        else if (_currentKey == "eve") daily->feels_like_eve[_arrayIndex] = value.toFloat();
        else if (_currentKey == "night") daily->feels_like_night[_arrayIndex] = value.toFloat();
      }

      else if (_currentSet == "weather") {
        if (_currentKey == "id") daily->id[_arrayIndex] = value.toInt();
        else if (_currentKey == "main") daily->main[_arrayIndex] = value;
        else if (_currentKey == "description") daily->description[_arrayIndex] = value;
        else if (_currentKey == "icon") daily->icon[_arrayIndex] = value;
      }
    }
  }
  else if (_apiId == 2) { // Current
    if (_currentParent == "") { // Start of JSON
      if (_currentKey == "dt") current->dt = value.toInt();
      else if (_currentKey == "timezone") current->timezone_offset = value.toInt();
      else if (_currentKey == "id") current->city_id = value.toInt();
      else if (_currentKey == "name") current->city_name = value;
      else if (_currentKey == "visibility") current->visibility = value.toInt();
    }
    else if (_currentParent == "coord") {
      if (_currentKey == "lat") current->lat = value.toFloat();
      else if (_currentKey == "lon") current->lon = value.toFloat();
    }
    else if (_currentParent == "weather") {
      if (_currentKey == "id") current->id = value.toInt();
      else if (_currentKey == "main") current->main = value;
      else if (_currentKey == "description") current->description = value;
      else if (_currentKey == "icon") current->icon = value;
    }
    else if (_currentParent == "main") {
      if (_currentKey == "temp") current->temp = value.toFloat();
      else if (_currentKey == "feels_like") current->feels_like = value.toFloat();
      else if (_currentKey == "pressure") current->pressure = value.toFloat();
      else if (_currentKey == "humidity") current->humidity = value.toInt();
      else if (_currentKey == "temp_min") current->temp_min = value.toFloat();
      else if (_currentKey == "temp_max") current->temp_max = value.toFloat();
      else if (_currentKey == "sea_level") current->sea_level = value.toInt();
      else if (_currentKey == "grnd_level") current->grnd_level = value.toInt();
    }
    else if (_currentParent == "wind") {
      if (_currentKey == "speed") current->wind_speed = value.toFloat();
      else if (_currentKey == "deg") current->wind_deg = value.toInt();
      else if (_currentKey == "wind_gust") current->wind_gust = value.toFloat();
    }
    else if (_currentParent == "clouds") {
      if (_currentKey == "all") current->clouds = value.toInt();
    }
    else if (_currentParent == "rain") {
      if (_currentKey == "1h") current->rain_1h = value.toFloat();
      else if (_currentKey == "3h") current->rain_3h = value.toFloat();
    }
    else if (_currentParent == "snow") {
      if (_currentKey == "1h") current->snow_1h = value.toFloat();
      else if (_currentKey == "3h") current->snow_3h = value.toFloat();
    }
    else if (_currentParent == "sys") {
      if (_currentKey == "sunrise") current->sunrise = value.toInt();
      else if (_currentKey == "sunset") current->sunset = value.toInt();
      else if (_currentKey == "country") current->country = value;
    }
  }
}