#include <vector>
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
    console.error(F("fs mount failed"));
  }

  builtinLed = new BuiltinLed();
  builtinLed->turnOn();

  // setup radio
  radioPortal.onEmit = [](const RadioEmit& emit) {
    builtinLed->flash();
    console.log().bracket(F("radio"))
      .write(F(" sent ")).bracket(emit.value)
      .write(F(" via channel ")).bracket(String(emit.channel));
  };

  // setup web
  webPortal.onRequestStart = []() { builtinLed->turnOn(); };
  webPortal.onRequestEnd = []() { builtinLed->turnOff(); };
  webPortal.onRadioEmit = [](int index) { radioPortal.emit(index); };
  webPortal.onResetService = []() { console.log("reset service"); };
  webPortal.onGetServiceState = [](std::vector<KeyValueModel>& items) {
    items.push_back({ .key = "key1", .value = "value1" });
  };
  webPortal.setup();

  ticker.attach(10, []() {
    // console.log("heartbeat");
  });

  // setup wifi
  victorOTA.setup();
  victorWifi.setup();

  // done
  builtinLed->flash();
  console.log(F("setup complete"));
  while (!WiFi.isConnected()) {
    builtinLed->flash();
    delay(50);
  }
}

void loop(void) {
  webPortal.loop();
  if (false) {
    String value = "";
    int channel = 1;
    radioPortal.receive(value, channel);
    console.log().bracket(F("radio"))
      .write(F(" received ")).bracket(value)
      .write(F(" from channel ")).bracket(String(channel));
    builtinLed->flash();
  }
}
