#include "AppMain.h"

namespace Victor::Components {

  AppMain::AppMain() {}

  AppMain::~AppMain() {
    if (webPortal != nullptr) {
      delete webPortal;
      webPortal = nullptr;
    }
    if (radioPortal != nullptr) {
      delete radioPortal;
      radioPortal = nullptr;
    }
  }

  void AppMain::setup(AppFeatures features) {
    console.begin(115200);

    if (!LittleFS.begin()) {
      console.error()
        .bracket(F("fs"))
        .section(F("mount failed"));
    }

    const auto appSetting = appStorage.load();
    _sleepMillis = appSetting.sleepMillis;

    builtinLed.setup(appSetting.led);
    builtinLed.turnOn();

    victorOTA.setup("/ota.json");
    victorWifi.setup("/wifi.json");

    if (features.web) {
      webPortal = new VictorWeb(80);
      webPortal->onRequestStart = []() { builtinLed.toggle(); };
      webPortal->onRequestEnd = []() { builtinLed.toggle(); };
      webPortal->onRadioEmit = [&](uint8_t index) { radioPortal->emit(index); };
      webPortal->onPageData = [&](DynamicJsonDocument& res) { res[F("hasRadio")] = (radioPortal != nullptr); };
      webPortal->setup();
    }

    if (features.radio) {
      radioPortal = new VictorRadio();
    }
  }

  void AppMain::loop(int8_t sleepMode) {
    if (webPortal != nullptr) {
      webPortal->loop();
    }
    // sleep
    if (sleepMode == -1) {
      sleepMode = victorWifi.isLightSleepMode();
    }
    if (sleepMode && _sleepMillis > 0) {
      delay(_sleepMillis);
    }
  }

} // namespace Victor::Components
