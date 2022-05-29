#include <Arduino.h>

#include "Console.h"
#include "BuiltinLed.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "VictorRadio.h"
#include "VictorWeb.h"
#include "Button/DigitalInputButton.h"
#include "Button/DigitalInterruptButton.h"

using namespace Victor;
using namespace Victor::Components;

VictorWeb webPortal(80);
VictorRadio radioPortal;
DigitalInputButton* button;
// DigitalInterruptButton* button;

bool debugEnabled = false;

void setup(void) {
  console.begin(115200);
  if (!LittleFS.begin()) {
    console.error()
      .bracket(F("fs"))
      .section(F("mount failed"));
  }

  builtinLed.setup();
  builtinLed.turnOn();
  victorOTA.setup("/ota.json");
  victorWifi.setup("/wifi.json");

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
  webPortal.onRadioEmit = [](uint8_t index) { radioPortal.emit(index); };
  webPortal.onServiceGet = [](std::vector<TextValueModel>& states, std::vector<TextValueModel>& buttons) {
    states.push_back({  .text = F("Text1"),   .value = F("value1") });
    buttons.push_back({ .text = F("Button1"), .value = F("action1") });
  };
  webPortal.onServicePost = [](const String& value) {
    console.log()
      .bracket(F("service"))
      .section(F("post"), value);
  };
  webPortal.setup();

  // input button
  button = new DigitalInputButton(0, 0);
  // button = new DigitalInterruptButton(0, 0);
  button->onAction = [](const ButtonAction action) {
    if (action == ButtonActionPressed) {
      builtinLed.flash();
    } else if (action == ButtonActionDoublePressed) {
      builtinLed.flash(500);
      debugEnabled = !debugEnabled;
      victorWifi.enableAP(debugEnabled);
    } else if (action == ButtonActionRestart) {
      ESP.restart();
    } else if (action == ButtonActionRestore) {
      ESP.eraseConfig();
      ESP.restart();
    }
    console.log()
      .bracket(F("button"))
      .section(F("action"), String(action));
  };

  // done
  console.log()
    .bracket(F("setup"))
    .section(F("complete"));
}

void loop(void) {
  webPortal.loop();
  button->loop();
  // loop radio
  if (false) {
    auto value = String(F(""));
    uint8_t channel = 1;
    radioPortal.receive(value, channel);
    builtinLed.flash();
    console.log()
      .bracket(F("radio"))
      .section(F("received"), value)
      .section(F("from channel"), String(channel));
  }
}
