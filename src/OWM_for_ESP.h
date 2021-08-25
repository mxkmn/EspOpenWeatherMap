#pragma once

// Defining available units
#define STANDART 0 // Kelvin units and meters/sec
#define METRIC 1   // Celsius units and meters/sec
#define IMPERIAL 2 // Fahrenheit units and miles/hour

// Defining available APIs
#define FULL 0    // https://openweathermap.org/api/one-call-api
#define CURRENT 1 // https://openweathermap.org/current

// Defining fully supported languages
#define EN 11   // English
#define RU 33   // Russian

// Defining partial supported languages (getShortStatus() in English, getStatus16() is not supported, translations in getStatus() are provided by OWM and may be strange)
#define AF 1     // Africans
#define AR 2     // Arabic
#define AZ 3     // Azerbaijani
#define BE 4     // Belarussian
#define BG 5     // Bulgarian
#define CA 6     // Catalan
#define CZ 7     // Czech
#define DA 8     // Danish
#define DE 9     // German
#define EL 10    // Greek
#define EU 12    // Basque
#define FA 13    // Persian (Farsi)
#define FI 14    // Finnish
#define FR 15    // French
#define GL 16    // Galician
#define HE 17    // Hebrew
#define HI 18    // Hindi
#define HR 19    // Croatian
#define HU 20    // Hungarian
#define ID 21    // Indonesian
#define IT 22    // Italian
#define JA 23    // Japanese
#define KR 24    // Korean
#define LA 25    // Latvian
#define LT 26    // Lithuanian
#define MK 27    // Macedonian
#define NO 28    // Norwegian Bokmål
#define NL 29    // Dutch
#define PL 30    // Polish
#define PT 31    // Portuguese
#define PT_BR 32 // Português Brasil
#define SE 34    // Swedish
#define SK 35    // Slovak
#define SL 36    // Slovenian
#define SP 37    // Spanish
#define SQ 38    // Albanian
#define SR 39    // Serbian
#define TH 40    // Thai
#define TR 41    // Turkish
#define UA 42    // Ukrainian
#define VI 43    // Vietnamese
#define ZH_CN 44 // Chinese Simplified
#define ZH_TW 45 // Chinese Traditional
#define ZU 46    // Zulu


#include "User_Setup.h"
#include "Data_Point_Set.h"


class OWM_Weather: public JsonListener {
  public:
    OWM_Weather(String apiKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", uint8_t units = DEFAULT_UNITS); // Class constructor

    // Sketch should call this forecast requests, they return true if no parse errors encountered
    bool getWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily, String latitude, String longitude, uint8_t type); // Setups all weather requests
    bool getCurrentWeather(OWM_current *current, String latitude, String longitude);                                              // Setups the current weather request
    bool getFullWeather(OWM_current *current, OWM_hourly *hourly, OWM_daily *daily, String latitude, String longitude);           // Setups the full weather request

    String getIconCode(uint8_t iconId, String begin = "", String end = ""); // Returns icon code

    String getShortStatus(uint16_t id); // Returns main status description
    String getStatus(uint16_t id);      // Returns full status description
    String getStatus16(uint16_t id);    // Returns full status description, limited to 16 characters (optimized for displays)

    void setApiKey(String apiKey); // Sets _apiKey
    void setUnits(uint8_t units);  // Sets _units



  private:
    bool getAndParseData(String url); // Sends a GET request and parses response, returns true if no parse errors

    uint8_t getIconId(String iconCode); // Returns icon ID

    // Printing methods
    void printlnError(String msg);
    void printTime(String msg);
    void printUnnecessary(String msg);
    void printlnHeader(String msg);
    void printJsonChar(char c);
    void printCallback(String msg);

    // Streaming parser callback methods, allow tracking and decisions. Connected with JSON_Decoder via .setListener(this);
    void value(const char *value); // Inserts the parsed data in the structures for sketch access

    void key(const char *key); // Inserts current key to _currentKey to grab from .value()

    void error(const char *message); // Sends error message to serial port

    void startDocument(); // JSON document has started, typically starts once
                          // Initialises variables used, e.g. sets objectLayer = 0 and _arrayIndex =0
    void endDocument();   // JSON document has ended, typically ends once

    void startObject();   // Called every time an Object start detected
                          // May be called multiple times as object layers entered
                          // Used to increment objectLayer
    void endObject();     // Called every time an object ends
                          // Used to decrement objectLayer and zero _arrayIndex

    void startArray();    // An array of name:value pairs entered
    void endArray();      // Array member ended, increments _arrayIndex



    // Variables
    String _apiKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // Stores user API key
    uint8_t _units; // Stores user units number
    uint8_t _type; // Current API type for .value()

    // The value storage structures are created and deleted by the sketch and a pointer passed via the
    // library getWeather() call the value() function is then used to populate the structs with values
    OWM_current  *current;   // pointer provided by sketch to the OWM_current struct
    OWM_hourly   *hourly;    // pointer provided by sketch to the OWM_hourly struct
    OWM_daily    *daily;     // pointer provided by sketch to the OWM_daily struct

    bool _parseOk;           // true if the parsing is completed

    uint16_t _objectLevel;   // Object level, increments for new object, decrements at end
    uint16_t _arrayIndex;    // Array index e.g. 5 for day 5 forecast
    uint16_t _arrayLevel;    // Array level
    String   _currentParent; // Current object e.g. "daily"
    String   _currentKey;    // Name key of the name:value pair e.g "temperature"
    String   _currentSet;    // Name key of the data set
};