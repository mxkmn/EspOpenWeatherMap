// The streaming parser is not the Arduino IDE library manager default, but this one slightly different and renamed to avoid conflicts: https://github.com/Bodmer/JSON_Decoder
#include <JSON_Listener.h>
#include <JSON_Decoder.h>

#include "OWM_for_ESP.h"


const char* HOST = "api.openweathermap.org";

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


#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #ifdef SECURE_CONNECTION
    #include <sntp.h> // It's used to set time from web

    // All BearSSL:: calls should be outside curly brackets, if it's possible. There may be problems inside brackets, such as a random crash
    BearSSL::WiFiClientSecure client;
    BearSSL::X509List cert(ROOT_CA);
    #define PORT 443
  #else
    WiFiClient client;
    #define PORT 80
  #endif
#elif defined(ESP32)
  #ifdef SECURE_CONNECTION
    #include <WiFiClientSecure.h>
    WiFiClientSecure client;
    #define PORT 443
  #else
    #include <WiFi.h>
    WiFiClient client;
    #define PORT 80
  #endif
#endif



OWM_Weather::OWM_Weather(String apiKey, uint8_t units) {
  // Setting internal variables
  _apiKey = apiKey;
  _units = units;

  // Connecting the certificate with WiFiClient library
#ifdef SECURE_CONNECTION
 #ifdef ESP8266
  client.setTrustAnchors(&cert);
 #else // ESP32
  client.setCACert(ROOT_CA);
 #endif
#endif
}



bool OWM_Weather::getCurrentWeather(OWM_current *current, String latitude, String longitude) {
  return getWeather(current, nullptr, nullptr, latitude, longitude, CURRENT);
}

bool OWM_Weather::getFullWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily, String latitude, String longitude) {
  return getWeather(current, hourly, daily, latitude, longitude, FULL);
}

bool OWM_Weather::getWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily, String latitude, String longitude, uint8_t type) {
  // Adding a pointer to structures to local variables to be handled by the parser
  this->current = current;
  this->hourly  = hourly;
  this->daily   = daily;

  // Creating an URL and adding connection type. "s" isn't necessary, but it's more readable on logging
 #ifdef SECURE_CONNECTION
  String url = "https://";
 #else
  String url = "http://";
 #endif

  // Adding API type to URL
  if (type == FULL) url += "api.openweathermap.org/data/2.5/onecall";
  else if (type == CURRENT) url += "api.openweathermap.org/data/2.5/weather";
  else {
    printlnError("API type is incorrect or unavailable. Set CURRENT or FULL.");
    return false;
  }
  _type = type;

  // Adding our location
  url += "?lat=" + latitude + "&lon=" + longitude;

  // Excluding useless data to speed up processing; pass a nullptr for current, hourly or daily pointers to exclude in response
  if (type == FULL) { // One Call API
    url += "&exclude=minutely,alerts";
    if (!current) url += ",current";
    if (!hourly)  url += ",hourly";
    if (!daily)   url += ",daily";
  }

  // Adding the remaining data: units (if it's not STANDART, because it's default units for OWM) and API key
  if (_units == METRIC) url += "&units=metric";
  else if (_units == IMPERIAL) url += "&units=imperial";
  else if (_units != STANDART) {
    printlnError("Units type is incorrect or unavailable. Set STANDART, METRIC or IMPERIAL.");
    return false;
  }
  url += "&appid=" + _apiKey; 

  // Sending GET request and feeding the parser
  bool result = getAndParseData(url);

  // Removing pointers from local variables to prevent crashes
  this->current = nullptr;
  this->hourly  = nullptr;
  this->daily   = nullptr;

  return result;
}



bool OWM_Weather::getAndParseData(String url) {
  uint32_t startTimer = millis(), actionTimer = millis();

 #if (defined SECURE_CONNECTION) && (defined ESP8266)
  if (time(nullptr) < 1000000) { // Setting current time for HTTPS connection
    printUnnecessary("Setting time using SNTP... ");

    uint8_t tickCounter = 0;
    while (time(nullptr) < 1000000) {
      if (tickCounter % 20 == 0) { // Reconnecting in every 4 seconds to prevent the bug with the long acquisition of time
        sntp_stop();
        sntp_setservername(0, "time.nist.gov");
        sntp_init();
      }
      else if (tickCounter == 99) { // If the time is not received in 20 seconds, returns an error
        printlnError("Error: getting time failed.");
        return false;
      }

      delay(200);
      tickCounter++;
    }

    printTime("Time received in " + String(millis() - actionTimer) + " ms. "); actionTimer = millis();
  }
 #endif

  printUnnecessary("Connecting to " + url + "\n");

  if (!client.connect(HOST, PORT)) {
    printlnError("Error: connection failed.");
    client.stop();
    return false;
  }
  printTime("Connected in " + String(millis() - actionTimer) + " ms. "); actionTimer = millis();

  // Sending GET request
  printUnnecessary("Sending GET request... ");
  client.print("GET " + url + " HTTP/1.1\r\n" + "Host: " + HOST + "\r\n" + "Connection: close\r\n\r\n");

  while (client.connected() or client.available()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      printlnHeader(line);
      if (line == "\r") {
        printUnnecessary("HTTP header end found. ");
        break;
      }
    }

    if ((millis() - actionTimer) > 2500) {
      printlnError("Error: HTTP header retrieval time exceeded.");
      client.stop();
      return false;
    }

    yield();
  }
  printTime("Got header in " + String(millis() - actionTimer) + " ms. "); actionTimer = millis();

  // Parsing the JSON data
  _parseOk = false;
  JSON_Decoder parser;
  parser.setListener(this);

  printUnnecessary("Parsing JSON. ");
  printJsonChar('\n');
  while (client.connected() or client.available()) {
    if (client.available()) {
      char c = client.read();
      parser.parse(c);
      printJsonChar(c);
    }

    if ((millis() - actionTimer) > 5000) {
      printlnError("Error: JSON parsing time exceeded.");
      parser.reset();
      client.stop();
      return false;
    }

    yield();
  }
  printJsonChar('\n');

  // Checking JSON completion
  if (_objectLevel != 0) printlnError("Error: JSON end wasn't received.");
  else {
    printTime("JSON receieved and parsed in " + String(millis() - actionTimer) + " ms. ");
    printTime("Weather receieved in " + String(millis() - startTimer) + " ms. ");
   #if not defined(LOG_TIME) and defined(LOG_UNNECESSARY_INFO)
    printUnnecessary("Weather receieved!\n");
   #endif
  }

  parser.reset();
  client.stop();
  return _parseOk;
}


void OWM_Weather::setApiKey(String apiKey) {
  _apiKey = apiKey;
}

void OWM_Weather::setUnits(uint8_t units) {
  _units = units;
}


String OWM_Weather::getIconCode(uint8_t iconId, String begin, String end) {
  String out = begin;
  switch (iconId) {
    case 10: { out += "01n"; break; }
    case 11: { out += "01d"; break; }
    case 12: { out += "02n"; break; }
    case 13: { out += "02d"; break; }
    case 14: { out += "03n"; break; }
    case 15: { out += "03d"; break; }
    case 16: { out += "04n"; break; }
    case 17: { out += "04d"; break; }
    case 18: { out += "09n"; break; }
    case 19: { out += "09d"; break; }
    case 20: { out += "10n"; break; }
    case 21: { out += "10d"; break; }
    case 22: { out += "11n"; break; }
    case 23: { out += "11d"; break; }
    case 24: { out += "13n"; break; }
    case 25: { out += "13d"; break; }
    case 26: { out += "50n"; break; }
    case 27: { out += "50d"; break; }
    default: return("Unknown");
  }
  out += end;
  return(out);
}

uint8_t OWM_Weather::getIconId(String iconCode) { // https://openweathermap.org/weather-conditions#Icon-list
  if      (iconCode == "01n") return(10);
  else if (iconCode == "01d") return(11);
  else if (iconCode == "02n") return(12);
  else if (iconCode == "02d") return(13);
  else if (iconCode == "03n") return(14);
  else if (iconCode == "03d") return(15);
  else if (iconCode == "04n") return(16);
  else if (iconCode == "04d") return(17);
  else if (iconCode == "09n") return(18);
  else if (iconCode == "09d") return(19);
  else if (iconCode == "10n") return(20);
  else if (iconCode == "10d") return(21);
  else if (iconCode == "11n") return(22);
  else if (iconCode == "11d") return(23);
  else if (iconCode == "13n") return(24);
  else if (iconCode == "13d") return(25);
  else if (iconCode == "50n") return(26);
  else if (iconCode == "50d") return(27);
  else return(0);
}


void OWM_Weather::value(const char *val) {
  String value = val;
  if (_type == FULL) { // One Call API
    if (_currentParent == "") { // Start of JSON
      if      (_currentKey == "lat") current->lat = value.toFloat();
      else if (_currentKey == "lon") current->lon = value.toFloat();
      else if (_currentKey == "timezone_offset") current->timezoneOffset = value.toInt();
     #ifdef ENABLE_STRINGS
      else if (_currentKey == "timezone") current->timezoneName = value;
     #endif
    }

    else if (_currentParent == "current") { // Current forecast - no array index - short path
      if      (_currentKey == "dt") current->serverUpdateTime = value.toInt();
      else if (_currentKey == "sunrise") current->sunriseTime = value.toInt();
      else if (_currentKey == "sunset") current->sunsetTime = value.toInt();
      else if (_currentKey == "temp") current->mainTemp = value.toFloat();
      else if (_currentKey == "feels_like") current->feelsLikeTemp = value.toFloat();
      else if (_currentKey == "pressure") current->pressureSeaLevel = value.toFloat();
      else if (_currentKey == "humidity") current->humidity = value.toInt();
      else if (_currentKey == "dew_point") current->dewPoint = value.toFloat();
      else if (_currentKey == "uvi") current->ultravioletIndex = value.toFloat();
      else if (_currentKey == "clouds") current->cloudiness = value.toInt();
      else if (_currentKey == "visibility") current->visibility = value.toInt();
      else if (_currentKey == "wind_speed") current->windSpeed = value.toFloat();
      else if (_currentKey == "wind_gust") current->windGust = value.toFloat();
      else if (_currentKey == "wind_deg") current->windDirection = value.toInt();

      else if (_currentKey == "id") current->weatherCondId = value.toInt();
      else if (_currentKey == "icon") current->weatherIconId = getIconId(value);

      else if (_currentSet == "rain") current->rainForLast1h = value.toFloat();
      else if (_currentSet == "snow") current->snowForLast1h = value.toFloat();
    }

    else if (_currentParent == "hourly") { // Hourly forecast
      if (_arrayIndex >= MAX_HOURS) return;
      
      if      (_currentKey == "dt") hourly->time[_arrayIndex] = value.toInt();
      else if (_currentKey == "temp") hourly->mainTemp[_arrayIndex] = value.toFloat();
      else if (_currentKey == "feels_like") hourly->feelsLikeTemp[_arrayIndex] = value.toFloat();
      else if (_currentKey == "pressure") hourly->pressureSeaLevel[_arrayIndex] = value.toInt();
      else if (_currentKey == "humidity") hourly->humidity[_arrayIndex] = value.toInt();
      else if (_currentKey == "dew_point") hourly->dewPoint[_arrayIndex] = value.toFloat();
      else if (_currentKey == "uvi") hourly->ultravioletIndex[_arrayIndex] = value.toFloat();
      else if (_currentKey == "clouds") hourly->cloudiness[_arrayIndex] = value.toInt();
      else if (_currentKey == "visibility") hourly->visibility[_arrayIndex] = value.toInt();
      else if (_currentKey == "wind_speed") hourly->windSpeed[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_gust") hourly->windGust[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_deg") hourly->windDirection[_arrayIndex] = value.toInt();
      else if (_currentKey == "pop") hourly->pop[_arrayIndex] = value.toFloat();

      else if (_currentSet == "rain") hourly->rainForLast1h[_arrayIndex] = value.toFloat();
      else if (_currentSet == "snow") hourly->snowForLast1h[_arrayIndex] = value.toFloat();

      else if (_currentSet == "weather") {
        if      (_currentKey == "id") hourly->weatherCondId[_arrayIndex] = value.toInt();
        else if (_currentKey == "icon") hourly->weatherIconId[_arrayIndex] = getIconId(value);
      }
    }

    else if (_currentParent == "daily") { // Daily forecast
      if (_arrayIndex >= MAX_DAYS) return;

      // There are some bugs with _currentSet, e.g. dt has _currentSet=="daily" or _currentSet=="uvi",
      // that's why you should be careful when you add keys by yourself. Use next line to debug:
      // Serial.print("_currentKey="); Serial.print(_currentKey); Serial.print(", _currentSet="); Serial.println(_currentSet);

      if      (_currentKey == "dt") daily->time[_arrayIndex] = value.toInt();
      else if (_currentKey == "sunrise") daily->sunriseTime[_arrayIndex] = value.toInt();
      else if (_currentKey == "sunset") daily->sunsetTime[_arrayIndex] = value.toInt();
      else if (_currentKey == "moonrise") daily->moonriseTime[_arrayIndex] = value.toInt();
      else if (_currentKey == "moonset") daily->moonsetTime[_arrayIndex] = value.toInt();
      else if (_currentKey == "moon_phase") daily->moonPhase[_arrayIndex] = value.toFloat();

      else if (_currentKey == "pressure") daily->pressureSeaLevel[_arrayIndex] = value.toInt();
      else if (_currentKey == "humidity") daily->humidity[_arrayIndex] = value.toInt();
      else if (_currentKey == "dew_point") daily->dewPoint[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_speed") daily->windSpeed[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_gust") daily->windGust[_arrayIndex] = value.toFloat();
      else if (_currentKey == "wind_deg") daily->windDirection[_arrayIndex] = value.toInt();
      else if (_currentKey == "rain") daily->rain[_arrayIndex] = value.toFloat();
      else if (_currentKey == "snow") daily->snow[_arrayIndex] = value.toFloat();
      else if (_currentKey == "clouds") daily->cloudiness[_arrayIndex] = value.toInt();
      else if (_currentKey == "pop") daily->pop[_arrayIndex] = value.toFloat();
      else if (_currentKey == "uvi") daily->ultravioletIndex[_arrayIndex] = value.toFloat();

      else if (_currentSet == "temp") {
        if      (_currentKey == "morn") daily->morningTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "day") daily->dayTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "eve") daily->eveningTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "night") daily->nightTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "min") daily->minTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "max") daily->maxTemp[_arrayIndex] = value.toFloat();
      }

      else if (_currentSet == "feels_like") {
        if      (_currentKey == "morn") daily->morningFeelsLikeTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "day") daily->dayFeelsLikeTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "eve") daily->eveningFeelsLikeTemp[_arrayIndex] = value.toFloat();
        else if (_currentKey == "night") daily->nightFeelsLikeTemp[_arrayIndex] = value.toFloat();
      }

      else if (_currentSet == "weather") {
        if      (_currentKey == "id") daily->weatherCondId[_arrayIndex] = value.toInt();
        else if (_currentKey == "icon") daily->weatherIconId[_arrayIndex] = getIconId(value);
      }
    }
  }
  else if (_type == CURRENT) { // Current API
    if (_currentParent == "") { // Start of JSON
      if (_currentKey == "dt") current->serverUpdateTime = value.toInt();
      else if (_currentKey == "timezone") current->timezoneOffset = value.toInt();
      else if (_currentKey == "id") current->cityId = value.toInt();
     #ifdef ENABLE_STRINGS
      else if (_currentKey == "name") current->cityName = value;
     #endif
      else if (_currentKey == "visibility") current->visibility = value.toInt();
    }
    else if (_currentParent == "coord") {
      if (_currentKey == "lat") current->lat = value.toFloat();
      else if (_currentKey == "lon") current->lon = value.toFloat();
    }
    else if (_currentParent == "weather") {
      if (_currentKey == "id") current->weatherCondId = value.toInt();
      else if (_currentKey == "icon") current->weatherIconId = getIconId(value);
    }
    else if (_currentParent == "main") {
      if (_currentKey == "temp") current->mainTemp = value.toFloat();
      else if (_currentKey == "feels_like") current->feelsLikeTemp = value.toFloat();
      else if (_currentKey == "pressure") current->pressureSeaLevel = value.toFloat();
      else if (_currentKey == "humidity") current->humidity = value.toInt();
      else if (_currentKey == "temp_min") current->minTemp = value.toFloat();
      else if (_currentKey == "temp_max") current->maxTemp = value.toFloat();
      else if (_currentKey == "sea_level") current->pressureSeaLevel = value.toInt();
      else if (_currentKey == "grnd_level") current->pressureGroundLevel = value.toInt();
    }
    else if (_currentParent == "wind") {
      if (_currentKey == "speed") current->windSpeed = value.toFloat();
      else if (_currentKey == "deg") current->windDirection = value.toInt();
      else if (_currentKey == "gust") current->windGust = value.toFloat();
    }
    else if (_currentParent == "clouds") {
      if (_currentKey == "all") current->cloudiness = value.toInt();
    }
    else if (_currentParent == "rain") {
      if (_currentKey == "1h") current->rainForLast1h = value.toFloat();
      else if (_currentKey == "3h") current->rainForLast3h = value.toFloat();
    }
    else if (_currentParent == "snow") {
      if (_currentKey == "1h") current->snowForLast1h = value.toFloat();
      else if (_currentKey == "3h") current->snowForLast3h = value.toFloat();
    }
    else if (_currentParent == "sys") {
      if (_currentKey == "sunrise") current->sunriseTime = value.toInt();
      else if (_currentKey == "sunset") current->sunsetTime = value.toInt();
     #ifdef ENABLE_STRINGS
      else if (_currentKey == "country") current->countryCode = value;
     #endif
    }
  }
}

