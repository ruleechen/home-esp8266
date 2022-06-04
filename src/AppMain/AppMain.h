#ifndef AppMain_h
#define AppMain_h

#include "Console.h"
#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "VictorWeb.h"
#include "VictorRadio.h"

namespace Victor::Components {

  struct AppFeatures {
    bool web = true;
    bool radio = false;
  };

  class AppMain {
   public:
    AppMain();
    ~AppMain();
    void setup(AppFeatures features);
    void loop(int8_t sleepMode = 0);
    VictorWeb* webPortal = nullptr;
    VictorRadio* radioPortal = nullptr;

   private:
    uint16_t _sleepMillis = 0;
  };

} // namespace Victor::Components

#endif // AppMain_h
