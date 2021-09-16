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

    static String padLeft(int num, int length) {
      auto str = String(num);
      while (str.length() < length) {
        str = "0" + str;
      }
      return str;
    }

    static String timeSince(unsigned long timestamp) {
      float s = (millis() - timestamp) / 1000;
      float timespan;
      String date = "";
      String time = "";
      // y
      int years = 0;
      auto oneYear = 31536000;
      timespan = s / oneYear;
      if (timespan > 1) {
        years = floor(timespan);
        s = s - years * oneYear;
      }
      date += padLeft(years, 4);
      // m
      int months = 0;
      auto oneMonth = 2592000;
      timespan = s / oneMonth;
      if (timespan > 1) {
        months = floor(timespan);
        s = s - months * oneMonth;
      }
      date += F("/") + padLeft(months, 2);
      // d
      int days = 0;
      auto oneDay = 86400;
      timespan = s / oneDay;
      if (timespan > 1) {
        days = floor(timespan);
        s = s - days * oneDay;
      }
      date += F("/") + padLeft(days, 2);
      // h
      int hours = 0;
      auto oneHour = 3600;
      timespan = s / oneHour;
      if (timespan > 1) {
        hours = floor(timespan);
        s = s - hours * oneHour;
      }
      time += padLeft(hours, 2);
      // m
      int minutes = 0;
      auto oneMinute = 60;
      timespan = s / oneMinute;
      if (timespan > 1) {
        minutes = floor(timespan);
        s = s - minutes * oneMinute;
      }
      time += F(":") + padLeft(minutes, 2);
      // s
      int seconds = floor(s);
      time += F(":") + padLeft(seconds, 2);
      // ret
      return (years > 0 || months > 0 || days > 0)
        ? date + F(" ") + time
        : time;
    }

    static unsigned char h2int(char c) {
      if (c >= '0' && c <= '9') {
        return ((unsigned char)c - '0');
      }
      if (c >= 'a' && c <= 'f') {
        return ((unsigned char)c - 'a' + 10);
      }
      if (c >= 'A' && c <= 'F') {
        return ((unsigned char)c - 'A' + 10);
      }
      return (0);
    }

    static String urlDecode(const String& str) {
      auto encodedString = String(F(""));
      char c;
      char code0;
      char code1;
      for (unsigned int i = 0; i < str.length(); i++) {
        c = str.charAt(i);
        if (c == '+') {
          encodedString += ' ';
        } else if (c == '%') {
          i++;
          code0 = str.charAt(i);
          i++;
          code1 = str.charAt(i);
          c = (h2int(code0) << 4) | h2int(code1);
          encodedString += c;
        } else {
          encodedString += c;
        }
        yield();
      }
      return encodedString;
    }

    static String urlEncode(const String& str) {
      auto encodedString = String(F(""));
      char c;
      char code0;
      char code1;
      // char code2;
      for (unsigned int i = 0; i < str.length(); i++) {
        c = str.charAt(i);
        if (c == ' ') {
          encodedString += '+';
        } else if (isalnum(c)) {
          encodedString += c;
        } else {
          code1 = (c & 0xf) + '0';
          if ((c & 0xf) > 9) {
            code1 = (c & 0xf) - 10 + 'A';
          }
          c = (c >> 4) & 0xf;
          code0 = c + '0';
          if (c > 9) {
            code0 = c - 10 + 'A';
          }
          // code2 = '\0';
          encodedString += '%';
          encodedString += code0;
          encodedString += code1;
          // encodedString += code2;
        }
        yield();
      }
      return encodedString;
    }
  };

} // namespace Victor

#endif // GlobalHelpers_h
