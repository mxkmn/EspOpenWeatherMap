// Configuration settings

// These parameters set the data point count stored in program memory (not the datapoint count
// sent by the server). So they determine the memory used during collection of the data points.

#define MAX_HOURS 48 // Set "hourly" forecast period, can be set from 1 to 48
#define MAX_DAYS 8 // Set "daily" forecast periods, can be set from 1 to 8 (Today + 7 days = 8 maximum)

#define LANGUAGE EN // Set your language. Check available languages in /examples/OWM_for_ESP_Test/Notes.ino
#define DEFAULT_UNITS METRIC // Set default units. They can be changed with .setUnits(). Check available units in /examples/OWM_for_ESP_Test/Notes.ino

#define SECURE_CONNECTION // Define if you want to use HTTPS instead of HTTP for security (3x slower)
#define ENABLE_STRINGS // Disable to save more RAM



// These parameters affect the information output
#define LOG_ERRORS // Define if you want to check serial messages of errors
//#define LOG_UNNECESSARY_INFO // Define if you want to check serial messages of progress and URLs
//#define LOG_TIME // Define if you want to know the time of receiving the weather

//#define SHOW_JSON     // Debug only - simple serial output formatting of whole JSON message
//#define SHOW_HEADER   // Debug only - for checking response header via serial message
//#define SHOW_CALLBACK // Debug only to show the JSON decode tree



// ###############################################################################
// DO NOT tinker below, this is configuration checking that helps stop crashes:
// ###############################################################################

// Checking and correcting bad settings
#if MAX_HOURS > 48
  #undef  MAX_HOURS
  #define MAX_HOURS 48
#endif
#if MAX_HOURS < 1
  #undef  MAX_HOURS
  #define MAX_HOURS 1
#endif

#if MAX_DAYS > 8
  #undef  MAX_DAYS
  #define MAX_DAYS 8
#endif
#if MAX_DAYS < 1
  #undef  MAX_DAYS
  #define MAX_DAYS 1
#endif