#include "AppStorage.h"

namespace Victor::Components {

  AppStorage::AppStorage(const char* filePath) : FileStorage(filePath) {
    _maxSize = 128;
    _enableCache = false;
  }

  void AppStorage::_serializeTo(const AppSetting& model, DynamicJsonDocument& doc) {
    doc[F("name")] = model.name;
    doc[F("led")][0] = model.ledPin;
    doc[F("led")][1] = model.ledOnHigh;
    doc[F("led")][2] = model.ledEnabled;
  }

  void AppStorage::_deserializeFrom(AppSetting& model, const DynamicJsonDocument& doc) {
    const char* name = doc[F("name")];
    model.name = String(name);
    model.ledPin = doc[F("led")][0];
    model.ledOnHigh = doc[F("led")][1];
    model.ledEnabled = doc[F("led")][2];
  }

  // global
  AppStorage appStorage;

} // namespace Victor::Components
