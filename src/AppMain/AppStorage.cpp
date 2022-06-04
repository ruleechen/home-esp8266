#include "AppStorage.h"

namespace Victor::Components {

  AppStorage::AppStorage(const char* filePath) : FileStorage(filePath) {
    _maxSize = 128;
    _enableCache = false;
  }

  void AppStorage::_serializeTo(const AppSetting& model, DynamicJsonDocument& doc) {
    doc[F("name")] = model.name;
    doc[F("sleep")] = model.sleepMillis;
    const auto ledArr = doc.createNestedArray(F("led"));
    ledArr[0] = model.led.pin;
    ledArr[1] = model.led.onHigh ? 1 : 0;
    ledArr[2] = model.led.enabled ? 1 : 0;
  }

  void AppStorage::_deserializeFrom(AppSetting& model, const DynamicJsonDocument& doc) {
    const char* name = doc[F("name")];
    model.name = String(name);
    model.sleepMillis = doc[F("sleep")];
    const auto ledArr = doc[F("led")];
    model.led = {
      .pin = ledArr[0],
      .onHigh = ledArr[1] == 1,
      .enabled = ledArr[2] == 1,
    };
  }

  // global
  AppStorage appStorage;

} // namespace Victor::Components
