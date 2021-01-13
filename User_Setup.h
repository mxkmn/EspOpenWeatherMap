// Configuration settings

// These parameters set the data point count stored in program memory (not the datapoint count
// sent by the server). So they determine the memory used during collection of the data points.
#define MAX_HOURS 48   // "hourly" forecast period, can be set from 1 to 48
#define MAX_DAYS 8     // "daily" forecast periods can be set from 1 to 8 (Today + 7 days = 8 maximum)


// These parameters affect the information output
#define LOG_ERRORS // Define if you want to check serial messages of errors
#define LOG_TIME // Define if you want to know the time of receiving the weather
#define LOG_UNNECESSARY_INFO // Define if you want to check serial messages of progress and URLs
//#define SHOW_HEADER   // Debug only - for checking response header via serial message
//#define SHOW_JSON     // Debug only - simple serial output formatting of whole JSON message
//#define SHOW_CALLBACK // Debug only to show the decode tree

//#define SECURE_CONNECTION // Define if you want use HTTPS instead of HTTP for security (3x slower)

// ###############################################################################
// DO NOT tinker below, this is configuration checking that helps stop crashes:
// ###############################################################################

// Check and correct bad settings
#if (MAX_HOURS > 48) || (MAX_HOURS < 1)
  #undef  MAX_HOURS
  #define MAX_HOURS 48 // Ignore compiler warning!
#endif

#if (MAX_DAYS > 8) || (MAX_DAYS < 1)
  #undef  MAX_DAYS
  #define MAX_DAYS 8  // Ignore compiler warning!
#endif