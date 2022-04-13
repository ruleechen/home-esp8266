#include "AppStorage.h"

namespace Victor::Components {

  AppStorage::AppStorage() {
    _filePath = "/app.json";
    _maxSize = 256;
    _enableCache = false;
  }

  void AppStorage::_serializeTo(const AppModel& model, DynamicJsonDocument& doc) {
    doc[F("name")] = model.name;
    doc[F("led")][0] = model.ledPin;
    doc[F("led")][1] = model.ledOnHigh;
    doc[F("led")][2] = model.ledEnabled;
    doc[F("wifi")][0] = model.wifiSsid;
    doc[F("wifi")][1] = model.wifiPass;
    doc[F("wifi")][2] = model.autoMode ? 1 : 0;
  }

  void AppStorage::_deserializeFrom(AppModel& model, const DynamicJsonDocument& doc) {
    const char* name = doc[F("name")];
    model.name = String(name);
    model.ledPin = doc[F("led")][0];
    model.ledOnHigh = doc[F("led")][1];
    model.ledEnabled = doc[F("led")][2];
    model.wifiSsid = String(doc[F("wifi")][0]);
    model.wifiPass = String(doc[F("wifi")][1]);
    model.autoMode = doc[F("wifi")][2] == 1;
  }

  // global
  AppStorage appStorage;

} // namespace Victor::Components
