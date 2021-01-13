#pragma once

#include "User_Setup.h"
#include "Data_Point_Set.h"

/***************************************************************************************
** Description:   JSON interface class
***************************************************************************************/
class OWM_Weather: public JsonListener {
  public:
    // Sketch calls this forecast request, it returns true if no parse errors encountered
    bool getWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily,
                     String api_key, String latitude, String longitude,
                     String units = "metric", String language = "en", String type = "full");

    bool getCurrentWeather(OWM_current *current,
                     String api_key, String latitude, String longitude,
                     String units = "metric", String language = "en");

    bool getFullWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily,
                     String api_key, String latitude, String longitude,
                     String units = "metric", String language = "en");

    bool parseRequest(String url); // Called by library, sends a GET request and
                                   // parses response, returns true if no parse errors

  private: // Streaming parser callback functions, allow tracking and decisions

    void startDocument(); // JSON document has started, typically starts once
                          // Initialises variables used, e.g. sets objectLayer = 0
                          // and arrayIndex =0
    void endDocument();   // JSON document has ended, typically ends once

    void startObject();   // Called every time an Object start detected
                          // may be called multiple times as object layers entered
                          // Used to increment objectLayer
    void endObject();     // Called every time an object ends
                          // Used to decrement objectLayer and zero arrayIndex


    void startArray();    // An array of name:value pairs entered
    void endArray();      // Array member ended, increments arrayIndex

    void key(const char *key);            // The current "object" or "name for a name:value pair"
    void value(const char *value);        // String value from name:value pair e.g. "1.23" or "rain"

    void whitespace(char c);              // Whitespace character in JSON - not used

    void error( const char *message );    // Error message is sent to serial port

    void fullDataSet(const char *value);    // Populate structure with full data set
    void partialDataSet(const char *value); // Populate structure with minimal data set

  private: // Variables used internal to library

    uint8_t api_id;          // ID of API type (1 is One Call API, 2 is Current weather data API)

    // The value storage structures are created and deleted by the sketch and
    // a pointer passed via the library getWeather() call the value() function
    // is then used to populate the structs with values
    OWM_current  *current;  // pointer provided by sketch to the OWM_current struct
    OWM_hourly   *hourly;   // pointer provided by sketch to the OWM_hourly struct
    OWM_daily    *daily;    // pointer provided by sketch to the OWM_daily struct

    String      valuePath;  // object (i.e. sequential key) path (like a "file path")
                            // taken to the name:value pair in the form "hourly/data"
                            // so values can be pulled from the correct array.
                            // Needed since different objects contain "data" arrays.

    String data_set;        // A copy of the last object name at the head of an array
                            // short equivalent to path.

    bool     parseOK;       // true if the parse been completed
                            // (does not mean data values gathered are good!)

    bool     partialSet = false;    // Set true for partial data set acquisition

    String   currentParent; // Current object e.g. "daily"
    uint16_t objectLevel;   // Object level, increments for new object, decrements at end
    String   currentKey;    // Name key of the name:value pair e.g "temperature"
    String   currentSet;    // Name key of the data set
    String   arrayPath;     // Path to name:value pair e.g.  "daily/data"
    uint16_t arrayIndex;    // Array index e.g. 5 for day 5 forecast, qualify with arrayPath
    uint16_t arrayLevel;    // Array level
};