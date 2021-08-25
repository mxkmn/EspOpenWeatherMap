/*
  [type] Supported types of API is CURRENT and FULL
    CURRENT https://openweathermap.org/weather-data
    FULL https://openweathermap.org/api/one-call-api



  [units] STANDART, METRIC and IMPERIAL units are available
    STANDART uses Kelvin units and meters/sec. It's default units for OWM and is not written in the GET request to speed up data sending
    METRIC uses Celsius units and meters/sec
    IMPERIAL uses Fahrenheit units and miles/hour



  [language] Returns summary properties in the desired language by .getShortStatus(), .getStatus() and .getStatus16()
    Fully supported languages are:
      EN: English
      RU: Russian

    Partial supported languages (getShortStatus in English, getStatus16 is not supported, translations in getStatus are provided by OWM and may be strange) are:
      AF: Afrikaans
      AR: Arabic
      AZ: Azerbaijani
      BE: Belarussian
      BG: Bulgarian
      CA: Catalan
      CZ: Czech
      DA: Danish
      DE: German
      EL: Greek
      EU: Basque
      FA: Persian (Farsi)
      FI: Finnish
      FR: French
      GL: Galician
      HE: Hebrew
      HI: Hindi
      HR: Croatian
      HU: Hungarian
      ID: Indonesian
      IT: Italian
      JA: Japanese
      KR: Korean
      LA: Latvian
      LT: Lithuanian
      MK: Macedonian
      NO: Norwegian
      NL: Dutch
      PL: Polish
      PT: Portuguese
      PT_BR: Português Brasil
      SE: Swedish
      SK: Slovak
      SL: Slovenian
      SP: Spanish
      SQ: Albanian
      SR: Serbian
      TH: Thai
      TR: Turkish
      UA: Ukrainian
      VI: Vietnamese
      ZH_CN: Chinese Simplified
      ZH_TW: Chinese Traditional
      ZU: Zulu
*/