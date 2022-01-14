#ifndef VictorOTA_h
#define VictorOTA_h

#include <functional>
#include <Arduino.h>
#include <ESP8266httpUpdate.h>
#include "Console.h"

namespace Victor::Components {

  enum VOtaType {
    VOta_All = 0,
    VOta_Sketch = 1,
    VOta_FileSystem = 2,
  };

  class VictorOTA {
   public:
    void setup();
    String getCurrentVersion() const;
    String checkNewVersion();
    void update(const String& version, VOtaType type);
    void trigger(VOtaType type);

   private:
    void _updateSketch();
    void _updateFileSystem();
    void _handleStart();
    void _handleEnd();
    void _handleProgress(int progress, int total);
    void _handleError(int error);
    static Console _log();
  };

  // global
  extern VictorOTA victorOTA;

} // namespace Victor::Components

#endif // VictorOTA_h
