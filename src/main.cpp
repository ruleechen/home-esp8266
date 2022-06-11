#include <Arduino.h>

#include "AppMain/AppMain.h"
// #include "Button/DigitalInputButton.h"
#include "Button/DigitalInterruptButton.h"
#include "Timer/IntervalOverAuto.h"

using namespace Victor;
using namespace Victor::Components;

AppMain* appMain;
// DigitalInputButton* button;
DigitalInterruptButton* button;
IntervalOverAuto twoSeconds(2000);

void setup(void) {
  appMain = new AppMain();
  appMain->setup();

  // setup radio
  appMain->radioPortal->onEmit = [](const RadioEmit& emit) {
    builtinLed.flash();
    console.log()
      .bracket(F("radio"))
      .section(F("sent"), emit.value)
      .section(F("via channel"), String(emit.channel));
  };

  // setup web
  appMain->webPortal->onServiceGet = [](std::vector<TextValueModel>& states, std::vector<TextValueModel>& buttons) {
    states.push_back({  .text = F("Text1"),   .value = F("value1") });
    buttons.push_back({ .text = F("Button1"), .value = F("action1") });
  };
  appMain->webPortal->onServicePost = [](const String& value) {
    console.log()
      .bracket(F("service"))
      .section(F("post"), value);
  };

  // input button
  // button = new DigitalInputButton(0, 0);
  button = new DigitalInterruptButton(0, 0);
  button->onAction = [](const ButtonAction action) {
    console.log()
      .bracket(F("button"))
      .section(F("action"), String(action));
    if (action == BUTTON_ACTION_PRESSED) {
      builtinLed.flash();
    } else if (action == BUTTON_ACTION_DOUBLE_PRESSED) {
      builtinLed.flash(500);
      const auto enable = victorWifi.isLightSleepMode();
      victorWifi.enableAP(enable); // toggle enabling ap
    } else if (action == BUTTON_ACTION_RESTART) {
      ESP.restart();
    } else if (action == BUTTON_ACTION_RESTORE) {
      ESP.eraseConfig();
      ESP.restart();
    }
  };

  // done
  console.log()
    .bracket(F("setup"))
    .section(F("complete"));
}

void loop(void) {
  if (twoSeconds.isOver()) {
    console.log("over");
  }
  appMain->loop();
  button->loop();
  // loop radio
  if (false) {
    auto value = String(F(""));
    uint8_t channel = 1;
    appMain->radioPortal->receive(value, channel);
    builtinLed.flash();
    console.log()
      .bracket(F("radio"))
      .section(F("received"), value)
      .section(F("from channel"), String(channel));
  }
}