void OWM_Weather::key(const char *key) {
  _currentKey = key;
  printCallback("\n>>> Key >>>" + (String)key);
}

void OWM_Weather::startDocument() {
  _currentParent = "", _currentKey = "", _objectLevel = 0, _arrayIndex = 0, _arrayLevel = 0, _currentSet = "";
  printCallback("\n>>> Start document >>>");
}

void OWM_Weather::endDocument() {
  _currentParent = "", _currentKey = "", _objectLevel = 0, _arrayIndex = 0, _arrayLevel = 0, _parseOk = true;
  printCallback("\n<<< End document <<<");
}

void OWM_Weather::startObject() {
  if (_arrayIndex == 0 && _objectLevel == 1) _currentParent = _currentKey;
  _currentSet = _currentKey;
  _objectLevel++;
  printCallback("\n>>> Start object level:" + String(_objectLevel) + " array level:" + String(_arrayLevel) + " array index:" + String(_arrayIndex) +" >>>");
}

void OWM_Weather::endObject() {
  if (_arrayLevel == 0) _currentParent = "";
  if (_arrayLevel == 1 && _objectLevel == 2) _arrayIndex++;
  _objectLevel--;
  printCallback("\n<<< End object <<<");
}

void OWM_Weather::startArray() {
  _arrayLevel++;
  printCallback("\n>>> Start array /" + String(_arrayLevel) + "/" + String(_arrayIndex) +" >>>");
}

void OWM_Weather::endArray() {
  if (_arrayLevel > 0) _arrayLevel--;
  if (_arrayLevel == 0) _arrayIndex = 0;
  printCallback("\n<<< End array <<<");
}

void OWM_Weather::error(const char *message) {
  _parseOk = false;
  printlnError("\nParse error message: " + String(message));
}


void OWM_Weather::printlnError(String msg) {
 #ifdef LOG_ERRORS
  Serial.println(msg);
 #endif
}

void OWM_Weather::printTime(String msg) {
 #ifdef LOG_TIME
  Serial.print(msg);
 #endif
}

void OWM_Weather::printUnnecessary(String msg) {
 #ifdef LOG_UNNECESSARY_INFO
  Serial.print(msg);
 #endif
}

void OWM_Weather::printlnHeader(String msg) {
 #ifdef SHOW_HEADER
  Serial.println(msg);
 #endif
}

void OWM_Weather::printJsonChar(char c) {
 #ifdef SHOW_JSON
  Serial.print(c);
 #endif
}

void OWM_Weather::printCallback(String msg) {
 #ifdef SHOW_CALLBACK
  Serial.print(msg);
 #endif
}










String OWM_Weather::getShortStatus(uint16_t id) {
 #if (LANGUAGE == RU)
  if (200 <= id and id <= 232) return("Гроза");
  else if (300 <= id and id <= 321) return("Морось");
  else if (500 <= id and id <= 531) return("Дождь");
  else if (600 <= id and id <= 622) return("Снегопад");
  else if (801 <= id and id <= 804) return("Облачно");
  else if (800 == id) return("Ясно");
  else if (701 == id or 721 == id) return("Туман");
  else if (711 == id) return("Дымка");
  else if (741 == id) return("Мгла");
  else if (731 == id or 751 == id) return("Песок");
  else if (761 == id) return("Пыльно");
  else if (762 == id) return("Пепел");
  else if (771 == id) return("Шквал");
  else if (781 == id) return("Торнадо");
  else return("Неизвестно (" + String(id) + ")");
 #else // English and unsupported languages
  if (200 <= id and id <= 232) return("Thunderstorm");
  else if (300 <= id and id <= 321) return("Drizzle");
  else if (500 <= id and id <= 531) return("Rain");
  else if (600 <= id and id <= 622) return("Snow");
  else if (801 <= id and id <= 804) return("Clouds");
  else if (800 == id) return("Clear");
  else if (701 == id) return("Mist");
  else if (711 == id) return("Smoke");
  else if (721 == id) return("Haze");
  else if (731 == id) return("Dust");
  else if (741 == id) return("Fog");
  else if (751 == id) return("Sand");
  else if (761 == id) return("Dust");
  else if (762 == id) return("Ash");
  else if (771 == id) return("Squall");
  else if (781 == id) return("Tornado");
  else return("Unknown (" + String(id) + ")");
 #endif
}

String OWM_Weather::getStatus16(uint16_t id) {
 #if (LANGUAGE == EN)
  switch (id) {
    case 200:
    case 201:
    case 202: return("Thund. with rain");
    case 210: return("Light thunderst.");
    case 212: return("Heavy thunderst.");
    case 221: return("Ragged thunders.");
    case 230:
    case 231:
    case 232: return("Thun. with driz.");

    case 300: return("Light drizzle");
    case 302: return("Heavy drizzle");
    case 310: return("Light driz. rain");
    case 312: return("Heavy driz. rain");
    case 313: return("Shower rain");
    case 314: return("Heavy show. rain");

    case 502: return("Heavy rain");
    case 520: return("Light show. rain");
    case 522: return("Heavy show. rain");
    case 531: return("Ragged sh. rain");

    case 612: return("Light show. sleet");
    case 615: return("Rain and snow");
    case 620: return("Light show. snow");
    case 622: return("Heavy show. snow");

    default: return(getStatus(id));
  }
 #elif (LANGUAGE == RU)
  switch (id) {
    case 802: return("Перем.облачность");
    case 803: return("Облачно с прояс.");

    case 200: return("Неб. дождь/гроза");
    case 202: return("Сил. дождь/гроза");
    case 230: return("Неб.морось/гроза");
    case 232: return("Сил.морось/гроза");
    case 310: return("Неб. морос.дождь");
    case 312: return("Сил. морос.дождь");
    case 314: return("Ливень с моросью");

    case 504: return("Сильный ливень");

    case 600:
    case 620: return("Мелкий снегопад");
    case 612: return("Ледяная крупа");
    case 615:
    case 616: return("Снегопад и дождь");

    case 762: return("Вулканич. пепел");

    default: return(getStatus(id));
  }
 #else
  return("Your language isn't supported. Get more info in Notes.ino");
 #endif
}

