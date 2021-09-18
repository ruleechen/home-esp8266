#include <Arduino.h>
#include <Ticker.h>

#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "VictorWeb.h"

using namespace Victor;
using namespace Victor::Components;

Ticker ticker;
BuiltinLed* builtinLed;
VictorWeb webPortal(80);

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error(F("[LittleFS] mount failed"));
  }

  builtinLed = new BuiltinLed();
  builtinLed->turnOn();

  VictorOTA::setup();
  VictorWifi::setup();

  webPortal.onRequestStart = []() { builtinLed->turnOn(); };
  webPortal.onRequestEnd = []() { builtinLed->turnOff(); };
  webPortal.setup();

  ticker.attach(1, []() { console.log("[ticker] heartbeat"); });

  console.log(F("[setup] complete"));
  builtinLed->flash();
}

void loop(void) {
  webPortal.loop();
}
