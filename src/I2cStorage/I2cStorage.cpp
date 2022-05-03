#include "I2cStorage.h"

namespace Victor::Components {

  I2cStorage::I2cStorage(const char* filePath) {
    _filePath = filePath;
    _maxSize = 512;
  }

  void I2cStorage::_serializeTo(const I2cConfig& model, DynamicJsonDocument& doc) {
    doc[F("sda")] = model.sdaPin;
    doc[F("scl")] = model.sclPin;
    doc[F("loop")] = model.loopSeconds;
    doc[F("reset")] = model.resetHours;
  }

  void I2cStorage::_deserializeFrom(I2cConfig& model, const DynamicJsonDocument& doc) {
    model.sdaPin = doc[F("sda")];
    model.sclPin = doc[F("scl")];
    model.loopSeconds = doc[F("loop")];
    model.resetHours = doc[F("reset")];
  }

} // namespace Victor::Components
