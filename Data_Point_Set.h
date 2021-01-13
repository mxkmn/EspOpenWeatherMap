// The structures below are the repository for the data values extracted from the
// JSON message. The structures are popolated with the extracted data by the "value()"
// member function in the main OWM_for_ESP.cpp file.

// The content is 255 or "" when first created.

typedef struct OWM_current { // Structure for current weather

  // current
  float    lat = 255;
  float    lon = 255;
  uint32_t dt = 255;
  uint32_t timezone_offset = 255;
  uint32_t sunrise = 255;
  uint32_t sunset = 255;
  float    temp = 255;
  float    feels_like = 255;
  uint16_t pressure = 255;
  uint8_t  humidity = 255;
  uint8_t  clouds = 255;
  uint32_t visibility = 255;
  float    wind_speed = 255;
  float    wind_gust = 255;
  uint16_t wind_deg = 255;
  float    rain_1h = 255;
  float    snow_1h = 255;
  uint16_t id = 255;
  String   main;
  String   description;
  String   icon;

  // only onecall API
  String   timezone = "";
  float    dew_point = 255;
  float    uvi = 255;

  // only current API
  uint32_t city_id = 255;
  String   city_name;
  String   country;
  float    rain_3h = 255;
  float    snow_3h = 255;
  float    temp_min = 255;
  float    temp_max = 255;
  uint16_t sea_level = 255;
  uint16_t grnd_level = 255;

} OWM_current;

typedef struct OWM_hourly { // Structure for hourly weather

  // hourly
  uint32_t dt[MAX_HOURS] = { 255 };
  float    temp[MAX_HOURS] = { 255 };
  float    feels_like[MAX_HOURS] = { 255 };
  float    pressure[MAX_HOURS] = { 255 };
  uint8_t  humidity[MAX_HOURS] = { 255 };
  float    dew_point[MAX_HOURS] = { 255 };
  uint8_t  clouds[MAX_HOURS] = { 255 };
  float    wind_speed[MAX_HOURS] = { 255 };
  float    wind_gust[MAX_HOURS] = { 255 };
  uint16_t wind_deg[MAX_HOURS] = { 255 };
  float    rain[MAX_HOURS] = { 255 };
  float    snow[MAX_HOURS] = { 255 };

  // hourly.weather
  uint16_t id[MAX_HOURS] = { 255 };
  String   main[MAX_HOURS];
  String   description[MAX_HOURS];
  String   icon[MAX_HOURS];
  float    pop[MAX_HOURS];
} OWM_hourly;

typedef struct OWM_daily { // Structure for daily weather

  // daily
  uint32_t dt[MAX_DAYS] = { 255 };  // dt
  uint32_t sunrise[MAX_DAYS] = { 255 };
  uint32_t sunset[MAX_DAYS] = { 255 };
  
  // daily.temp
  float    temp_morn[MAX_DAYS] = { 255 };
  float    temp_day[MAX_DAYS] = { 255 };
  float    temp_eve[MAX_DAYS] = { 255 };
  float    temp_night[MAX_DAYS] = { 255 };
  float    temp_min[MAX_DAYS] = { 255 };
  float    temp_max[MAX_DAYS] = { 255 };

  // daily.feels_like
  float    feels_like_morn[MAX_DAYS] = { 255 };
  float    feels_like_day[MAX_DAYS] = { 255 };
  float    feels_like_eve[MAX_DAYS] = { 255 };
  float    feels_like_night[MAX_DAYS] = { 255 };

  // daily
  float    pressure[MAX_DAYS] = { 255 };
  uint8_t  humidity[MAX_DAYS] = { 255 };
  float    dew_point[MAX_DAYS] = { 255 };
  float    wind_speed[MAX_DAYS] = { 255 };
  float    wind_gust[MAX_DAYS] = { 255 };
  uint16_t wind_deg[MAX_DAYS] = { 255 };
  uint8_t  clouds[MAX_DAYS] = { 255 };
  float    uvi[MAX_DAYS] = { 255 };
  uint32_t visibility[MAX_DAYS] = { 255 };

  float    rain[MAX_DAYS] = { 255 };
  float    snow[MAX_DAYS] = { 255 };

  // daily.weather
  uint16_t id[MAX_DAYS] = { 255 };
  String   main[MAX_DAYS];
  String   description[MAX_DAYS];
  String   icon[MAX_DAYS];
  float    pop[MAX_DAYS];

} OWM_daily;