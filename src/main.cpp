#include <Arduino.h>

#include "Console.h"
#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "VictorRadio.h"
#include "VictorWeb.h"

using namespace Victor;
using namespace Victor::Components;

VictorWeb webPortal(80);
VictorRadio radioPortal;

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error(F("fs mount failed"));
  }

  builtinLed.setup();
  builtinLed.turnOn();

  // setup radio
  radioPortal.onEmit = [](const RadioEmit& emit) {
    builtinLed.flash();
    console.log()
      .bracket(F("radio"))
      .section(F("sent"), emit.value)
      .section(F("via channel"), String(emit.channel));
  };

  // setup web
  webPortal.onRequestStart = []() { builtinLed.toggle(); };
  webPortal.onRequestEnd = []() { builtinLed.toggle(); };
  webPortal.onRadioEmit = [](const int index) { radioPortal.emit(index); };
  webPortal.onServiceGet = [](std::vector<KeyValueModel>& items) {
    items.push_back({ .key = "key1", .value = "value1" });
  };
  webPortal.onServicePost = [](const String type) {
    console.log().bracket("service").section("post", type);
  };
  webPortal.setup();

  // setup wifi
  victorOTA.setup();
  victorWifi.setup();

  // done
  console.log(F("setup complete"));
}

void loop(void) {
  webPortal.loop();
  // loop radio
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
