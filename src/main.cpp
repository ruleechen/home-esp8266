#include <Arduino.h>
#include <Ticker.h>

#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "VictorRadio.h"
#include "VictorWeb.h"

using namespace Victor;
using namespace Victor::Components;

Ticker ticker;
BuiltinLed* builtinLed;
VictorWeb webPortal(80);
VictorRadio radioPortal(&ticker);

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error(F("[LittleFS] mount failed"));
  }

  builtinLed = new BuiltinLed();
  builtinLed->turnOn();

  victorOTA.setup();
  victorWifi.setup();

  radioPortal.onEmit = [](const RadioEmit& emit) {
    builtinLed->flash();
    console.log().type(F("Radio"))
      .write(F(" sent [")).write(emit.value).write(F("]"))
      .write(F(" via channel [")).write(String(emit.channel)).write(F("]")).newline();
  };

  webPortal.onRequestStart = []() { builtinLed->turnOn(); };
  webPortal.onRequestEnd = []() { builtinLed->turnOff(); };
  webPortal.onRadioEmit = [](int index) { radioPortal.emit(index); };
  webPortal.setup();

  ticker.attach(10, []() { console.log("[ticker] heartbeat"); });

  console.log(F("[setup] complete"));
  builtinLed->flash();
}

void loop(void) {
  webPortal.loop();
  if (false) {
    String value = "";
    int channel = 1;
    radioPortal.receive(value, channel);
    console.log().type(F("Radio"))
      .write(F(" received [")).write(value).write(F("]"))
      .write(F(" from channel [")).write(String(channel)).write(F("]")).newline();
    builtinLed->flash();
  }
}
