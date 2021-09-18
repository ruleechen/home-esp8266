#ifndef GlobalHelpers_h
#define GlobalHelpers_h

#include <vector>
#include <Arduino.h>

namespace Victor {

  class GlobalHelpers {
   public:
    static String randomString(int length) {
      auto result = String(F(""));
      int generated = 0;
      while (generated < length) {
        // .platformio/packages/framework-arduinoespressif8266/cores/esp8266/WMath.cpp
        byte randomValue = random(0, 26);
        char letter = randomValue + 'a';
        if (randomValue > 26) {
          letter = (randomValue - 26);
        }
        result += letter;
        generated++;
      }
      return result;
    }

    static std::vector<String> splitString(const String& str, const String& delimiter) {
      std::vector<String> parts;
      auto delimiterLength = delimiter.length();
      auto searchFrom = 0;
      while (true) {
        auto index = str.indexOf(delimiter, searchFrom);
        if (index == -1) {
          parts.push_back(str.substring(searchFrom));
          break;
        }
        parts.push_back(str.substring(searchFrom, index));
        searchFrom = index + delimiterLength;
      }
      return parts;
    }

  };

} // namespace Victor

#endif // GlobalHelpers_h
