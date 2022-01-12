#include <Arduino.h>

#include "Console.h"
#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "WebPortal.h"

using namespace Victor;
using namespace Victor::Events;
using namespace Victor::Components;

WebPortal webPortal(80);

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error(F("fs mount failed"));
  }

  builtinLed.setup();
  builtinLed.turnOn();

  webPortal.onRequestStart = []() { builtinLed.toggle(); };
  webPortal.onRequestEnd = []() { builtinLed.toggle(); };
  webPortal.setup();

  victorOTA.setup();
  victorWifi.setup();

  console.log(F("setup complete"));
}

void loop(void) {
  webPortal.loop();
}
