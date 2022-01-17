#ifndef VictorWifi_h
#define VictorWifi_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "Console.h"
#include "AppStorage.h"
#include "BuiltinLed.h"

namespace Victor::Components {
  class VictorWifi {
   public:
    void setup();
    void reset();
    WiFiMode_t getMode() const;
    void setMode(WiFiMode_t mode);
    static String modeName(WiFiMode_t mode);
    void join(const String& ssid, const String& password = emptyString, int32_t channel = 0, uint8_t* bssid = nullptr);
    bool isConnected() const;
    bool isMDNSRunning() const;
    void waitForConnected();
    String getHostId();
    String getHostName();

   private:
    WiFiEventHandler _gotIPHandler = nullptr;
    WiFiEventHandler _disconnectedHandler = nullptr;
    void _handleStaGotIP(const WiFiEventStationModeGotIP& args);
    void _handleStaDisconnected(const WiFiEventStationModeDisconnected& args);
    static Console _log();
  };

  // global
  extern VictorWifi victorWifi;

} // namespace Victor::Components

#endif // VictorWifi_h
