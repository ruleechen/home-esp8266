#include <Arduino.h>

#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "WebPortal.h"

using namespace Victor;
using namespace Victor::Events;
using namespace Victor::Components;

BuiltinLed* builtinLed;
WebPortal webPortal(80);

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error(F("fs mount failed"));
  }

  builtinLed = new BuiltinLed();
  builtinLed->turnOn();

  victorOTA.setup();
  victorWifi.setup();

  webPortal.onRequestStart = []() { builtinLed->turnOn(); };
  webPortal.onRequestEnd = []() { builtinLed->turnOff(); };
  webPortal.setup();

  console.log(F("setup complete"));
  builtinLed->flash();
}

void loop(void) {
  webPortal.loop();
}
