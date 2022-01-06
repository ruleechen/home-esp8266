#include <Arduino.h>

#include "Console.h"
#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "VictorRadio.h"
#include "VictorWeb.h"

using namespace Victor;
using namespace Victor::Components;

VictorRadio radioPortal;
VictorWeb webPortal(80);

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error(F("fs mount failed"));
  }

  builtinLed.setup();
  builtinLed.turnOn();

  radioPortal.onEmit = [](const RadioEmit& emit) {
    builtinLed.flash();
    // emit via your radio tool
    console.log()
      .bracket(F("radio"))
      .section(F("sent"), emit.value)
      .section(F("via channel"), String(emit.channel));
  };

  webPortal.onRequestStart = []() { builtinLed.turnOn(); };
  webPortal.onRequestEnd = []() { builtinLed.turnOff(); };
  webPortal.onRadioEmit = [](int index) { radioPortal.emit(index); };
  webPortal.setup();

  victorOTA.setup();
  victorWifi.setup();

  console.log(F("setup complete"));
}

void loop(void) {
  webPortal.loop();
  // receive from your radio tool
  if (false) {
    String value = "";
    int channel = 1;
    radioPortal.receive(value, channel);
    builtinLed.flash();
    console.log()
      .bracket(F("radio"))
      .section(F("received"), value)
      .section(F("from channel"), String(channel));
  }
}
