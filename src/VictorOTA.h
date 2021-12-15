#ifndef VictorOTA_h
#define VictorOTA_h

#include <functional>
#include <Arduino.h>
#include <ESP8266httpUpdate.h>
#include "Commons.h"

namespace Victor::Components {
  class VictorOTA {
   public:
    void setup();
    String getCurrentVersion();
    String checkNewVersion();
    void update(String version, VOtaType type);
    void trigger(VOtaType type);

   private:
    Console _log();
    void _updateSketch();
    void _updateFileSystem();
    void _handleStart();
    void _handleEnd();
    void _handleProgress(int progress, int total);
    void _handleError(int error);
  };

  // global
  extern VictorOTA victorOTA;

} // namespace Victor::Components

#endif // VictorOTA_h
