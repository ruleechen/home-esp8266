#ifndef VictorWifi_h
#define VictorWifi_h

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include "Commons.h"
#include "AppStorage.h"

namespace Victor::Components {
  class VictorWifi {
   public:
    void setup();
    void reset();
    void join(String ssid, String password = emptyString, int32_t channel = 0, uint8_t* bssid = NULL);
    String getHostId();
    String getLocalHostName();
    String getHostName(bool includeVersion = true);

   private:
    Console _log();
    void _handleStationModeGotIP(const WiFiEventStationModeGotIP& args);
    void _handleStationModeDisconnected(const WiFiEventStationModeDisconnected& args);
  };

  // global
  extern VictorWifi victorWifi;

} // namespace Victor::Components

#endif // VictorWifi_h
