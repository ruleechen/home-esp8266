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
    static void join(String ssid, String password, bool waitForConnecting = true);
    static String getHostId();
    static String getHostName(bool includeVersion = false);

   private:
    static void _onWifiEvent(WiFiEvent_t event);
  };
} // namespace Victor::Components

#endif // VictorWifi_h
