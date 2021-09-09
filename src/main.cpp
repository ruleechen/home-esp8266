#include <Arduino.h>

#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "VictorWeb.h"
#include "Timer.h"

using namespace Victor;
using namespace Victor::Events;
using namespace Victor::Components;

BuiltinLed* builtinLed;
VictorWeb webPortal(80);

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error(F("LittleFS mount failed"));
  }

  builtinLed = new BuiltinLed();
  builtinLed->turnOn();

  VictorOTA::setup();
  VictorWifi::setup();

  webPortal.onRequestStart = []() { builtinLed->turnOn(); };
  webPortal.onRequestEnd = []() { builtinLed->turnOff(); };
  webPortal.setup();

  timer.setInterval(1 * 1000, []() { console.log("heartbeat"); });

  console.log(F("setup complete"));
  builtinLed->flash();
}

void loop(void) {
  timer.loop();
  webPortal.loop();
}
