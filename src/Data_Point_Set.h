// The structures below are the repository for the data values extracted from the JSON message.
// The structures are populated with the extracted data by the .value() method in the OWM_for_ESP.cpp file.

// The content is 255 or "" when first created.

typedef struct OWM_current { // Structure for current weather

  float    lat = 255;
  float    lon = 255;
  uint32_t cityId = 255; // Only Current API
 #ifdef ENABLE_STRINGS
  String   cityName; // Only Current API
  String   countryCode; // Only Current API
  String   timezoneName; // Only One Call API
 #endif

  uint32_t timezoneOffset = 255;
  uint32_t serverUpdateTime = 255;
  uint32_t sunriseTime = 255;
  uint32_t sunsetTime = 255;

  float    mainTemp = 255;
  float    feelsLikeTemp = 255;
  float    minTemp = 255; // Only Current API
  float    maxTemp = 255; // Only Current API

  uint8_t  humidity = 255;
  uint16_t pressureSeaLevel = 255;
  uint16_t pressureGroundLevel = 255; // Only Current API
  float    windSpeed = 255;
  float    windGust = 255;
  uint16_t windDirection = 255;

  float    dewPoint = 255; // Only One Call API
  float    rainForLast1h = 0; // Sometimes OWM doesn't send this data because it is not raining
  float    snowForLast1h = 0; // Sometimes OWM doesn't send this data because it is not snowing
  float    rainForLast3h = 255; // Only Current API
  float    snowForLast3h = 255; // Only Current API

  uint8_t  cloudiness = 255;
  uint32_t visibility = 255;
  float    ultravioletIndex = 255; // Only One Call API

  uint16_t weatherCondId = 255;
  uint8_t  weatherIconId = 255;

} OWM_current;

typedef struct OWM_hourly { // Structure for hourly weather

  uint32_t time[MAX_HOURS] = { 255 };

  float    mainTemp[MAX_HOURS] = { 255 };
  float    feelsLikeTemp[MAX_HOURS] = { 255 };

  uint8_t  humidity[MAX_HOURS] = { 255 };
  uint16_t pressureSeaLevel[MAX_HOURS] = { 255 };
  float    windSpeed[MAX_HOURS] = { 255 };
  float    windGust[MAX_HOURS] = { 255 };
  uint16_t windDirection[MAX_HOURS] = { 255 };

  float    dewPoint[MAX_HOURS] = { 255 };
  float    rainForLast1h[MAX_HOURS] = { 0 }; // Sometimes OWM doesn't send this data because it is not raining
  float    snowForLast1h[MAX_HOURS] = { 0 }; // Sometimes OWM doesn't send this data because it is not snowing

  uint8_t  cloudiness[MAX_HOURS] = { 255 };
  uint16_t visibility[MAX_HOURS] = { 255 };
  float    ultravioletIndex[MAX_HOURS] = { 255 };
  float    pop[MAX_HOURS];

  uint16_t weatherCondId[MAX_HOURS] = { 255 };
  uint8_t  weatherIconId[MAX_HOURS] = { 255 };

} OWM_hourly;

typedef struct OWM_daily { // Structure for daily weather

  uint32_t time[MAX_DAYS] = { 255 };
  uint32_t sunriseTime[MAX_DAYS] = { 255 };
  uint32_t sunsetTime[MAX_DAYS] = { 255 };
  uint32_t moonriseTime[MAX_DAYS] = { 255 };
  uint32_t moonsetTime[MAX_DAYS] = { 255 };

  float    minTemp[MAX_DAYS] = { 255 };
  float    maxTemp[MAX_DAYS] = { 255 };

  float    morningTemp[MAX_DAYS] = { 255 };
  float    dayTemp[MAX_DAYS] = { 255 };
  float    eveningTemp[MAX_DAYS] = { 255 };
  float    nightTemp[MAX_DAYS] = { 255 };

  float    morningFeelsLikeTemp[MAX_DAYS] = { 255 };
  float    dayFeelsLikeTemp[MAX_DAYS] = { 255 };
  float    eveningFeelsLikeTemp[MAX_DAYS] = { 255 };
  float    nightFeelsLikeTemp[MAX_DAYS] = { 255 };

  uint8_t  humidity[MAX_DAYS] = { 255 };
  uint16_t pressureSeaLevel[MAX_DAYS] = { 255 };
  float    windSpeed[MAX_DAYS] = { 255 };
  float    windGust[MAX_DAYS] = { 255 };
  uint16_t windDirection[MAX_DAYS] = { 255 };

  float    dewPoint[MAX_DAYS] = { 255 };
  float    rain[MAX_DAYS] = { 0 }; // Sometimes OWM doesn't send this data because it is not raining
  float    snow[MAX_DAYS] = { 0 }; // Sometimes OWM doesn't send this data because it is not snowing

  uint8_t  cloudiness[MAX_DAYS] = { 255 };
  float    ultravioletIndex[MAX_DAYS] = { 255 };
  float    pop[MAX_DAYS];

  float    moonPhase[MAX_DAYS] = { 255 };

  uint16_t weatherCondId[MAX_DAYS] = { 255 };
  uint8_t  weatherIconId[MAX_DAYS] = { 255 };

} OWM_daily;