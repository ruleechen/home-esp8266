#include "WifiStorage.h"

namespace Victor::Components {

  WifiStorage::WifiStorage(const char* filePath) : FileStorage(filePath) {
    _maxSize = 128;
    _enableCache = false;
  }

  void WifiStorage::_serializeTo(const WifiSetting* model, DynamicJsonDocument& doc) {
    doc[F("ssid")]  = model->ssid;
    doc[F("pswd")]  = model->pswd;
    doc[F("auto")]  = model->autoMode ? 1 : 0;
    doc[F("brand")] = model->brand;
  }

  void WifiStorage::_deserializeFrom(WifiSetting* model, const DynamicJsonDocument& doc) {
    model->ssid     = String(doc[F("ssid")]);
    model->pswd     = String(doc[F("pswd")]);
    model->autoMode = doc[F("auto")] == 1;
    model->brand    = String(doc[F("brand")]);
  }

} // namespace Victor::Components
