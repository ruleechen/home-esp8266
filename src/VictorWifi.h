#ifndef VictorWifi_h
#define VictorWifi_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Console.h"
#include "AppStorage.h"
#include "BuiltinLed.h"

namespace Victor::Components {
  class VictorWifi {
   public:
    void setup();
    void reset();
    void join(String ssid, String password = emptyString, int32_t channel = 0, uint8_t* bssid = nullptr);
    bool isConnected();
    void waitForConnected();
    String getHostId();
    String getHostName();
    String getApName();

   private:
    WiFiEventHandler _gotIPHandler = nullptr;
    WiFiEventHandler _disconnectedHandler = nullptr;
    void _handleStationModeGotIP(const WiFiEventStationModeGotIP& args);
    void _handleStationModeDisconnected(const WiFiEventStationModeDisconnected& args);
    static Console _log();
  };

  // global
  extern VictorWifi victorWifi;

} // namespace Victor::Components

#endif // VictorWifi_h