String OWM_Weather::getStatus(uint16_t id) {
 #if (LANGUAGE == EN)
  switch (id) {
    case 800: return("Clear sky");
    case 801: return("Few clouds");
    case 802: return("Scattered clouds");
    case 803: return("Broken clouds");
    case 804: return("Overcast clouds");

    case 200: return("Thunderstorm with light rain");
    case 201: return("Thunderstorm with rain");
    case 202: return("Thunderstorm with heavy rain");
    case 210: return("Light thunderstorm");
    case 211: return("Thunderstorm");
    case 212: return("Heavy thunderstorm");
    case 221: return("Ragged thunderstorm");
    case 230: return("Thunderstorm with light drizzle");
    case 231: return("Thunderstorm with drizzle");
    case 232: return("Thunderstorm with heavy drizzle");

    case 300: return("Light intensity drizzle");
    case 301: return("Drizzle");
    case 302: return("Heavy intensity drizzle");
    case 310: return("Light intensity drizzle rain");
    case 311: return("Drizzle rain");
    case 312: return("Heavy intensity drizzle rain");
    case 313: return("Shower rain and drizzle");
    case 314: return("Heavy shower rain and drizzle");
    case 321: return("Shower drizzle");

    case 500: return("Light rain");
    case 501: return("Moderate rain");
    case 502: return("Heavy intensity rain");
    case 503: return("Very heavy rain");
    case 504: return("Extreme rain");
    case 511: return("Freezing rain");
    case 520: return("Light intensity shower rain");
    case 521: return("Shower rain");
    case 522: return("Heavy intensity shower rain");
    case 531: return("Ragged shower rain");

    case 600: return("Light snow");
    case 601: return("Snow");
    case 602: return("Heavy snow");
    case 611: return("Sleet");
    case 612: return("Light shower sleet");
    case 613: return("Shower sleet");
    case 615: return("Light rain and snow");
    case 616: return("Rain and snow");
    case 620: return("Light shower snow");
    case 621: return("Shower snow");
    case 622: return("Heavy shower snow");

    case 701: return("Mist");
    case 711: return("Smoke");
    case 721: return("Haze");
    case 731: return("Sand/dust whirls");
    case 741: return("Fog");
    case 751: return("Sand");
    case 761: return("Dust");
    case 762: return("Volcanic ash");
    case 771: return("Squalls");
    case 781: return("Tornado");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == RU)
  switch (id) {
    case 800: return("Ясно");
    case 801: return("Малооблачно");
    case 802: return("Переменная облачность");
    case 803: return("Облачно с прояснениями");
    case 804: return("Пасмурно");

    case 200: return("Небольшой дождь с грозой");
    case 201: return("Дождь с грозой");
    case 202: return("Сильный дождь с грозой");
    case 210: return("Редкая гроза");
    case 211: return("Гроза");
    case 212: return("Частая гроза");
    case 221: return("Местами гроза");
    case 230: return("Небольшая морось с грозой");
    case 231: return("Морось с грозой");
    case 232: return("Сильная морось с грозой");

    case 300: return("Небольшая морось");
    case 301: return("Морось");
    case 302: return("Сильная морось");
    case 310: return("Небольшой моросящий дождь");
    case 311: return("Моросящий дождь");
    case 312: return("Сильный моросящий дождь");
    case 313: return("Ливень с моросью");
    case 314: return("Сильный ливень с моросью");
    case 321: return("Мелкий дождь");

    case 500: return("Небольшой дождь");
    case 501: return("Умеренный дождь");
    case 502: return("Сильный дождь");
    case 503: return("Проливной дождь");
    case 504: return("Очень сильный проливной дождь");
    case 511: return("Ледяной дождь");
    case 520: return("Небольшой ливень");
    case 521: return("Ливень");
    case 522: return("Сильный ливень");
    case 531: return("Местами ливень");

    case 600:
    case 620: return("Небольшой снегопад");
    case 601:
    case 621: return("Снегопад");
    case 602:
    case 622: return("Сильный снегопад");
    case 611: return("Ледяная крупа");
    case 612: return("Интенсивная ледяная крупа");
    case 613: return("Мокрый снег");
    case 615: return("Небольшой снегопад с дождём");
    case 616: return("Снегопад с дождём");

    case 701: return("Туман");
    case 711: return("Дымка");
    case 721: return("Лёгкий туман");
    case 731: return("Песчаная буря");
    case 741: return("Плотный туман");
    case 751: return("Песчаный ветер");
    case 761: return("Пыльная буря");
    case 762: return("Вулканический пепел");
    case 771: return("Шквальный ветер");
    case 781: return("Торнадо");
    default:  return("Неизвестно (" + String(id) + ")");
  }





 #elif (LANGUAGE == AF)
  switch (id) {
    case 200: return("Donderstorm met ligte reën");
    case 201: return("Donderstorm met reën");
    case 202: return("Donderstorm met swaar reën");
    case 210: return("Ligte donderstorm");
    case 211: return("Donderstorm");
    case 212: return("Swaar donderstorm");
    case 221: return("Donderstorms plek-plek");
    case 230: return("Donderstorm met sagte motreën");
    case 231: return("Donderstorm met motreën");
    case 232: return("Donderstorm met digte motreën");
    case 300: return("Sagte motreën");
    case 301: return("Motreën");
    case 302: return("Digte motreën");
    case 310: return("Sagte motreën tot reën");
    case 311: return("Motreën tot reën");
    case 312: return("Digte motreën tot reën");
    case 313: return("Reënbuie en motreën");
    case 314: return("Swaar reënbuie en motreën");
    case 321: return("Motreënbuie");
    case 500: return("Ligte reën");
    case 501: return("Matige reën");
    case 502: return("Swaar reën");
    case 503: return("Baie swaar reën");
    case 504: return("Ekstreme swaar reën");
    case 511: return("Vriesreën");
    case 520: return("Ligte reënbuie");
    case 521: return("Reënbuie");
    case 522: return("Swaar reënbuie");
    case 531: return("Reënbuie plek-plek");
    case 600: return("Ligte sneeu");
    case 601: return("Sneeu");
    case 602: return("Swaar sneeu");
    case 611: return("Sneeureën");
    case 612: return("Sneeureënbui");
    case 615: return("Ligte reën en sneeu");
    case 616: return("Reën en sneeu");
    case 620: return("Ligte sneeubui");
    case 621: return("Sneeubui");
    case 622: return("Swaar sneeubui");
    case 701: return("Newel");
    case 711: return("Rook");
    case 721: return("Deinserigheid");
    case 731: return("Sand, dwarrelwinde");
    case 741: return("Mis");
    case 751: return("Sand");
    case 761: return("Stof");
    case 762: return("Vulkaniese as");
    case 771: return("Rukwinde");
    case 781: return("Tornado");
    case 800: return("Helder lug");
    case 801: return("Enkele wolke");
    case 802: return("Verstrooide wolke");
    case 803: return("Los wolke");
    case 804: return("Bewolk");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == AR)
  switch (id) {
    case 200: return("عاصفة رعدية مع أمطار خفيفة");
    case 201: return("عاصفة رعدية مع مطر");
    case 202: return("عاصفة رعدية مع أمطار غزيرة");
    case 210: return("عاصفة رعدية خفيفة");
    case 211: return("عاصفة رعدية");
    case 212: return("عاصفة رعدية قوية");
    case 221: return("عاصفة رعدية خشنة");
    case 230: return("عاصفة رعدية مع رذاذ خفيف");
    case 231: return("عاصفة رعدية مع رذاذ");
    case 232: return("عاصفة رعدية مع رذاذ قوي");
    case 300: return("رذاذ خفيف الكثافة");
    case 301: return("رذاذ");
    case 302: return("رذاذ شديد الكثافة");
    case 310: return("رذاذ مطر خفيف الكثافة");
    case 311: return("رذاذ مطر");
    case 312: return("رذاذ مطر شديد الكثافة");
    case 313: return("رذاذ المطر");
    case 314: return("رذاذ مطر قوي");
    case 321: return("وابل مطر");
    case 500: return("مطر خفيف");
    case 501: return("مطر متوسط الغزارة");
    case 502: return("مطر غزير");
    case 503: return("مطر شديد الغزارة");
    case 504: return("مطر غزير جدا");
    case 511: return("مطر بارد");
    case 520: return("وابل مطر خفيف");
    case 521: return("وابل مطر");
    case 522: return("وابل مطر شديد الكثافة");
    case 531: return("رذاذ مطر كثيف");
    case 600: return("ثلوج خفيفة");
    case 601: return("ثلوج");
    case 602: return("ثلوج قوية");
    case 611: return("صقيع");
    case 612: return("رذاذ من الصقيع");
    case 615: return("مطر خفيف مع ثلوج");
    case 616: return("مطر مع ثلوج");
    case 620: return("رذاذ ثلوج خفيف");
    case 621: return("وابل ثلوج");
    case 622: return("رذاذ ثلوج كثيف");
    case 701: return("ضباب");
    case 711: return("دخان");
    case 721: return("غبار");
    case 731: return("غبار");
    case 741: return("ضباب");
    case 751: return("رمال");
    case 761: return("غبار");
    case 762: return("رماد بركاني");
    case 771: return("سكوالس");
    case 781: return("إعصار");
    case 800: return("سماء صافية");
    case 801: return("غائم جزئي");
    case 802: return("غيوم متفرقة");
    case 803: return("غيوم متناثرة");
    case 804: return("غيوم قاتمة");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == AZ)
  switch (id) {
    case 200: return("Şimşək, yağış");
    case 201: return("Şimşək");
    case 202: return("Şimşək, leysan");
    case 210: return("Zəif şimşək");
    case 211: return("Ildırım");
    case 212: return("Güclü şimşək");
    case 221: return("Dağınıq leysan");
    case 230: return("Şimşək, çiskin");
    case 231: return("Şimşək, çiskinli leysan");
    case 232: return("Şimşək, güclü leysan");
    case 300: return("Yüngül çovğun");
    case 301: return("Çiskin");
    case 302: return("Güclü intensiv çiskin");
    case 310: return("Yüngül intensivli çiskinli yağış");
    case 311: return("Çiskinli yağış");
    case 312: return("Güclü intensivli çiskinli yağış");
    case 313: return("Leysan yağışı və çiskin");
    case 314: return("Güclü leysan yağışı və çiskin");
    case 321: return("Leysan yağışı");
    case 500: return("Yüngül yağış");
    case 501 return(": mülayim yağış");
    case 502: return("Şiddətli yağış");
    case 503: return("Çox şiddətli yağış");
    case 504: return("Ekstremal yağış");
    case 511: return("Dondurucu yağış");
    case 520: return("Yüngül intensivli leysan yağışı");
    case 521: return("Leysan yağışı");
    case 522: return("Şiddətli intensiv leysan yağışı");
    case 531: return("Sıx leysan yağışı");
    case 600: return("Yüngül qar");
    case 601: return("Qar");
    case 602: return("Güclü qar");
    case 611: return("Sulu qar");
    case 612: return("Leysan sulu qar");
    case 615: return("Yüngül yağış və qar");
    case 616: return("Yağış və qar");
    case 620: return("Yüngül sulu qar");
    case 621: return("Sulu qar");
    case 622: return("Güclü sulu qar");
    case 701: return("Duman");
    case 711: return("Çisgin");
    case 721: return("Çən");
    case 731: return("Qum, toz fırtınaları");
    case 741: return("Duman");
    case 751: return("Qum");
    case 761: return("Toz");
    case 762: return("Vulkanik kül");
    case 771: return("Səslər");
    case 781: return("Tornado");
    case 800: return("Aydın səma");
    case 801: return("Az buludlu");
    case 802: return("Dağınıq buludlu");
    case 803: return("Az buludlu");
    case 804: return("Qapalı buludlu");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == BG)
  switch (id) {
    case 200: return("Гръмотевична буря със слаб дъжд");
    case 201: return("Гръмотевична буря с валеж");
    case 202: return("Гръмотевична буря с порой");
    case 210: return("Слаба гръмотевична буря");
    case 211: return("Гръмотевична буря");
    case 212: return("Силна гръмотевична буря");
    case 221: return("Гръмотевична буря с прекъсвания");
    case 230: return("Гръмотевична буря със слаб ръмеж");
    case 231: return("Гръмотевична буря с ръмеж");
    case 232: return("Гръмотевична буря със силен ръмеж");
    case 300: return("Слаб ръмеж");
    case 301: return("Ръмеж");
    case 302: return("Силен ръмеж");
    case 310: return("Слаб ръмеж и дъжд");
    case 311: return("Ръмеж и дъжд");
    case 312: return("Силен ръмеж и дъжд");
    case 313: return("Проливен ръмеж и дъжд");
    case 314: return("Пороен ръмеж и дъжд");
    case 321: return("Силен ръмеж");
    case 500: return("Слаб дъжд");
    case 501: return("Умерен дъжд");
    case 502: return("Интензивен дъжд");
    case 503: return("Много силен дъжд");
    case 504: return("Пороен дъжд");
    case 511: return("Леден дъжд");
    case 520: return("Кратковременен пороен дъжд");
    case 521: return("Проливен дъжд");
    case 522: return("Пороен дъжд");
    case 531: return("Пороен дъжд с прекъсвания");
    case 600: return("Слаб снеговалеж");
    case 601: return("Снеговалеж");
    case 602: return("Силен снеговалеж");
    case 611: return("Суграшица");
    case 612: return("Силна суграшица");
    case 615: return("Слаб валеж от дъжд и сняг");
    case 616: return("Дъжд и сняг");
    case 620: return("Краткотраен слаб снеговалеж");
    case 621: return("Краткотраен интензивен снеговалеж");
    case 622: return("Краткотраен обилен снеговалеж");
    case 701: return("Лека мъгла");
    case 711: return("Димка");
    case 721: return("Мараня");
    case 731: return("Пясъчна или прашна буря");
    case 741: return("Мъгла");
    case 751: return("Пясък");
    case 761: return("Прах");
    case 762: return("Вулканична пепел");
    case 771: return("Шквал");
    case 781: return("Торнадо");
    case 800: return("Ясно небе");
    case 801: return("Предимно ясно");
    case 802: return("Разкъсана облачност");
    case 803: return("Предимно облачно");
    case 804: return("Облачно");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == CA)
  switch (id) {
    case 200: return("Tempesta amb pluja suau");
    case 201: return("Tempesta amb pluja");
    case 202: return("Tempesta amb pluja intensa");
    case 210: return("Tempesta suau");
    case 211: return("Tempesta");
    case 212: return("Tempesta forta");
    case 221: return("Tempesta irregular");
    case 230: return("Tempesta amb plugim suau");
    case 231: return("Tempesta amb plugin");
    case 232: return("Tempesta amb molt plugim");
    case 300: return("Plugim suau");
    case 301: return("Plugim");
    case 302: return("Plugim intens");
    case 310: return("Plugim suau");
    case 311: return("Plugim");
    case 312: return("Plugim intens");
    case 313: return("Pluja");
    case 314: return("Pluja intensa");
    case 321: return("Plugim");
    case 500: return("Pluja suau");
    case 501: return("Pluja");
    case 502: return("Pluja intensa");
    case 503: return("Pluja molt intensa");
    case 504: return("Pluja extrema");
    case 511: return("Pluja glaçada");
    case 520: return("Pluja suau");
    case 521: return("Pluja suau");
    case 522: return("Pluja intensa");
    case 531: return("Pluja irregular");
    case 600: return("Nevada suau");
    case 601: return("Nevada");
    case 602: return("Nevada intensa");
    case 611: return("Aiguaneu");
    case 612: return("Pluja d'aiguaneu");
    case 615: return("Plugim i neu");
    case 616: return("Pluja i neu");
    case 620: return("Nevada suau");
    case 621: return("Nevada");
    case 622: return("Nevada intensa");
    case 701: return("Boira");
    case 711: return("Fum");
    case 721: return("Boirina");
    case 731: return("Sorra");
    case 741: return("Boira");
    case 751: return("Sorra");
    case 761: return("Pols");
    case 762: return("Cendra volcànica");
    case 771: return("Xàfec");
    case 781: return("Tornado");
    case 800: return("Cel net");
    case 801: return("Lleugerament ennuvolat");
    case 802: return("Núvols dispersos");
    case 803: return("Nuvolositat variable");
    case 804: return("Ennuvolat");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == CZ)
  switch (id) {
    case 200: return("Bouřka se slabým deštěm");
    case 201: return("Bouřka a déšť");
    case 202: return("Bouřka se silným deštěm");
    case 210: return("Slabší bouřka");
    case 211: return("Bouřka");
    case 212: return("Silná bouřka");
    case 221: return("Bouřková přeháňka");
    case 230: return("Bouřka se slabým mrholením");
    case 231: return("Bouřka s mrholením");
    case 232: return("Bouřka se silným mrholením");
    case 300: return("Slabé mrholení");
    case 301: return("Mrholení");
    case 302: return("Silné mrholení");
    case 310: return("Slabé mrholení a déšť");
    case 311: return("Mrholení s deštěm");
    case 312: return("Silné mrholení a déšť");
    case 313: return("Mrholení a přeháňky");
    case 314: return("Mrholení a silné přeháňky");
    case 321: return("Občasné mrholení");
    case 500: return("Slabý déšť");
    case 501: return("Déšť");
    case 502: return("Prudký déšť");
    case 503: return("Přívalový déšť");
    case 504: return("Průtrž mračen");
    case 511: return("Mrznoucí déšť");
    case 520: return("Slabé přeháňky");
    case 521: return("Přeháňky");
    case 522: return("Silné přeháňky");
    case 531: return("Občasné přeháňky");
    case 600: return("Mírné sněžení");
    case 601: return("Sněžení");
    case 602: return("Husté sněžení");
    case 611: return("Zmrzlý déšť");
    case 612: return("Smíšené přeháňky");
    case 615: return("Slabý déšť se sněhem");
    case 616: return("Déšť se sněhem");
    case 620: return("Slabé sněhové přeháňky");
    case 621: return("Sněhové přeháňky");
    case 622: return("Silné sněhové přeháňky");
    case 701: return("Mlha");
    case 711: return("Kouř");
    case 721: return("Opar");
    case 731: return("Písečné či prachové víry");
    case 741: return("Hustá mlha");
    case 751: return("Písek");
    case 761: return("Prašno");
    case 762: return("Sopečný popel");
    case 771: return("Prudké bouře");
    case 781: return("Tornádo");
    case 800: return("Jasno");
    case 801: return("Skoro jasno");
    case 802: return("Polojasno");
    case 803: return("Oblačno");
    case 804: return("Zataženo");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == DA)
  switch (id) {
    case 200: return("Torden med let regn");
    case 201: return("Torden med regn");
    case 202: return("Torden med kraftig regn");
    case 210: return("Svag torden");
    case 211: return("Torden");
    case 212: return("Kraftig torden");
    case 221: return("Uregelmæssig torden");
    case 230: return("Torden med let støvregn");
    case 231: return("Torden med støvregn");
    case 232: return("Torden med kraftig støvregn");
    case 300: return("Let støvregn");
    case 301: return("Støvregn");
    case 302: return("Kraftig støvregn");
    case 310: return("Let finregn");
    case 311: return("Finregn");
    case 312: return("Kraftig finregn");
    case 313: return("Regnbyger og støvregn");
    case 314: return("Kraftige regnbyger og støvregn");
    case 321: return("Byger med støvregn");
    case 500: return("Let regn");
    case 501: return("Regn");
    case 502: return("Kraftig regn");
    case 503: return("Meget kraftig regn");
    case 504: return("Skybrud");
    case 511: return("Risiko for islag");
    case 520: return("Lette regnbyger");
    case 521: return("Regnbyger");
    case 522: return("Kraftige regnbyger");
    case 531: return("Uregelmæssige regnbyger");
    case 600: return("Let snevejr");
    case 601: return("Snevejr");
    case 602: return("Kraftigt snevejr");
    case 611: return("Slud");
    case 612: return("Sludbyger");
    case 615: return("Let regn og snevejr");
    case 616: return("Regn og snevejr");
    case 620: return("Lette snebyger");
    case 621: return("Snebyger");
    case 622: return("Kraftige snebyger");
    case 701: return("Dis");
    case 711: return("Røg");
    case 721: return("Dårlig sigtbarhed");
    case 731: return("Sand- og støvskyer");
    case 741: return("Tåge");
    case 751: return("Sand");
    case 761: return("Støv");
    case 762: return("Vulkansk aske");
    case 771: return("Rulleskyer");
    case 781: return("Tornado");
    case 800: return("Klar himmel");
    case 801: return("Få skyer");
    case 802: return("Spredte skyer");
    case 803: return("Spredt skydække");
    case 804: return("Skydække");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == DE)
  switch (id) {
    case 200: return("Gewitter mit leichtem Regen");
    case 201: return("Gewitter mit Regen");
    case 202: return("Gewitter mit starkem Regen");
    case 210: return("Leichte Gewitter");
    case 211: return("Gewitter");
    case 212: return("Schwere Gewitter");
    case 221: return("Vereinzelte Gewitter");
    case 230: return("Gewitter mit leichtem Nieselregen");
    case 231: return("Gewitter mit Nieselregen");
    case 232: return("Gewitter mit starkem Nieselregen");
    case 300: return("Leichtes Nieseln");
    case 301: return("Nieseln");
    case 302: return("Starkes Nieseln");
    case 310: return("Leichter Nieselregen");
    case 311: return("Nieselregen");
    case 312: return("Starker Nieselregen");
    case 313: return("Schauerregen und Nieselregen");
    case 314: return("Schwerer Schauerregen und Nieselregen");
    case 321: return("Nieselschauer");
    case 500: return("Leichter Regen");
    case 501: return("Mäßiger Regen");
    case 502: return("Starker Regen");
    case 503: return("Sehr starker Regen");
    case 504: return("Extremer Regen");
    case 511: return("Eisregen");
    case 520: return("Leichter Regenschauer");
    case 521: return("Regenschauer");
    case 531: return("Gelegentliche Schauer");
    case 522: return("Heftige Regenschauer");
    case 600: return("Mäßiger Schnee");
    case 601: return("Schnee");
    case 602: return("Heftiger Schneefall");
    case 611: return("Graupel");
    case 612: return("Graupelschauer");
    case 615: return("Leichter Schneeregen");
    case 620: return("Leichter Schneeschauer");
    case 621: return("Schneeschauer");
    case 622: return("Schwerer Schneeschauer");
    case 701: return("Trüb");
    case 711: return("Rauch");
    case 721: return("Dunst");
    case 731: return("Sand / Staubsturm");
    case 741: return("Nebel");
    case 751: return("Sand");
    case 761: return("Staub");
    case 762: return("Vulkanische Asche");
    case 771: return("Sturmböen");
    case 781: return("Tornado");
    case 800: return("Klarer Himmel");
    case 801: return("Ein paar Wolken");
    case 802: return("Mäßig bewölkt");
    case 803: return("Überwiegend bewölkt");
    case 804: return("Bedeckt");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == EL)
  switch (id) {
    case 200: return("Θύελλα με ασθενείς βροχοπτώσεις");
    case 201: return("Θύελλα με βροχοπτώσεις");
    case 202: return("Θύελλα με έντονες βροχοπτώσεις");
    case 210: return("Ήπια θύελλα");
    case 211: return("Θύελλα");
    case 212: return("Έντονη θύελλα");
    case 221: return("Ισχυρή θύελλα");
    case 230: return("Θύελλα με ασθενείς ψιχάλες");
    case 231: return("Θύελλα με ψιχάλες");
    case 232: return("Θύελλα με έντονες ψιχάλες");
    case 300: return("Ψιχάλες μικρής έντασης");
    case 301: return("Ψιχάλες");
    case 302: return("Ψιχάλες μεγάλης έντασης");
    case 310: return("Ψιλόβροχο μικρής έντασης");
    case 311: return("Ψιλόβροχο");
    case 312: return("Ψιλόβροχο μεγάλης έντασης");
    case 313: return("Μπόρα και ψιχάλες");
    case 314: return("Έντονη μπόρα και ψιχάλες");
    case 321: return("Έντονες ψιχάλες");
    case 500: return("Ασθενείς βροχοπτώσεις");
    case 501: return("Βροχοπτώσεις μέτριας έντασης");
    case 502: return("Βροχοπτώσεις μεγάλης έντασης");
    case 503: return("Πολύ έντονες βροχοπτώσεις");
    case 504: return("Ακραίες βροχοπτώσεις");
    case 511: return("Παγωμένη βροχή");
    case 520: return("Μπόρα μικρής έντασης");
    case 521: return("Μπόρα");
    case 522: return("Μπόρα μεγάλης έντασης");
    case 531: return("Ισχυρή μπόρα");
    case 600: return("Ήπιες χιονοπτώσεις");
    case 601: return("Χιονοπτώσεις");
    case 602: return("Έντονες χιονοπτώσεις");
    case 611: return("Χιονόνερο");
    case 612: return("Χιονόνερο μεγάλης έντασης");
    case 615: return("Ασθενείς βροχοπτώσεις και χιονοπτώσεις");
    case 616: return("Βροχοπτώσεις και χιονοπτώσεις");
    case 620: return("Ήπια χιονοθύελλα");
    case 621: return("Χιονοθύελλα");
    case 622: return("Έντονη χιονοθύελλα");
    case 701: return("Ασθενής ομίχλη");
    case 711: return("Υδρατμοί");
    case 721: return("Καταχνιά");
    case 731: return("Αμμοστρόβιλος, κονιορτοστρόβιλος");
    case 741: return("Ομίχλη");
    case 751: return("Άμμος");
    case 761: return("Σκόνη");
    case 762: return("Ηφαιστειακή τέφρα");
    case 771: return("Μπουρίνι");
    case 781: return("Ανεμοστρόβιλος");
    case 800: return("Αίθριος καιρός");
    case 801: return("Ελαφρές νεφώσεις");
    case 802: return("Σποραδικές νεφώσεις");
    case 803: return("Αραιές νεφώσεις");
    case 804: return("Αυξημένες νεφώσεις");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == EU)
  switch (id) {
    case 200: return("Trumoi-Ekaitza euri arinarekin");
    case 201: return("Trumoi-Ekaitza euriarekin");
    case 202: return("Trumoi-Ekaitza euri ugariarekin");
    case 210: return("Trumoi-Ekaitz arina");
    case 211: return("Trumoi-Ekaitza");
    case 212: return("Trumoi-Ekaitz gogorra");
    case 221: return("Trumoi-Ekaitz oso gogorra");
    case 230: return("Trumoi-Ekaitza langar arinarekin");
    case 231: return("Trumoi-Ekaitza langarrarekin");
    case 232: return("Trumoi-Ekaitza langar ugariarekin");
    case 300: return("Langar arina");
    case 301: return("Langarra");
    case 302: return("Langar ugaria");
    case 310: return("Zirimiri arina");
    case 311: return("Zirimiria");
    case 312: return("Zirimiri ugaria");
    case 313: return("Euri-Zaparrada eta langarra");
    case 314: return("Euri-Zaparrada ugaria eta langarra");
    case 321: return("Langar-Zaparrada");
    case 500: return("Euri arina");
    case 501: return("Euria");
    case 502: return("Euri ugaria");
    case 503: return("Euri oso ugaria");
    case 504: return("Euri bortitza");
    case 511: return("Euri izozkorra");
    case 520: return("Euri-Zaparrada arina");
    case 521: return("Euri-Zaparrada");
    case 522: return("Euri-Zaparrada bizia");
    case 531: return("Euri-Zaparrada bortitza");
    case 600: return("Elur arina");
    case 601: return("Elurra");
    case 602: return("Elur ugaria");
    case 611: return("Elurbustia");
    case 612: return("Elurbusti-Zaparrada");
    case 615: return("Euri arina eta elurra");
    case 616: return("Euria eta elurra");
    case 620: return("Elur-Zaparrada arina");
    case 621: return("Elur-Zaparrada");
    case 622: return("Elur-Zaparrada ugaria");
    case 701: return("Lainobera");
    case 711: return("Kea");
    case 721: return("Gandua");
    case 731: return("Hare/Hauts-Zurrunbiloak");
    case 741: return("Lainoa");
    case 751: return("Harea");
    case 761: return("Hautsa");
    case 762: return("Errrauts bolkanikoa");
    case 771: return("Zurrunbiloak");
    case 781: return("Tornadoa");
    case 800: return("Oskarbi");
    case 801: return("Hodei gutxi");
    case 802: return("Hodei sakabanatuak");
    case 803: return("Hodeiak eta ostarteak");
    case 804: return("Zeru estalia");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == FA)
  switch (id) {
    case 200: return("بارش خفیف باران همراه با رعد و برق");
    case 201: return("بارش باران همراه با رعد و برق");
    case 202: return("بارش سنگین باران همراه با رعد و برق");
    case 210: return("رعد و برق خفیف");
    case 211: return("رعد و برق");
    case 212: return("رعد و برق سنگین");
    case 221: return("رعد و برق پراکنده");
    case 230: return("نم نم خفیف باران همراه با رعد و برق");
    case 231: return("نم نم باران همراه با رعد و برق");
    case 232: return("نم نم سنگین باران همراه با رعد و برق");
    case 300: return("نم نم خفیف باران");
    case 301: return("نم نم باران");
    case 302: return("نم نم سنگین باران");
    case 310: return("نم نم باران با شدت کم");
    case 311: return("نم نم باران");
    case 312: return("نم نم باران با شدت زیاد");
    case 313: return("نم نم باران و باران شدید");
    case 314: return("نم نم باران و باران بسیار سنگین");
    case 321: return("نم نم باران");
    case 500: return("بارش خفیف باران");
    case 501: return("بارش باران");
    case 502: return("بارش خیلی شدید باران");
    case 503: return("بارش بسیار سنگین باران");
    case 504: return("بارش باران شدید");
    case 511: return("بارش تگرگ");
    case 520: return("بارش شدید باران");
    case 521: return("بارش باران");
    case 522: return("رگبار شدید باران");
    case 531: return("رگبار پراکنده باران");
    case 600: return("بارش خفیف برف");
    case 601: return("برف");
    case 602: return("بارش سنگین برف");
    case 611: return("بوران");
    case 612: return("بارش برف و باران");
    case 615: return("بارش خفیف باران و برف");
    case 616: return("بارش باران و برف");
    case 620: return("بارش خفیف برف");
    case 621: return("بارش برف");
    case 622: return("بارش سنگین برف");
    case 701: return("مه و غبار");
    case 711: return("دودی");
    case 721: return("ریز گرد");
    case 731: return("غبار آلود");
    case 741: return("مه آلود");
    case 751: return("شن");
    case 761: return("گرد و غبار");
    case 762: return("خاکستر آتشفشانی");
    case 771: return("بوران");
    case 781: return("گرد باد");
    case 800: return("آسمان صاف");
    case 801: return("کمی ابری");
    case 802: return("ابرهای پراکنده");
    case 803: return("ابرهای پارچه پارچه شده");
    case 804: return("پوشیده از ابر");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == FI)
  switch (id) {
    case 200: return("Ukkosmyrsky ja kevyt sade");
    case 201: return("Ukkosmyrsky ja sade");
    case 202: return("Ukkosmyrsky ja kova sade");
    case 210: return("Pieni ukkosmyrsky");
    case 211: return("Ukkosmyrsky");
    case 212: return("Kova ukkosmyrsky");
    case 221: return("Lievä ukkosmyrsky");
    case 230: return("Ukkosmyrsky ja kevyt tihkusade");
    case 231: return("Ukkosmyrsky ja tihkusade");
    case 232: return("Ukkosmyrsky ja kova tihkusade");
    case 300: return("Lievä tihuttainen");
    case 301: return("Tihuttaa");
    case 302: return("Kova tihuttainen");
    case 310: return("Lievä tihkusade");
    case 311: return("Tihkusade");
    case 312: return("Kova tihkusade");
    case 313: return("Sadesuihkun ja tihkusadetta");
    case 314: return("Raskas sadesuihkun ja tihkusadetta");
    case 321: return("Tihkusade");
    case 500: return("Pieni sade");
    case 501: return("Kohtalainen sade");
    case 502: return("Kova sade");
    case 503: return("Erittäin runsasta sadetta");
    case 504: return("Kova sade");
    case 511: return("Jäätävä sade");
    case 520: return("Lievä tihkusade");
    case 521: return("Tihkusade");
    case 522: return("Kova sade");
    case 531: return("Satunnaiset suihkut");
    case 600: return("Pieni lumisade");
    case 601: return("Lumi");
    case 602: return("Paljon lunta");
    case 611: return("Räntä");
    case 612: return("Räntä suhku");
    case 620: return("Kevyt lumi suihku");
    case 621: return("Lumikuuro");
    case 622: return("Raskas lumi suihku");
    case 701: return("Sumu");
    case 711: return("Savu");
    case 721: return("Sumu");
    case 731: return("Hiekka/Pöly pyörre");
    case 741: return("Sumu");
    case 751: return("Hiekka");
    case 761: return("Pöly");
    case 762: return("Vulkaanista tuhkaa");
    case 771: return("Puuskaista tuulta");
    case 781: return("Tornado");
    case 800: return("Taivas on selkeä");
    case 801: return("Vähän pilviä");
    case 802: return("Ajoittaisia pilviä");
    case 803: return("Hajanaisia pilviä");
    case 804: return("Pilvinen");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == FR)
  switch (id) {
    case 200: return("Orage et pluie fine");
    case 201: return("Orage et pluie");
    case 202: return("Orage et forte pluie");
    case 210: return("Orage léger");
    case 211: return("Orage");
    case 212: return("Gros orage");
    case 221: return("Orages localisés");
    case 230: return("Orage et légère bruine");
    case 231: return("Orage et bruine");
    case 232: return("Orage et forte bruine");
    case 300: return("Bruine légère");
    case 301: return("Bruine");
    case 302: return("Forte bruine");
    case 310: return("Pluie très fine");
    case 311: return("Pluie fine");
    case 312: return("Grosse pluie fine");
    case 313: return("Averses de pluie et de bruine");
    case 314: return("Grosses averses de pluie et de bruine");
    case 321: return("Averses de bruine");
    case 500: return("Légère pluie");
    case 501: return("Pluie modérée");
    case 502: return("Forte pluie");
    case 503: return("Très forte pluie");
    case 504: return("Pluie extrême");
    case 511: return("Pluie verglaçante");
    case 520: return("Petites averses");
    case 521: return("Averses de pluie");
    case 522: return("Grosses averses");
    case 531: return("Averses localisées");
    case 600: return("Légères chutes de neige");
    case 601: return("Chutes de neige");
    case 602: return("Fortes chutes de neige");
    case 611: return("Neige fondue");
    case 612: return("Averses de neige fondue");
    case 615: return("Pluie légère et neige");
    case 616: return("Pluie et neige");
    case 620: return("Petites averses de neige");
    case 621: return("Averses de neige");
    case 622: return("Grosses averses de neige");
    case 701: return("Brume");
    case 711: return("Fumée");
    case 721: return("Brume sèche");
    case 731: return("Tempête de sable");
    case 741: return("Brouillard");
    case 751: return("Sable");
    case 761: return("Poussière");
    case 762: return("Cendres volcaniques");
    case 771: return("Bourrasques");
    case 781: return("Tornade");
    case 800: return("Ciel dégagé");
    case 801: return("Peu nuageux");
    case 802: return("Partiellement nuageux");
    case 803: return("Nuageux");
    case 804: return("Couvert");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == GL)
  switch (id) {
    case 200: return("Tormenta eléctrica con choiva lixeira");
    case 201: return("Tormenta eléctrica con choiva");
    case 202: return("Tormenta eléctrica con choiva intensa");
    case 210: return("Tormenta eléctrica lixeira");
    case 211: return("Tormenta eléctrica");
    case 212: return("Tormenta eléctrica forte");
    case 221: return("Tormenta eléctrica irregular");
    case 230: return("Tormenta eléctrica con orballo lixeiro");
    case 231: return("Tormenta eléctrica con orballo");
    case 232: return("Tormenta eléctrica con orballo intenso");
    case 300: return("Orballo lixeiro");
    case 301: return("Orballo");
    case 302: return("Orballo de gran intensidade");
    case 310: return("Choiva e orballo lixeiro");
    case 311: return("Choiva e orballo");
    case 312: return("Choiva e orballo de gran intensidade");
    case 313: return("Choiva e regue");
    case 314: return("Choiva aguaceiro e regue");
    case 321: return("Orballo de ducha");
    case 500: return("Choiva lixeira");
    case 501: return("Choiva moderada");
    case 502: return("Choiva de gran intensidade");
    case 503: return("Choiva moi forte");
    case 504: return("Choiva extrema");
    case 511: return("Choiva xeada");
    case 520: return("Choiva de ducha de baixa intensidade");
    case 521: return("Choiva de ducha");
    case 522: return("Choiva de ducha de gran intensidade");
    case 531: return("Choivas ocasionais");
    case 600: return("Nevada lixeira");
    case 601: return("Neve");
    case 602: return("Nevada intensa");
    case 611: return("Auganeve");
    case 612: return("Ducha saraiba");
    case 620: return("Ducha de neve Luz");
    case 621: return("Neve de ducha");
    case 622: return("Ducha de neve pesada");
    case 701: return("Néboa");
    case 711: return("Fume");
    case 721: return("Néboa lixeira");
    case 731: return("Remuiños de area e polvo");
    case 741: return("Bruma");
    case 751: return("Area");
    case 761: return("Po");
    case 762: return("Cinzas volcánicas");
    case 771: return("Squalls");
    case 781: return("Furacán");
    case 800: return("Ceo claro");
    case 801: return("Algo de nubes");
    case 802: return("Nubes dispersas");
    case 803: return("Nubes rotas");
    case 804: return("Nubes");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == HE)
  switch (id) {
    case 200: return("סופת רעמים עם גשם קל");
    case 201: return("סופת רעמים עם גשם");
    case 202: return("סופת רעמים עם גשם כבד");
    case 210: return("סופת רעמים קלה");
    case 211: return("סופת רעמים");
    case 212: return("סופת רעמים כבדה");
    case 221: return("סופות רעמים לסירוגין");
    case 230: return("סופת רעמים עם טפטוף קל");
    case 231: return("סופת רעמים עם טפטוף");
    case 232: return("סופת רעמים עם טפטוף כבד");
    case 300: return("טפטוף קל");
    case 301: return("טפטוף");
    case 302: return("טפטוף כבד");
    case 310: return("גשם מטפטף קל");
    case 311: return("גשם מטפטף");
    case 312: return("גשם מטפטף כבד");
    case 313: return("ממטרים וטפטוף");
    case 314: return("ממטרים כבדים וטפטוף");
    case 321: return("ממטר מטפטף");
    case 500: return("גשם קל");
    case 501: return("גשם בינוני");
    case 502: return("גשם כבד");
    case 503: return("גשם כבד מאוד");
    case 504: return("גשם קיצוני");
    case 511: return("גשם מקפיא");
    case 520: return("ממטרים קלים");
    case 521: return("ממטרים");
    case 522: return("ממטרים כבדים");
    case 531: return("ממטרים לסירוגין");
    case 600: return("שלג קל");
    case 601: return("שלג");
    case 602: return("שלג כבד");
    case 611: return("גשם עם שלג");
    case 612: return("ממטרים עם שלג");
    case 615: return("גשם קל ושלג");
    case 616: return("גשם ושלג");
    case 620: return("ממטרי שלג קלים");
    case 621: return("ממטרי שלג");
    case 622: return("ממטרי שלג כבדים");
    case 701: return("ערפל דק");
    case 711: return("עשן");
    case 721: return("אובך");
    case 731: return("חול, סופות אבק");
    case 741: return("ערפל");
    case 751: return("חול");
    case 761: return("אבק");
    case 762: return("אפר געשי");
    case 771: return("סערות");
    case 781: return("טורנדו");
    case 800: return("שמיים בהירים");
    case 801: return("מעונן חלקית");
    case 802: return("עננים בודדים");
    case 803: return("שברי ענן");
    case 804: return("מעונן");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == HI)
  switch (id) {
    case 200: return("हल्की वर्षा के साथ तूफान");
    case 201: return("वर्षा के साथ तूफान");
    case 202: return("भारी वर्षा के साथ तूफान");
    case 210: return("कम तीव्रता वाला तूफान");
    case 211: return("तूफान");
    case 212: return("अधिक तीव्रता वाला तूफान");
    case 221: return("अत्यधिक कमज़ोर तूफान");
    case 230: return("हल्की बूंदाबांदी के साथ तूफान");
    case 231: return("बूंदाबांदी के साथ तूफान");
    case 232: return("भारी बूंदाबांदी के साथ तूफान");
    case 300: return("कम तीव्रता वाली बूंदाबांदी");
    case 301: return("बूंदाबांदी");
    case 302: return("अधिक तीव्रता वाली बूंदाबांदी");
    case 310: return("कम तीव्र बूंदाबांदी");
    case 311: return("बूंदाबांदी");
    case 312: return("अधिक तीव्र बूंदाबांदी");
    case 313: return("बौछार और बूंदाबांदी");
    case 314: return("भारी बौछार और बूंदाबांदी");
    case 321: return("बौछार वाली बूंदाबांदी");
    case 500: return("हल्की वर्षा");
    case 501: return("मध्यम वर्षा");
    case 502: return("अधिक तीव्र वर्षा");
    case 503: return("अत्यधिक भारी वर्षा");
    case 504: return("अत्यधिक वर्षा");
    case 511: return("बर्फ के रूप में जमने वाली वर्षा");
    case 520: return("कम तीव्र बौछार");
    case 521: return("बौछार");
    case 522: return("अधिक तीव्र बौछार");
    case 531: return("अत्यधिक कमज़ोर बौछार");
    case 600: return("हल्का हिमपात");
    case 601: return("हिमपात");
    case 602: return("भारी हिमपात");
    case 611: return("ओलों के साथ वर्षा");
    case 612: return("ओलों के साथ बौछार");
    case 615: return("हल्की वर्षा और हिमपात");
    case 616: return("वर्षा और हिमपात");
    case 620: return("हल्की बौछार वाला हिमपात");
    case 621: return("बौछार वाला हिमपात");
    case 622: return("अधिक बौछार वाला हिमपात");
    case 701: return("धुंध");
    case 711: return("धुआं");
    case 721: return("कुहरा");
    case 731: return("रेत, धूल के भँवर");
    case 741: return("कोहरा");
    case 751: return("रेत");
    case 761: return("धूल");
    case 762: return("ज्वालामुखी की राख");
    case 771: return("आँधी");
    case 781: return("बवंडर");
    case 800: return("साफ आकाश");
    case 801: return("कुछ बादल");
    case 802: return("छितरे हुए बादल");
    case 803: return("टूटे हुए बादल");
    case 804: return("घनघोर बादल");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == HR)
  switch (id) {
    case 200: return("Grmljavinska oluja s slabom kišom");
    case 201: return("Grmljavinska oluja s kišom");
    case 202: return("Grmljavinska oluja s jakom kišom");
    case 210: return("Slaba grmljavinska oluja");
    case 211: return("Grmljavinska oluja");
    case 212: return("Jaka grmljavinska oluja");
    case 221: return("Orkanska grmljavinska oluja");
    case 230: return("Grmljavinska oluja sa slabom rosuljom");
    case 231: return("Grmljavinska oluja s rosuljom");
    case 232: return("Grmljavinska oluja sa jakom rosuljom");
    case 300: return("Rosulja slabog intenziteta");
    case 301: return("Rosulja");
    case 302: return("Rosulja jakog intenziteta");
    case 310: return("Rosulja s kišom slabog intenziteta");
    case 311: return("Rosulja s kišom");
    case 312: return("Rosulja s kišom jakog intenziteta");
    case 313: return("Pljuskovi i rosulja");
    case 314: return("Rosulja s jakim pljuskovima");
    case 321: return("Rosulja s pljuskovima");
    case 500: return("Slaba kiša");
    case 501: return("Umjerena kiša");
    case 502: return("Kiša jakog intenziteta");
    case 503: return("Vrlo jaka kiša");
    case 504: return("Ekstremna kiša");
    case 511: return("Ledena kiša");
    case 520: return("Pljusak slabog intenziteta");
    case 521: return("Pljusak");
    case 522: return("Pljusak jakog intenziteta");
    case 531: return("Olujna kiša s pljuskovima");
    case 600: return("Slabi snijeg");
    case 601: return("Snijeg");
    case 602: return("Gusti snijeg");
    case 611: return("Susnježica");
    case 612: return("Susnježica s pljuskovima");
    case 615: return("Slaba kiša i snijeg");
    case 616: return("Kiša i snijeg");
    case 620: return("Snijeg s povremenim pljuskovima");
    case 621: return("Snijeg s pljuskovima");
    case 622: return("Snijeg s jakim pljuskovima");
    case 701: return("Sumaglica");
    case 711: return("Dim");
    case 721: return("Izmaglica");
    case 731: return("Kovitlaci pijeska ili prašine");
    case 741: return("Magla");
    case 751: return("Pijesak");
    case 761: return("Prašina");
    case 762: return("Vulkanski pepeo");
    case 771: return("Zapusi vjetra s kišom");
    case 781: return("Tornado");
    case 800: return("Vedro");
    case 801: return("Blaga naoblaka");
    case 802: return("Raštrkani oblaci");
    case 803: return("Isprekidani oblaci");
    case 804: return("Oblačno");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == HU)
  switch (id) {
    case 200: return("Vihar enyhe esővel");
    case 201: return("Vihar esővel");
    case 202: return("Vihar heves esővel");
    case 210: return("Enyhe zivatar");
    case 211: return("Vihar");
    case 212: return("Heves vihar");
    case 221: return("Durva vihar");
    case 230: return("Vihar enyhe szitálással");
    case 231: return("Vihar szitálással");
    case 232: return("Vihar erős szitálással");
    case 300: return("Enyhe intenzitású szitálás");
    case 301: return("Szitálás");
    case 302: return("Erős intenzitású szitálás");
    case 310: return("Enyhe intenzitású szitáló eső");
    case 311: return("Szitáló eső");
    case 312: return("Erős intenzitású szitáló eső");
    case 313: return("Zuhanyeső szitálás");
    case 314: return("Nehéz zuhanyeső szitálás");
    case 321: return("Zápor");
    case 500: return("Enyhe eső");
    case 501: return("Közepes eső");
    case 502: return("Heves intenzitású eső");
    case 503: return("Nagyon heves eső");
    case 504: return("Extrém eső");
    case 511: return("Ónos eső");
    case 520: return("Enyhe intenzitású zápor");
    case 521: return("Zápor");
    case 522: return("Erős intenzitású zápor");
    case 531: return("Alkalmi zuhanyok");
    case 600: return("Enyhe havazás");
    case 601: return("Havazás");
    case 602: return("Erős havazás");
    case 611: return("Havas eső");
    case 612: return("Ónos eső zuhany");
    case 615: return("Enyhe esőzés és havazás");
    case 616: return("Hó és eső");
    case 620: return("Enyhe hózápor");
    case 621: return("Hózápor");
    case 622: return("Intenzív hózápor");
    case 701: return("Gyenge köd");
    case 711: return("Köd");
    case 721: return("Köd");
    case 731: return("Homokvihar");
    case 741: return("Köd");
    case 751: return("Homok");
    case 761: return("Por");
    case 762: return("Vulkáni hamu");
    case 771: return("Vihar");
    case 781: return("Tornádó");
    case 800: return("Tiszta égbolt");
    case 801: return("Kevés felhő");
    case 802: return("Szórványos felhőzet");
    case 803: return("Erős felhőzet");
    case 804: return("Borús égbolt");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == ID)
  switch (id) {
    case 200: return("Badai dengan hujan rintik-rintik");
    case 201: return("Badai dengan hujan");
    case 202: return("Badai dengan hujan lebat");
    case 210: return("Badai ringan");
    case 211: return("Badai");
    case 212: return("Badai yang berat");
    case 221: return("Badai yang buruk");
    case 230: return("Badai dengan gerimis kecil");
    case 231: return("Badai dengan gerimis");
    case 232: return("Badai dengan gerimis lebat");
    case 300: return("Gerimis berintensitas rendah");
    case 301: return("Gerimis");
    case 302: return("Gerimis berintensitas tinggi");
    case 310: return("Hujan gerimis berintensitas rendah");
    case 311: return("Hujan gerimis");
    case 312: return("Hujan gerimis berintensitas tinggi");
    case 313: return("Hujan sesaat dan gerimis");
    case 314: return("Hujan deras sesaat dan gerimis");
    case 321: return("Gerimis sesaat");
    case 500: return("Hujan rintik-rintik");
    case 501: return("Hujan sedang");
    case 502: return("Hujan berintensitas tinggi");
    case 503: return("Hujan yang sangat lebat");
    case 504: return("Hujan ekstrem");
    case 511: return("Hujan yang sangat dingin");
    case 520: return("Hujan sesaat berintensitas rendah");
    case 521: return("Hujan sesaat");
    case 522: return("Hujan sesaat berintensitas tinggi");
    case 531: return("Hujan deras sesaat");
    case 600: return("Salju ringan");
    case 601: return("Salju");
    case 602: return("Salju lebat");
    case 611: return("Hujan bercampur salju");
    case 612: return("Hujan es");
    case 615: return("Hujan rintik-rintik dan salju");
    case 616: return("Hujan dan salju");
    case 620: return("Hujan salju ringan");
    case 621: return("Hujan salju");
    case 622: return("Hujan salju lebat");
    case 701: return("Halimun");
    case 711: return("Berasap");
    case 721: return("Kabut asap");
    case 731: return("Pusaran debu, pasir");
    case 741: return("Kabut");
    case 751: return("Pasir");
    case 761: return("Debu");
    case 762: return("Abu vulkanik");
    case 771: return("Angin kencang");
    case 781: return("Angin topan");
    case 800: return("Langit cerah");
    case 801: return("Sedikit berawan");
    case 802: return("Awan tersebar");
    case 803: return("Awan pecah");
    case 804: return("Awan mendung");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == IT)
  switch (id) {
    case 200: return("Temporale con pioggerella");
    case 201: return("Temporale con pioggia");
    case 202: return("Temporale con pioggia forte");
    case 210: return("Temporale");
    case 211: return("Temporale");
    case 212: return("Temporale forte");
    case 221: return("Temporale");
    case 230: return("Temporale con pioggerella");
    case 231: return("Temporale con pioggerella");
    case 232: return("Temporale con pioggerella");
    case 300: return("Pioggerella");
    case 301: return("Pioggerella");
    case 302: return("Pioggerella");
    case 310: return("Pioggerella");
    case 311: return("Pioggerella");
    case 312: return("Forte pioggerella");
    case 313: return("Doccia a pioggia e pioggia");
    case 314: return("Doccia a pioggia pesante e pioggerellina");
    case 321: return("Acquazzone");
    case 500: return("Pioggia leggera");
    case 501: return("Pioggia moderata");
    case 502: return("Forte pioggia");
    case 503: return("Pioggia fortissima");
    case 504: return("Pioggia estrema");
    case 511: return("Pioggia gelata");
    case 520: return("Pioggerella");
    case 521: return("Acquazzone");
    case 522: return("Acquazzone");
    case 531: return("Occasionali rovesci");
    case 600: return("Neve");
    case 601: return("Neve");
    case 602: return("Forte nevicata");
    case 611: return("Nevischio");
    case 612: return("Doccia nevischio");
    case 620: return("Doccia di neve leggera");
    case 621: return("Forte nevicata");
    case 622: return("Doccia di neve pesante");
    case 701: return("Foschia");
    case 711: return("Fumo");
    case 721: return("Foschia");
    case 731: return("Mulinelli di sabbia/polvere");
    case 741: return("Nebbia");
    case 751: return("Sabbia");
    case 761: return("La polvere");
    case 762: return("Cenere vulcanica");
    case 771: return("Bufere");
    case 781: return("Tornado");
    case 800: return("Cielo sereno");
    case 801: return("Poche nuvole");
    case 802: return("Nubi sparse");
    case 803: return("Nubi sparse");
    case 804: return("Cielo coperto");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == JA)
  switch (id) {
    case 200: return("雷を伴う弱い雨");
    case 201: return("雷を伴う雨");
    case 202: return("雷を伴う強い雨");
    case 210: return("弱い雷雨");
    case 211: return("雷雨");
    case 212: return("強い雷雨");
    case 221: return("時々雷雨");
    case 230: return("雷を伴う弱い霧雨");
    case 231: return("雷を伴う霧雨");
    case 232: return("雷を伴う強い霧雨");
    case 300: return("弱い霧雨");
    case 301: return("霧雨");
    case 302: return("強い霧様");
    case 310: return("弱い霧雨");
    case 311: return("霧雨");
    case 312: return("強い霧雨");
    case 313: return("にわか雨と霧雨");
    case 314: return("強いにわか雨と霧雨");
    case 321: return("にわか霧雨");
    case 500: return("小雨");
    case 501: return("適度な雨");
    case 502: return("強い雨");
    case 503: return("激しい雨");
    case 504: return("豪雨");
    case 511: return("氷晶雨");
    case 520: return("弱いにわか雨");
    case 521: return("にわか雨");
    case 522: return("強いにわか雨");
    case 531: return("時々雨");
    case 600: return("小雪");
    case 601: return("雪");
    case 602: return("大雪");
    case 611: return("みぞれ");
    case 612: return("にわかみぞれ");
    case 615: return("小雨と雪");
    case 616: return("雨と雪");
    case 620: return("弱いにわか雪");
    case 621: return("にわか雪");
    case 622: return("強いにわか雪");
    case 701: return("霧");
    case 711: return("煙");
    case 721: return("薄霧");
    case 731: return("砂、ほこり、渦巻き");
    case 741: return("霧");
    case 751: return("砂");
    case 761: return("ほこり");
    case 762: return("火山灰");
    case 771: return("スコール");
    case 781: return("竜巻");
    case 800: return("晴天");
    case 801: return("薄い雲");
    case 802: return("雲");
    case 803: return("曇りがち");
    case 804: return("厚い雲");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == KR)
  switch (id) {
    case 200: return("가벼운 비를 동반한 뇌우");
    case 201: return("비를 동반한 뇌우");
    case 202: return("폭우를 동반한 뇌우");
    case 210: return("약한 뇌우");
    case 211: return("뇌우");
    case 212: return("강한 뇌우");
    case 221: return("불규칙적 뇌우");
    case 230: return("약한 연무를 동반한 뇌우");
    case 231: return("연무를 동반한 뇌우");
    case 232: return("강한 이슬비를 동반한 뇌우");
    case 300: return("가벼운 안개비");
    case 301: return("안개비");
    case 302: return("강한 안개비");
    case 310: return("가벼운 이슬비");
    case 311: return("이슬비");
    case 312: return("강한 이슬비");
    case 313: return("소나기와 안개비");
    case 314: return("강한 소나기와 안개비");
    case 321: return("소나기");
    case 500: return("실 비");
    case 501: return("보통 비");
    case 502: return("강한 비");
    case 503: return("매우 강한 비");
    case 504: return("극심한 비");
    case 511: return("우박");
    case 520: return("약한 소나기 비");
    case 521: return("소나기 비");
    case 522: return("강한 소나기 비");
    case 531: return("불규칙적 소나기 비");
    case 600: return("가벼운 눈");
    case 601: return("눈");
    case 602: return("강한 눈");
    case 611: return("진눈깨비");
    case 612: return("소나기 진눈깨비");
    case 615: return("약한 비와 눈");
    case 616: return("비와 눈");
    case 620: return("약한 소나기 눈");
    case 621: return("소나기 눈");
    case 622: return("강한 소나기 눈");
    case 701: return("박무");
    case 711: return("연기");
    case 721: return("연무");
    case 731: return("모래 먼지 회오리바람");
    case 741: return("안개");
    case 751: return("모래");
    case 761: return("먼지");
    case 762: return("화산재");
    case 771: return("스콜");
    case 781: return("토네이도");
    case 800: return("맑음");
    case 801: return("약간의 구름이 낀 하늘");
    case 802: return("구름조금");
    case 803: return("튼구름");
    case 804: return("온흐림");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == LA)
  switch (id) {
    case 200: return("Negaiss ar nokrišņiem");
    case 201: return("Negaiss un lietus");
    case 202: return("Negais un stiprs lietus");
    case 210: return("Negaiss");
    case 211: return("Pērkona negaiss");
    case 212: return("Pērkona negaiss");
    case 221: return("Negaiss");
    case 230: return("Pērkona negaiss");
    case 231: return("Pērkona negaiss");
    case 232: return("Pērkona negaiss");
    case 300: return("Līņā");
    case 301: return("Līņā");
    case 302: return("Stipri līņā");
    case 310: return("Lietus");
    case 311: return("Līņā");
    case 312: return("Stiprs lietus");
    case 313: return("Stiprs lietus");
    case 314: return("Stiprs lietus");
    case 321: return("Smidzina");
    case 500: return("Smidzina");
    case 501: return("Lietus");
    case 502: return("Stiprs lietus");
    case 503: return("Stiprs lietus");
    case 504: return("Stiprs lietus");
    case 511: return("Sasalstošais lietus");
    case 520: return("Stiprs lietus");
    case 521: return("Stiprs lietus");
    case 522: return("Stiprs lietus");
    case 531: return("Stiprs lietus");
    case 600: return("Viegls sniegs");
    case 601: return("Sniegs");
    case 602: return("Sniegs");
    case 611: return("Lietus un sniegs");
    case 612: return("Lietus un sniegs");
    case 615: return("Lietus un sniegs");
    case 616: return("Lietus un sniegs");
    case 620: return("Sniegs");
    case 621: return("Slapjš sniegs");
    case 622: return("Slapjš sniegs");
    case 701: return("Migla");
    case 711: return("Miglains");
    case 721: return("Miglains");
    case 731: return("Putekļu virpuļi");
    case 741: return("Migla");
    case 751: return("Smiltis");
    case 761: return("Putekļi");
    case 762: return("Vulkāniskie pelni");
    case 771: return("Brāzmas");
    case 781: return("Tornado");
    case 800: return("Skaidrs laiks");
    case 801: return("Mākoņains");
    case 802: return("Mākoņains");
    case 803: return("Mākoņains");
    case 804: return("Mākoņains");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == LT)
  switch (id) {
    case 200: return("Perkūnija su lengvu lietumi");
    case 201: return("Perkūnija su lietumi");
    case 202: return("Perkūnija su smarkiu lietumi");
    case 210: return("Nedidelė perkūnija");
    case 211: return("Perkūnija");
    case 212: return("Smarki perkūnija");
    case 221: return("Vietomis perkūnija");
    case 230: return("Perkūnija su lengva dulksna");
    case 231: return("Perkūnija su dulksna");
    case 232: return("Perkūnija su stipria dulksna");
    case 300: return("Mažo intensyvumo dulksna");
    case 301: return("Dulksna");
    case 302: return("Didelio intensyvumo dulksna");
    case 310: return("Mažo intensyvumo dulkiantis lietus");
    case 311: return("Dulkiantis lietus");
    case 312: return("Didelio intensyvumo dulkiantis lietus");
    case 313: return("Lietus ir dulksna");
    case 314: return("Stiprus lietus ir dulksna");
    case 321: return("Dulksna");
    case 500: return("Lengvas lietus");
    case 501: return("Nepastovus lietus");
    case 502: return("Didelio intensyvumo lietus");
    case 503: return("Labai stiprus lietus");
    case 504: return("Ekstremalus lietus");
    case 511: return("Stingdantis lietus");
    case 520: return("Mažo intensyvumo lietus");
    case 521: return("Lietus");
    case 522: return("Didelio intensyvumo lietus");
    case 531: return("Vietomis lietus");
    case 600: return("Lengvas sniegas");
    case 601: return("Sniegas");
    case 602: return("Smarkus sniegas");
    case 611: return("Šlapdriba");
    case 612: return("Šlapdriba");
    case 615: return("Lengvas lietus su sniegu");
    case 616: return("Lietus su sniegu");
    case 620: return("Lengvas sniegas");
    case 621: return("Sniegas");
    case 622: return("Smarkus sniegas");
    case 701: return("Migla");
    case 711: return("Smogas");
    case 721: return("Migla");
    case 731: return("Smėlis, dulkių sūkuriai");
    case 741: return("Rūkas");
    case 751: return("Smėlis");
    case 761: return("Dulkės");
    case 762: return("Vulkaniniai pelenai");
    case 771: return("Škvalas");
    case 781: return("Tornadas");
    case 800: return("Giedra");
    case 801: return("Keli debesys");
    case 802: return("Mažai debesuota");
    case 803: return("Debesuota su pragiedruliais");
    case 804: return("Debesuota");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == MK)
  switch (id) {
    case 200: return("Грмежи со слаб дожд");
    case 201: return("Грмежи со дожд");
    case 202: return("Грмежи со силен дожд");
    case 210: return("Слаби грмежи");
    case 211: return("Грмежи");
    case 212: return("Силни грмежи");
    case 221: return("Многу силни грмежи");
    case 230: return("Грмежи со слабо росење");
    case 231: return("Грмежи со росење");
    case 232: return("Грмежи со силно росење");
    case 300: return("Слабо росење");
    case 301: return("Росење");
    case 302: return("Силно росење");
    case 310: return("Слабо росење");
    case 311: return("Росење");
    case 312: return("Силно росење");
    case 313: return("Изригвам дожд и ситен дожд");
    case 314: return("Тешки туш дожд и ситен дожд");
    case 321: return("Дожд");
    case 500: return("Слаб дожд");
    case 501: return("Слаб дожд");
    case 502: return("Силен дожд");
    case 503: return("Многу силен дожд");
    case 504: return("Обилен дожд");
    case 511: return("Град");
    case 520: return("Слабо росење");
    case 521: return("Роси");
    case 522: return("Силно росење");
    case 531: return("Повремен дожд");
    case 600: return("Слаб снег");
    case 601: return("Снег");
    case 602: return("Силен снег");
    case 611: return("Лапавица");
    case 612: return("Лапавица туш");
    case 620: return("Слаб снег туш");
    case 621: return("Лапавица");
    case 622: return("Обилните снежни врнежи туш");
    case 701: return("Магла");
    case 711: return("Смог");
    case 721: return("Замагленост");
    case 731: return("Песочен вртлог");
    case 741: return("Магла");
    case 751: return("Прашина од ветерот носена");
    case 761: return("Прашина");
    case 762: return("Вулкански пепел");
    case 771: return("Налетот");
    case 781: return("Торнадо");
    case 800: return("Чисто небо");
    case 801: return("Неколку облаци");
    case 802: return("Одвоени облаци");
    case 803: return("Облаци");
    case 804: return("Облачно");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == NO)
  switch (id) {
    case 200: return("Lyn og torden med lett regn");
    case 201: return("Lyn og torden med regn");
    case 202: return("Lyn og torden med kraftig regn");
    case 210: return("Lett lyn og torden");
    case 211: return("Lyn og torden");
    case 212: return("Kraftig lyn og torden");
    case 221: return("Voldsomme byger med lyn og torden");
    case 230: return("Lyn og torden med lett yr");
    case 231: return("Lyn og torden med yr");
    case 232: return("Lyn og torden med kraftig yr");
    case 300: return("Lett yr");
    case 301: return("Yr");
    case 302: return("Kraftig yr");
    case 310: return("Lett yr og regn");
    case 311: return("Yr og regn");
    case 312: return("Kraftig yr og regn");
    case 313: return("Yr- og regnbyger");
    case 314: return("Kraftige yr- og regnbyger");
    case 321: return("Yrbyger");
    case 500: return("Lett regn");
    case 501: return("Moderat regn");
    case 502: return("Kraftig regn");
    case 503: return("Svært kraftig regn");
    case 504: return("Ekstremt regn");
    case 511: return("Regn som fryser");
    case 520: return("Lette regnbyger");
    case 521: return("Regnbyger");
    case 522: return("Kraftige regnbyger");
    case 531: return("Voldsomme regnbyger");
    case 600: return("Lett snø");
    case 601: return("Snø");
    case 602: return("Kraftig snø");
    case 611: return("Sludd");
    case 612: return("Sluddbyger");
    case 615: return("Lett regn og snø");
    case 616: return("Regn og snø");
    case 620: return("Lette snøbyger");
    case 621: return("Snøbyger");
    case 622: return("Kraftige snøbyger");
    case 701: return("Tåke");
    case 711: return("Røyk");
    case 721: return("Dis");
    case 731: return("Sand- eller støvskyer");
    case 741: return("Tåke");
    case 751: return("Sand");
    case 761: return("Støv");
    case 762: return("Vulkansk aske");
    case 771: return("Stormbyger");
    case 781: return("Tornado");
    case 800: return("Klart");
    case 801: return("Enkelte skyer");
    case 802: return("Spredte skyer");
    case 803: return("Spredt skydekke");
    case 804: return("Overskyet");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == NL)
  switch (id) {
    case 200: return("Onweersbui met lichte regen");
    case 201: return("Onweersbui met regen");
    case 202: return("Onweersbui met zware regenval");
    case 210: return("Lichte onweersbui");
    case 211: return("Onweersbui");
    case 212: return("Zware onweersbui");
    case 221: return("Onregelmatig onweersbui");
    case 230: return("Onweersbui met lichte motregen");
    case 231: return("Onweersbui met motregen");
    case 232: return("Onweersbui met zware motregen");
    case 300: return("Lichte motregen");
    case 301: return("Motregen");
    case 302: return("Zware motregen");
    case 310: return("Lichte motregen/regen");
    case 311: return("Motregen");
    case 312: return("Zware motregen/regen");
    case 313: return("Stortbuien en motregen");
    case 314: return("Hevige stortbuien en motregen");
    case 321: return("Zware motregen");
    case 500: return("Lichte regen");
    case 501: return("Matige regen");
    case 502: return("Zware regenval");
    case 503: return("Zeer zware regenval");
    case 504: return("Extreme regen");
    case 511: return("Koude buien");
    case 520: return("Lichte stortregen");
    case 521: return("Stortregen");
    case 522: return("Zware stortregen");
    case 531: return("Afwisselende stortbuien");
    case 600: return("Lichte sneeuw");
    case 601: return("Sneeuw");
    case 602: return("Hevige sneeuw");
    case 611: return("Ijzel");
    case 612: return("Lichte ijzel");
    case 620: return("Lichte sneeuw");
    case 621: return("Natte sneeuw");
    case 622: return("Hevige sneeuw");
    case 701: return("Mist");
    case 711: return("Mist");
    case 721: return("Nevel");
    case 731: return("Zand/Stof werveling");
    case 741: return("Mist");
    case 751: return("Zandstorm");
    case 761: return("Mist");
    case 762: return("Vulkanische as");
    case 771: return("Rukwinden");
    case 781: return("Tornado");
    case 800: return("Onbewolkt");
    case 801: return("Licht bewolkt");
    case 802: return("Half bewolkt");
    case 803: return("Zwaar bewolkt");
    case 804: return("Geheel bewolkt");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == PL)
  switch (id) {
    case 200: return("Burza z lekkimi opadami deszczu");
    case 201: return("Burza z opadami deszczu");
    case 202: return("Burza z intensywnymi opadami deszczu");
    case 210: return("Lekka burza");
    case 211: return("Burza");
    case 212: return("Silna burza");
    case 221: return("Burza");
    case 230: return("Burza z lekką mżawką");
    case 231: return("Burza z mżawka");
    case 232: return("Burza z intensywną mżawką");
    case 300: return("Lekka mżawka");
    case 301: return("Mżawka");
    case 302: return("Intensywna mżawka");
    case 310: return("Lekkie opady drobnego deszczu");
    case 311: return("Deszcz / mżawka");
    case 312: return("Intensywny deszcz / mżawka");
    case 313: return("Ulewny deszcz i mżawka");
    case 314: return("Gęsty ulewny deszcz i mżawka");
    case 321: return("Silna mżawka");
    case 500: return("Słabe opady deszczu");
    case 501: return("Umiarkowane opady deszczu");
    case 502: return("Silne opady deszczu");
    case 503: return("Bardzo silne opady deszczu");
    case 504: return("Ulewa");
    case 511: return("Marznący deszcz");
    case 520: return("Słabe, przelotne opady deszczu");
    case 521: return("Deszcz");
    case 522: return("Silne, przelotne opady deszczu");
    case 531: return("Okazjonalne opady deszczu");
    case 600: return("Słabe opady śniegu");
    case 601: return("Śnieg");
    case 602: return("Silne opady śniegu");
    case 611: return("Deszcz ze śniegem");
    case 612: return("Przelotne opady deszczu ze śniegiem");
    case 620: return("Słabe przelotne opady śniegu");
    case 621: return("Przelotne opady śniegu");
    case 622: return("Silne opady śniegu");
    case 701: return("Zamglenie");
    case 711: return("Dym");
    case 721: return("Zamglenia");
    case 731: return("Zamieć piaskowa");
    case 741: return("Mgła");
    case 751: return("Piasek");
    case 761: return("Pył");
    case 762: return("Pył wulkaniczny");
    case 771: return("Szkwał");
    case 781: return("Tornado");
    case 800: return("Bezchmurnie");
    case 801: return("Lekkie zachmurzenie");
    case 802: return("Rozproszone chmury");
    case 803: return("Pochmurno z przejaśnieniami");
    case 804: return("Całkowite zachmurzenie");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == PT)
  switch (id) {
    case 200: return("Trovoada com chuva fraca");
    case 201: return("Trovoada com chuva");
    case 202: return("Trovoada com chuva forte");
    case 210: return("Trovoada leve");
    case 211: return("Trovoada");
    case 212: return("Trovoada forte");
    case 221: return("Trovoada irregular");
    case 230: return("Trovoada com chuvisco fraco");
    case 231: return("Trovoada com chuvisco");
    case 232: return("Trovoada com chuvisco forte");
    case 300: return("Chuvisco fraco");
    case 301: return("Chuvisco");
    case 302: return("Chuvisco forte");
    case 310: return("Chuvisco e chuva fraca");
    case 311: return("Chuvisco e chuva");
    case 312: return("Chuvisco e chuva forte");
    case 313: return("Aguaceiros e chuvisco");
    case 314: return("Aguaceiros fortes chuva e chuvisco");
    case 321: return("Aguaceiros e chuvisco");
    case 500: return("Chuva fraca");
    case 501: return("Chuva moderada");
    case 502: return("Chuva forte");
    case 503: return("Chuva muito forte");
    case 504: return("Chuva extrema");
    case 511: return("Chuva com congelamento");
    case 520: return("Aguaceiros fracos");
    case 521: return("Aguaceiros");
    case 522: return("Aguaceiros fortes");
    case 531: return("Aguaceiros ocasionais");
    case 600: return("Neve fraca");
    case 601: return("Neve");
    case 602: return("Neve forte");
    case 611: return("Chuva com neve");
    case 612: return("Aguaceiros com neve");
    case 615: return("Chuva fraca e neve");
    case 616: return("Chuva e neve");
    case 620: return("Neve fraca intermitente");
    case 621: return("Neve intermitente");
    case 622: return("Neve forte intermitente");
    case 701: return("Nevoeiro");
    case 711: return("Fumo");
    case 721: return("Névoa");
    case 731: return("Turbilhões de areia/poeira");
    case 741: return("Neblina");
    case 751: return("Areia");
    case 761: return("Poeira");
    case 762: return("Cinzas vulcânicas");
    case 771: return("Fumo");
    case 781: return("Tornado");
    case 800: return("Céu limpo");
    case 801: return("Céu pouco nublado");
    case 802: return("Nuvens dispersas");
    case 803: return("Nuvens quebradas");
    case 804: return("Nublado");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == PT_BR)
  switch (id) {
    case 200: return("Trovoada com chuva fraca");
    case 201: return("Trovoada com chuva");
    case 202: return("Trovoada com chuva forte");
    case 210: return("Tempestade com raios");
    case 211: return("Trovoadas");
    case 212: return("Tempestades");
    case 221: return("Trovoadas");
    case 230: return("Trovoada com leve garoa");
    case 231: return("Trovoada com garoa");
    case 232: return("Trovoada com forte garoa");
    case 300: return("Garoa de leve intensidade");
    case 301: return("Chuviscos");
    case 302: return("Chuvas");
    case 310: return("Garoa leve");
    case 311: return("Chuva");
    case 312: return("Chuva forte");
    case 313: return("Chuva e chuviscos");
    case 314: return("Chuva forte e chuviscos");
    case 321: return("Chuviscos");
    case 500: return("Chuva leve");
    case 501: return("Chuva moderada");
    case 502: return("Chuva forte");
    case 503: return("Chuva muito forte");
    case 504: return("Chuva extrema");
    case 511: return("Chuva congelante");
    case 520: return("Chuviscos com intensidade de raios");
    case 521: return("Chuvas");
    case 522: return("Chuvas fortes");
    case 531: return("Chuva irregular");
    case 600: return("Pouca neve");
    case 601: return("Neve");
    case 602: return("Nevasca");
    case 611: return("Granizo");
    case 612: return("Chuva de granizo");
    case 615: return("Chuva leve e neve");
    case 616: return("Chuva e neve");
    case 620: return("Garoa e neve");
    case 621: return("Neve");
    case 622: return("Forte neve");
    case 701: return("Névoa");
    case 711: return("Fumaça");
    case 721: return("Neblina");
    case 731: return("Areia,Turbilhões de poeira");
    case 741: return("Névoa");
    case 751: return("Chuva de areia");
    case 761: return("Poeira");
    case 762: return("Cinza vulcanica");
    case 771: return("Rajadas");
    case 781: return("Tornado");
    case 800: return("Céu limpo");
    case 801: return("Algumas nuvens");
    case 802: return("Nuvens dispersas");
    case 803: return("Nublado");
    case 804: return("Nublado");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == RO)
  switch (id) {
    case 200: return("Furtună cu ploaie ușoară");
    case 201: return("Furtună");
    case 202: return("Furtună cu ploaie puternică");
    case 210: return("Furtună ușoară");
    case 211: return("Furtună");
    case 212: return("Furtună puternică");
    case 221: return("Furtună aprigă");
    case 230: return("Furtună cu burniță");
    case 231: return("Furtună cu burniță");
    case 232: return("Furtună cu burniță");
    case 300: return("Burniță de intensitate joasă");
    case 301: return("Burniță");
    case 302: return("Burniță de intensitate mare");
    case 310: return("Burniță de intensitate joasă");
    case 311: return("Burniță");
    case 312: return("Burniță de intensitate mare");
    case 313: return("Spew ploaie și burniță");
    case 314: return("Ploaie de ploaie grea și burniță");
    case 321: return("Burniță");
    case 500: return("Ploaie ușoară");
    case 501: return("Ploaie");
    case 502: return("Ploaie puternică");
    case 503: return("Ploaie torențială");
    case 504: return("Ploaie extremă");
    case 511: return("Ploaie înghețată");
    case 520: return("Ploaie de scurtă durată");
    case 521: return("Ploaie de scurtă durată");
    case 522: return("Ploaie de scurtă durată");
    case 531: return("Dusuri ocazionale");
    case 600: return("Ninsoare ușoară");
    case 601: return("Ninsoare");
    case 602: return("Ninsoare puternică");
    case 611: return("Lapoviță");
    case 612: return("Cabină de duș lapoviță");
    case 620: return("Dus ninsoare ușoară");
    case 621: return("Ninsoare de scurtă durată");
    case 622: return("Duș zăpadă grea");
    case 701: return("Ceață");
    case 711: return("Ceață");
    case 721: return("Ceață");
    case 731: return("Vârtejuri de nisip");
    case 741: return("Ceață");
    case 751: return("Nisip");
    case 761: return("Praf");
    case 762: return("Cenușă vulcanică");
    case 771: return("Furtuni");
    case 781: return("Tornadă");
    case 800: return("Cer senin");
    case 801: return("Câțiva nori");
    case 802: return("Nori împrăștiați");
    case 803: return("Cer fragmentat");
    case 804: return("Cer acoperit de nori");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == SE)
  switch (id) {
    case 200: return("Åskväder med lätt regn");
    case 201: return("Åskväder med regn");
    case 202: return("Åskväder med kraftigt regn");
    case 210: return("Svagt åskväder");
    case 211: return("Åskväder");
    case 212: return("Kraftigt åskväder");
    case 221: return("Visst åskväder");
    case 230: return("Åskväder med lätt duggregn");
    case 231: return("Åskväder med duggregn");
    case 232: return("Åskväder med kraftigt duggregn");
    case 300: return("Lätt duggregn");
    case 301: return("Duggregn");
    case 302: return("Kraftigt duggregn");
    case 310: return("Lätt duggregn");
    case 311: return("Duggregn");
    case 312: return("Kraftigt duggregn");
    case 313: return("Regnskurar och duggregn");
    case 314: return("Kraftiga regnskurar och duggregn");
    case 321: return("Skurar med duggregn");
    case 500: return("Lätt regn");
    case 501: return("Något regn");
    case 502: return("Måttligt regn");
    case 503: return("Kraftigt regn");
    case 504: return("Rikligt med regn");
    case 511: return("Underkylt regn");
    case 520: return("Lätta regnskurar");
    case 521: return("Regnskurar");
    case 522: return("Kraftiga regnskurar");
    case 531: return("Enstaka regnskurar");
    case 600: return("Lätt snöfall");
    case 601: return("Snöfall");
    case 602: return("Kraftigt snöfall");
    case 611: return("Snöblandat regn");
    case 612: return("Skurar med snöblandat regn");
    case 615: return("Lätt regn och snö");
    case 616: return("Regn och snö");
    case 620: return("Lätta snöbyar");
    case 621: return("Snöbyar");
    case 622: return("Kraftiga snöbyar");
    case 701: return("Dimma");
    case 711: return("Smogg");
    case 721: return("Dis");
    case 731: return("Sandstorm");
    case 741: return("Dimma");
    case 751: return("Sand");
    case 761: return("Damm");
    case 762: return("Vulkanisk aska");
    case 771: return("Vindstötar");
    case 781: return("Tromb");
    case 800: return("Klar himmel");
    case 801: return("Lätt molnighet");
    case 802: return("Växlande molnighet");
    case 803: return("Molnigt");
    case 804: return("Mulet");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == SK)
  switch (id) {
    case 200: return("Búrka so slabým dažďom");
    case 201: return("Búrka s dažďom");
    case 202: return("Búrka so silným dažďom");
    case 210: return("Mierna búrka");
    case 211: return("Búrka");
    case 212: return("Silná búrka");
    case 221: return("Prudká búrka");
    case 230: return("Búrka so slabým mrholením");
    case 231: return("Búrka s mrholením");
    case 232: return("Búrka so silným mrholením");
    case 300: return("Slabé mrholenie");
    case 301: return("Mrholenie");
    case 302: return("Silné mrholenie");
    case 310: return("Slabé popŕchanie");
    case 311: return("Popŕchanie");
    case 312: return("Silné popŕchanie");
    case 313: return("Prehánka s mrholením");
    case 314: return("Silná prehánka s mrholením");
    case 321: return("Jemné mrholenie");
    case 500: return("Slabý dážď");
    case 501: return("Mierny dážď");
    case 502: return("Silný dážď");
    case 503: return("Veľmi silný dážď");
    case 504: return("Extrémny dážď");
    case 511: return("Mrznúci dážď");
    case 520: return("Slabá prehánka");
    case 521: return("Prehánka");
    case 522: return("Silná prehánka");
    case 531: return("Občasné prehánky");
    case 600: return("Slabé sneženie");
    case 601: return("Sneženie");
    case 602: return("Silné sneženie");
    case 611: return("Dážď so snehom");
    case 612: return("Slabá prehánka dažďa a snehu");
    case 620: return("Slabá snehová prehánka");
    case 621: return("Snehová prehánka");
    case 622: return("Silná snehová prehánka");
    case 701: return("Hmla");
    case 711: return("Dym");
    case 721: return("Opar");
    case 731: return("Pieskové/Prašné víry");
    case 741: return("Hmla");
    case 751: return("Piesok");
    case 761: return("Prach");
    case 762: return("Sopečný popol");
    case 771: return("Búrka");
    case 781: return("Tornádo");
    case 800: return("Jasná obloha");
    case 801: return("Takmer jasno");
    case 802: return("Polojasno");
    case 803: return("Oblačno");
    case 804: return("Zamračené");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == SL)
  switch (id) {
    case 200: return("Nevihta z rahlim dežjem");
    case 201: return("Nevihta z dežjem");
    case 202: return("Nevihta z močnim dežjem");
    case 210: return("Manjša nevihta");
    case 211: return("Nevihta");
    case 212: return("Močna nevihta");
    case 221: return("Občasna nevihta");
    case 230: return("Nevihta z rahlim rošenjem");
    case 231: return("Nevihta z rošenjem");
    case 232: return("Nevihta z močnim rošenjem");
    case 300: return("Rahlo rošenje");
    case 301: return("Rošenje");
    case 302: return("Rošenje močnejše intenzitete");
    case 310: return("Pršenje šibkejše intenzitete");
    case 311: return("Pršenje šibkejše intenzitete");
    case 312: return("Pršenje močnejše intenzitete");
    case 313: return("Pršenje šibkejše intenzitete");
    case 314: return("Pršenje in močnejši dež");
    case 321: return("Pršenje in dež");
    case 500: return("Rahel dež");
    case 501: return("Dež");
    case 502: return("Močan dež");
    case 503: return("Zelo močan dež");
    case 504: return("Ekstremni dež");
    case 511: return("Dež ki zmrzuje");
    case 520: return("Dež šibkejše intenzitete");
    case 521: return("Dež");
    case 522: return("Dež močnejše intenzitete");
    case 531: return("Občasni dež");
    case 600: return("Rahlo sneženje");
    case 601: return("Sneženje");
    case 602: return("Močno sneženje");
    case 611: return("Žled");
    case 612: return("Ledeni dež");
    case 615: return("Rahel dež s snegom");
    case 616: return("Dež s snegom");
    case 620: return("Rahlo sneženje");
    case 621: return("Sneg");
    case 622: return("Močno sneženje");
    case 701: return("Megla");
    case 711: return("Meglica");
    case 721: return("Para");
    case 731: return("Pesek, vrtinčenje prahu");
    case 741: return("Megla");
    case 751: return("Pesek");
    case 761: return("Prah");
    case 762: return("Vulkanski pepel");
    case 771: return("Sunki");
    case 781: return("Tornado");
    case 800: return("Jasno");
    case 801: return("Pretrgana oblačnost");
    case 802: return("Delno oblačno");
    case 803: return("Pretežno oblačno");
    case 804: return("Oblačno");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == SP)
  switch (id) {
    case 200: return("Tormenta con lluvia ligera");
    case 201: return("Tormenta con lluvia");
    case 202: return("Tormenta con lluvia intensa");
    case 210: return("Ligera tormenta");
    case 211: return("Tormenta");
    case 212: return("Fuerte tormenta");
    case 221: return("Tormenta irregular");
    case 230: return("Tormenta con llovizna ligera");
    case 231: return("Tormenta con llovizna");
    case 232: return("Tormenta con llovizna intensa");
    case 300: return("Llovizna ligera");
    case 301: return("Llovizna");
    case 302: return("Llovizna de gran intensidad");
    case 310: return("Lluvia y llovizna ligera");
    case 311: return("Lluvia y llovizna");
    case 312: return("Lluvia y llovizna de gran intensidad");
    case 313: return("Spew lluvia y llovizna");
    case 314: return("Lluvia y lluvia fuerte");
    case 321: return("Chubasco");
    case 500: return("Lluvia ligera");
    case 501: return("Lluvia moderada");
    case 502: return("Lluvia de gran intensidad");
    case 503: return("Lluvia muy fuerte");
    case 504: return("Lluvia muy fuerte");
    case 511: return("Lluvia helada");
    case 520: return("Chubasco de ligera intensidad");
    case 521: return("Chubasco");
    case 522: return("Chubasco de gran intensidad");
    case 531: return("Chubascos ocasionales");
    case 600: return("Nevada ligera");
    case 601: return("Nieve");
    case 602: return("Nevada intensa");
    case 611: return("Aguanieve");
    case 612: return("Lluvia de aguanieve");
    case 620: return("Lluvia ligera");
    case 621: return("Chubasco de nieve");
    case 622: return("Fuerte lluvia de nieve");
    case 701: return("Niebla");
    case 711: return("Humo");
    case 721: return("Niebla");
    case 731: return("Torbellinos de arena/polvo");
    case 741: return("Bruma");
    case 751: return("Arena");
    case 761: return("Polvo");
    case 762: return("Ceniza volcánica");
    case 771: return("Ráfagas");
    case 781: return("Tornado");
    case 800: return("Cielo claro");
    case 801: return("Algo de nubes");
    case 802: return("Nubes dispersas");
    case 803: return("Nubes rotas");
    case 804: return("Nubes");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == SR)
  switch (id) {
    case 200: return("Грмљавина са слабом кишом");
    case 201: return("Грмљавина са кишом");
    case 202: return("Грмљавина са јаком кишом");
    case 210: return("Слаба грмљавина");
    case 211: return("Грмљавина");
    case 212: return("Јака грмљавина");
    case 221: return("Изузетно јака грмљавина");
    case 230: return("Грмљавина са слабим ромињањем");
    case 231: return("Грмљавина са ромињањем");
    case 232: return("Грмљавина са јаким ромињањем");
    case 300: return("Ромињање слабог интензитета");
    case 301: return("Ромињање");
    case 302: return("Ромињање јаког интензитета");
    case 310: return("Слабо ромињање кише");
    case 311: return("Ромињање кише");
    case 312: return("Јако ромињање кише");
    case 313: return("Пљусак и ромињање");
    case 314: return("Јак пљусак и ромињање");
    case 321: return("Јако ромињање");
    case 500: return("Кишица");
    case 501: return("Умерена киша");
    case 502: return("Киша јаког интензитета");
    case 503: return("Киша веома јаког интензитета");
    case 504: return("Снажна киша");
    case 511: return("Ледена киша");
    case 520: return("Слаб краткотрајни пљусак");
    case 521: return("Пљусак");
    case 522: return("Јак краткотрајни пљусак");
    case 531: return("Изузетно јак пљусак");
    case 600: return("Слаб снег");
    case 601: return("Снег");
    case 602: return("Јак снег");
    case 611: return("Суснежица");
    case 612: return("Налет суснежице");
    case 615: return("Слаба киша и снег");
    case 616: return("Киша и снег");
    case 620: return("Слабе снежне падавине");
    case 621: return("Снежне падавине");
    case 622: return("Јаке снежне падавине");
    case 701: return("Сумаглица");
    case 711: return("Смог");
    case 721: return("Измаглица");
    case 731: return("Песак, вртлози прашине");
    case 741: return("Магла");
    case 751: return("Песак");
    case 761: return("Прашина");
    case 762: return("Вулкански пепео");
    case 771: return("Олуја");
    case 781: return("Торнадо");
    case 800: return("Ведро небо");
    case 801: return("Местимични облаци");
    case 802: return("Разбацани облаци");
    case 803: return("Испрекидани облаци");
    case 804: return("Ниски облаци");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == TH)
  switch (id) {
    case 200: return("พายุฟ้าคะนองพร้อมฝนเล็กน้อย");
    case 201: return("พายุฟ้าคะนองพร้อมฝน");
    case 202: return("พายุฟ้าคะนองพร้อมฝนหนัก");
    case 210: return("พายุฟ้าคะนองเล็กน้อย");
    case 211: return("พายุฟ้าคะนอง");
    case 212: return("พายุฟ้าคะนองหนัก");
    case 221: return("พายุฟ้าคะนองเป็นช่วง ๆ");
    case 230: return("พายุฟ้าคะนองพร้อมฝนปรอยเล็กน้อย");
    case 231: return("พายุฟ้าคะนองพร้อมฝนปรอย");
    case 232: return("พายุฟ้าคะนองพร้อมฝนปรอยหนัก");
    case 300: return("ฝนปรอยแบบไม่แรง");
    case 301: return("ฝนปรอย");
    case 302: return("ฝนปรอยแบบแรง");
    case 310: return("ฝนพร้อมฝนปรอยแบบไม่แรง");
    case 311: return("ฝนพร้อมฝนปรอย");
    case 312: return("ฝนพร้อมฝนปรอยแบบแรง");
    case 313: return("ฝนซู่และฝนปรอย");
    case 314: return("ฝนซู่และฝนปรอยหนัก");
    case 321: return("ฝนปรอยแบบฝนซู่");
    case 500: return("ฝนเบา ๆ");
    case 501: return("ฝนปานกลาง");
    case 502: return("ฝนแบบแรง");
    case 503: return("ฝนตกหนักมาก");
    case 504: return("ฝนตกขั้นรุนแรง");
    case 511: return("ฝนจับตัวเป็นน้ำแข็ง");
    case 520: return("ฝนซู่แบบไม่แรง");
    case 521: return("ฝนซู่");
    case 522: return("ฝนซู่แบบแรง");
    case 531: return("ฝนซู่เป็นช่วง ๆ");
    case 600: return("หิมะเล็กน้อย");
    case 601: return("หิมะ");
    case 602: return("หิมะตกหนัก");
    case 611: return("ฝนลูกเห็บ");
    case 612: return("ฝนลูกเห็บแบบฝนซู่");
    case 615: return("ฝนและหิมะเล็กน้อย");
    case 616: return("ฝนและหิมะ");
    case 620: return("หิมะเล็กน้อยช่วงสั้น ๆ");
    case 621: return("หิมะตกช่วงสั้น ๆ");
    case 622: return("หิมะตกหนักช่วงสั้น ๆ");
    case 701: return("หมอก");
    case 711: return("ควัน");
    case 721: return("เมฆหมอก");
    case 731: return("ทราย ลมฝุ่นหมุน");
    case 741: return("หมอก");
    case 751: return("ทราย");
    case 761: return("ฝุ่น");
    case 762: return("เถ้าภูเขาไฟ");
    case 771: return("ลมสควอลล์");
    case 781: return("ทอร์นาโด");
    case 800: return("ท้องฟ้าแจ่มใส");
    case 801: return("เมฆเล็กน้อย");
    case 802: return("เมฆกระจาย");
    case 803: return("เมฆเป็นหย่อม ๆ");
    case 804: return("เมฆเต็มท้องฟ้า");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == TR)
  switch (id) {
    case 200: return("Gök gürültülü hafif yağmurlu");
    case 201: return("Gök gürültülü yağmurlu");
    case 202: return("Gök gürültülü sağanak yağışlı");
    case 210: return("Hafif sağanak");
    case 211: return("Sağanak");
    case 212: return("Şiddetli sağanak");
    case 221: return("Aralıklı sağanak");
    case 230: return("Gök gürültülü hafif yağışlı");
    case 231: return("Gök gürültülü yağışlı");
    case 232: return("Gök gürültülü şiddetli yağışlı");
    case 300: return("Yer yer hafif yoğunluklu yağış");
    case 301: return("Yer yer yağışlı");
    case 302: return("Yer yer yoğun yağışlı");
    case 310: return("Yer yer hafif yağışlı");
    case 311: return("Yer yer yağışlı");
    case 312: return("Yer yer yoğun yağışlı");
    case 313: return("Yağmur ve çiselemek");
    case 314: return("Yoğun yağışlı duş ve yağmur");
    case 321: return("Yer yer sağanak yağışlı");
    case 500: return("Hafif yağmur");
    case 501: return("Orta şiddetli yağmur");
    case 502: return("Şiddetli yağmur");
    case 503: return("Çok şiddetli yağmur");
    case 504: return("Aşırı yağmur");
    case 511: return("Yağışlı ve soğuk hava");
    case 520: return("Kısa süreli hafif yoğunluklu yağmur");
    case 521: return("Kısa süreli yağmur");
    case 522: return("Kısa süreli şiddetli yağmur");
    case 531: return("Ara sıra duşlar");
    case 600: return("Hafif kar yağışlı");
    case 601: return("Kar yağışlı");
    case 602: return("Yoğun kar yağışlı");
    case 611: return("Karla karışık yağmurlu");
    case 612: return("Karışık duş");
    case 620: return("Hafif kar duşu");
    case 621: return("Kısa sürelü kar yağışı");
    case 622: return("Ağır kar duşu");
    case 701: return("Sisli");
    case 711: return("Sisli");
    case 721: return("Hafif sisli");
    case 731: return("Kum/Toz fırtınası");
    case 741: return("Sisli");
    case 751: return("Kum");
    case 761: return("Toz");
    case 762: return("Volkanik kül");
    case 771: return("Feryat figan");
    case 781: return("Kasırga");
    case 800: return("Açık");
    case 801: return("Az bulutlu");
    case 802: return("Parçalı az bulutlu");
    case 803: return("Parçalı bulutlu");
    case 804: return("Kapalı");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == UA)
  switch (id) {
    case 200: return("Гроза та легкий дощ");
    case 201: return("Гроза та дощ");
    case 202: return("Гроза та злива");
    case 210: return("Легка гроза");
    case 211: return("Гроза");
    case 212: return("Сильна гроза");
    case 221: return("Короткочасна гроза");
    case 230: return("Гроза та легка мряка");
    case 231: return("Гроза та мряка");
    case 232: return("Гроза та сильна мряка");
    case 300: return("Слабка мряка");
    case 301: return("Мряка");
    case 302: return("Сильна мряка");
    case 310: return("Легка мряка та дощ");
    case 311: return("Мряка та дощ");
    case 312: return("Сильна мряка та дощ");
    case 313: return("Проливний дощ та мряка");
    case 314: return("Сильний проливний дощ та мряка");
    case 321: return("Дощова мряка");
    case 500: return("Легкий дощ");
    case 501: return("Помірний дощ");
    case 502: return("Сильний дощ");
    case 503: return("Дуже сильний дощ");
    case 504: return("Надзвичайно сильний дощ");
    case 511: return("Переохолоджений дощ");
    case 520: return("Легка злива");
    case 521: return("Злива");
    case 522: return("Сильна злива");
    case 531: return("Короткочасна злива");
    case 600: return("Легкий сніг");
    case 601: return("Сніг");
    case 602: return("Сильний сніг");
    case 611: return("Мокрий сніг");
    case 612: return("Мокра хуртовина");
    case 615: return("Легкий дощ та сніг");
    case 616: return("Дощ та сніг");
    case 620: return("Легка хуртовина");
    case 621: return("Хуртовина");
    case 622: return("Сильна хуртовина");
    case 701: return("Димка");
    case 711: return("Дим");
    case 721: return("Імла");
    case 731: return("Пісок, пилові бурі");
    case 741: return("Туман");
    case 751: return("Пісок");
    case 761: return("Пил");
    case 762: return("Вулканічний попіл");
    case 771: return("Шквальний вітер");
    case 781: return("Смерч");
    case 800: return("Чисте небо");
    case 801: return("Кілька хмар");
    case 802: return("Уривчасті хмари");
    case 803: return("Рвані хмари");
    case 804: return("Хмарно");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == VI)
  switch (id) {
    case 200: return("Giông bão và Mưa nhẹ");
    case 201: return("Giông bão và Mưa");
    case 202: return("Giông bão Mưa lớn");
    case 210: return("Giông bão có chớp giật");
    case 211: return("Bão");
    case 212: return("Giông bão lớn");
    case 221: return("Bão vài nơi");
    case 230: return("Giông bão và Mưa phùn nhẹ");
    case 231: return("Giông bão với mưa phùn");
    case 232: return("Giông bão với mưa phùn nặng");
    case 300: return("Ánh sáng cường độ mưa phùn");
    case 301: return("Mưa phùn");
    case 302: return("Mưa phùn cường độ nặng");
    case 310: return("Mưa phùn nhẹ");
    case 311: return("Mưa và mưa phùn");
    case 312: return("Mưa và mưa phùn nặng");
    case 313: return("Mưa mưa và mưa phùn");
    case 314: return("Mưa mưa và mưa phùn");
    case 321: return("Mưa rào và mưa phùn");
    case 500: return("Mưa nhẹ");
    case 501: return("Mưa vừa");
    case 502: return("Mưa cường độ nặng");
    case 503: return("Mưa rất nặng");
    case 504: return("Mưa lốc");
    case 511: return("Mưa lạnh");
    case 520: return("Mưa rào nhẹ");
    case 521: return("Mưa rào");
    case 522: return("Mưa rào cường độ nặng");
    case 531: return("Thỉnh thoảng tắm");
    case 600: return("Tuyết rơi nhẹ");
    case 601: return("Tuyết");
    case 602: return("Tuyết nặng hạt");
    case 611: return("Mưa đá");
    case 612: return("Tắm mưa");
    case 620: return("Ánh sáng tuyết tắm");
    case 621: return("Tuyết mù trời");
    case 622: return("Mưa tuyết nặng");
    case 701: return("Sương mờ");
    case 711: return("Khói bụi");
    case 721: return("Đám mây");
    case 731: return("Bão cát và lốc xoáy");
    case 741: return("Sương mù");
    case 751: return("Cát");
    case 761: return("Bụi");
    case 762: return("Tro núi lửa");
    case 771: return("Các vụ xáo trộn");
    case 781: return("Cơn lốc xoáy");
    case 800: return("Bầu trời quang đãng");
    case 801: return("Mây thưa");
    case 802: return("Mây rải rác");
    case 803: return("Mây cụm");
    case 804: return("Mây đen u ám");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == ZH_CN)
  switch (id) {
    case 200: return("雷阵雨");
    case 201: return("雷阵雨");
    case 202: return("雷阵雨");
    case 210: return("雷阵雨");
    case 211: return("雷阵雨");
    case 212: return("雷阵雨");
    case 221: return("雷阵雨");
    case 230: return("雷阵雨");
    case 231: return("雷阵雨");
    case 232: return("雷阵雨");
    case 300: return("小雨");
    case 301: return("小雨");
    case 302: return("大雨");
    case 310: return("小雨");
    case 311: return("小雨");
    case 312: return("大雨");
    case 313: return("淋雨和毛毛雨");
    case 314: return("大雨淋雨和毛毛雨");
    case 321: return("阵雨");
    case 500: return("小雨");
    case 501: return("中雨");
    case 502: return("大雨");
    case 503: return("大雨");
    case 504: return("暴雨");
    case 511: return("冻雨");
    case 520: return("阵雨");
    case 521: return("阵雨");
    case 522: return("大雨");
    case 531: return("偶尔淋浴");
    case 600: return("小雪");
    case 601: return("雪");
    case 602: return("大雪");
    case 611: return("雨夹雪");
    case 612: return("雨淋淋浴");
    case 620: return("轻雪淋浴");
    case 621: return("阵雪");
    case 622: return("大雪淋浴");
    case 701: return("薄雾");
    case 711: return("烟雾");
    case 721: return("薄雾");
    case 731: return("沙旋风");
    case 741: return("大雾");
    case 751: return("沙子");
    case 761: return("灰尘");
    case 762: return("火山灰");
    case 771: return("哗啦啦");
    case 781: return("龙卷风");
    case 800: return("晴");
    case 801: return("晴，少云");
    case 802: return("多云");
    case 803: return("多云");
    case 804: return("阴，多云");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == ZH_TW)
  switch (id) {
    case 200: return("雷陣雨");
    case 201: return("雷陣雨");
    case 202: return("雷陣雨");
    case 210: return("雷陣雨");
    case 211: return("雷陣雨");
    case 212: return("雷陣雨");
    case 221: return("雷陣雨");
    case 230: return("雷陣雨");
    case 231: return("雷陣雨");
    case 232: return("雷陣雨");
    case 300: return("小雨");
    case 301: return("小雨");
    case 302: return("大雨");
    case 310: return("小雨");
    case 311: return("小雨");
    case 312: return("大雨");
    case 313: return("淋雨和毛毛雨");
    case 314: return("大雨淋雨和毛毛雨");
    case 321: return("陣雨");
    case 500: return("小雨");
    case 501: return("中雨");
    case 502: return("大雨");
    case 503: return("大雨");
    case 504: return("暴雨");
    case 511: return("凍雨");
    case 520: return("陣雨");
    case 521: return("陣雨");
    case 522: return("大雨");
    case 531: return("偶尔淋浴");
    case 600: return("小雪");
    case 601: return("雪");
    case 602: return("大雪");
    case 611: return("雨夾雪");
    case 612: return("雨淋淋浴");
    case 620: return("輕雪淋浴");
    case 621: return("陣雪");
    case 622: return("大雪淋浴");
    case 701: return("薄霧");
    case 711: return("煙霧");
    case 721: return("薄霧");
    case 731: return("沙旋風");
    case 741: return("大霧");
    case 751: return("沙子");
    case 761: return("灰塵");
    case 762: return("火山灰");
    case 771: return("嘩啦啦");
    case 781: return("龍捲風");
    case 800: return("晴");
    case 801: return("晴，少雲");
    case 802: return("多雲");
    case 803: return("多雲");
    case 804: return("陰，多雲");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == ZU)
  switch (id) {
    case 200: return("Ukuduma okuhambisana nemikhemezelo");
    case 201: return("Ukuduma okuhambisana nemvula");
    case 202: return("Ukuduma okuhambisana nemvula enamandla");
    case 210: return("Ukuduma okungatheni");
    case 211: return("Ukuduma");
    case 212: return("Ukuduma okunamandla");
    case 221: return("Ukuduma okuhamba kubuye");
    case 230: return("Ukuduma okuqeqebulayo");
    case 231: return("Ukuduma okuhambisana namaconsana");
    case 232: return("Ukuduma okuhambisana namaconsi abanzi");
    case 300: return("Amathe ezimpukane");
    case 301: return("Amaconsana");
    case 302: return("Amaconsi anamandlanyana");
    case 310: return("Amathe ezimpukane nemvudlana");
    case 311: return("Imikhizwana");
    case 312: return("Amaconsi anamandlanyana nemvudlana");
    case 313: return("Amathe ezimpukane nemikhizwana");
    case 314: return("Imvula enamandla nemikhemezelo");
    case 321: return("Imvudlana engatheni");
    case 500: return("Imvudlana");
    case 501: return("Imvula ekahle");
    case 502: return("Imvula enamandla");
    case 503: return("Imvula enkulu kakhulu");
    case 504: return("Imvula enamdla amakhulu");
    case 511: return("Imvula ebanda kakhulu");
    case 520: return("Imvula encane");
    case 521: return("Imikhemezelo");
    case 522: return("Imvula enamandla kakhulu");
    case 531: return("Imikhemezelo ebhedayo");
    case 600: return("Isithwathwa");
    case 601: return("Yiqhwa");
    case 602: return("Umkhithiko onamandla");
    case 611: return("Imikhithikwana");
    case 612: return("Imikhithiko emincane");
    case 615: return("Emvudlana nomkhithiko");
    case 616: return("Imvula nomkhithiko");
    case 620: return("Imikhithiko engenamandla");
    case 621: return("Imikhithiko engelutho");
    case 622: return("Imikhithiko enamandla");
    case 701: return("Inkungu");
    case 711: return("Intuthu");
    case 721: return("Ihezi");
    case 731: return("Inhlabathi, izintuli");
    case 741: return("Ifogi");
    case 751: return("Inhlabathi");
    case 761: return("Izintuli");
    case 762: return("Umlotha wentabamlilo");
    case 771: return("Isiphepho");
    case 781: return("Ithonado");
    case 800: return("Isibhakabhaka esingenalutho");
    case 801: return("Gqwagqwa ngamafu");
    case 802: return("Amafu lapho nalapho");
    case 803: return("Amafu angenamandla");
    case 804: return("Liguqubele");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == SQ)
  switch (id) {
    case 200: return("Stuhi me shi të lehtë");
    case 201: return("Stuhi me shi");
    case 202: return("Stuhi me shi të madh");
    case 210: return("Stuhi e lehtë");
    case 211: return("Stuhi");
    case 212: return("Stuhi e rëndë");
    case 221: return("Stuhi e rrëmbyeshme");
    case 230: return("Stuhi me rrufe të lehta");
    case 231: return("Stuhi me rrufe");
    case 232: return("Stuhi me reshje të medha");
    case 300: return("Rrufe e intezitetit të lehte");
    case 301: return("Rrufe");
    case 302: return("Shi me rrufe të medha");
    case 310: return("Shi me rrufe");
    case 311: return("Shi me rrufe");
    case 312: return("Shi i me rrufe të intensitet të lartë");
    case 313: return("Shiu me rrufe");
    case 314: return("Shi i intezitetit të larte me rrufe");
    case 321: return("Rrufe");
    case 500: return("Shi i lehtë");
    case 501: return("Shi mesatar");
    case 502: return("Shi me intensitet të lartë");
    case 503: return("Shi shumë i madh");
    case 504: return("Shi ekstrem");
    case 511: return("Shi me ngrica");
    case 520: return("Shi me intensitet të lehtë");
    case 521: return("Rrebesh shiu");
    case 522: return("Shi i intezitetit të lehtë");
    case 531: return("Shi i dendur");
    case 600: return("Borë e lehtë");
    case 601: return("Borë");
    case 602: return("Borë e rëndë");
    case 611: return("Borë me shi");
    case 612: return("Bresher");
    case 615: return("Shi i lehtë dhe borë");
    case 616: return("Shi dhe borë");
    case 620: return("Bororë me shi");
    case 621: return("Borë");
    case 622: return("Borë e intezitetit të lartë");
    case 701: return("Mjegull");
    case 711: return("Mjegull");
    case 721: return("Mjegull e dendur");
    case 731: return("Pluhur, vorbulla pluhuri");
    case 741: return("Mjegull");
    case 751: return("Rërë");
    case 761: return("Pluhur");
    case 762: return("Hir vullkanik");
    case 771: return("Shtrëngatë");
    case 781: return("Tornado");
    case 800: return("I kthjellët");
    case 801: return("Pjesërisht me re");
    case 802: return("Re të shpërndara");
    case 803: return("I vrenjtur");
    case 804: return("Të mbuluar me re");
    default:  return("Unknown (" + String(id) + ")");
  }
 #elif (LANGUAGE == BE)
  switch (id) {
    case 200: return("Навальніца з невялікім дажджом");
    case 201: return("Навальніца з дажджом");
    case 202: return("Навальніца з моцным дажджом");
    case 210: return("Невялікая навальніца");
    case 211: return("Навальніца");
    case 212: return("Моцная навальніца");
    case 221: return("Навальніца");
    case 230: return("Навальніца з невялікай імглой");
    case 231: return("Навальніца з імглой");
    case 232: return("Навальніца з моцнай імглой");
    case 300: return("Невялікая імгла");
    case 301: return("Лёгкі дождж, імгла");
    case 302: return("Кароткачасовы дождж");
    case 310: return("Невялікая імгла з дажджом");
    case 311: return("Лёгкі дождж");
    case 312: return("Лёгкі інтэнсіўны дождж");
    case 313: return("Кароткачасовы дождж з імглой");
    case 314: return("Моцны кароткачасовы дождж з імглой");
    case 321: return("Кароткачасовая імгла");
    case 500: return("Лёгкі дождж");
    case 501: return("Умераны дождж");
    case 502: return("Моцны дождж");
    case 503: return("Вельмі моцны дождж");
    case 504: return("Лівень");
    case 511: return("Дождж з градам");
    case 520: return("Лёгкі кароткачасовы дождж");
    case 521: return("Кароткачасовы дождж");
    case 522: return("Моцны кароткачасовы дождж");
    case 531: return("Кароткачасовыя дажджы");
    case 600: return("Невялікі снег");
    case 601: return("Снег");
    case 602: return("Моцны снег");
    case 611: return("Мокры снег");
    case 612: return("Кароткачасовы мокры снег");
    case 615: return("Лёгкі снег з дажджом");
    case 616: return("Снег з дажджом");
    case 620: return("Лёгкі кароткачасовы снег");
    case 621: return("Кароткачасовы снег");
    case 622: return("Моцны кароткачасовы снег");
    case 701: return("Туман");
    case 711: return("Дым");
    case 721: return("Лёгкі туман");
    case 731: return("Віхор пяску і пылу");
    case 741: return("Туман");
    case 751: return("Пясок");
    case 761: return("Пыл");
    case 762: return("Вулканічны попел");
    case 771: return("Шквалы");
    case 781: return("Тарнада");
    case 800: return("Ясна");
    case 801: return("Невялікая воблачнасць");
    case 802: return("Воблачна");
    case 803: return("Пераменная воблачнасць");
    case 804: return("Пахмурна");
    default:  return("Unknown (" + String(id) + ")");
  }
 #else
  return("Your language isn't supported. Get more info in Notes.ino");
 #endif
}