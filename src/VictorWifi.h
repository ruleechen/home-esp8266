#ifndef VictorWifi_h
#define VictorWifi_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Commons.h"
#include "AppStorage.h"

namespace Victor::Components {
  class VictorWifi {
   public:
    static void setup();
    static void reset();
    static void join(String ssid, String password = emptyString, int32_t channel = 0, uint8_t* bssid = NULL);
    static String getHostId();
    static String getLocalHostName();
    static String getHostName(bool includeVersion = true);

   private:
    static Console _log();
    static void _onWifiEvent(WiFiEvent_t event);
  };
} // namespace Victor::Components

#endif // VictorWifi_h
