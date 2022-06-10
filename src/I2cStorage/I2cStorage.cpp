#include "I2cStorage.h"

namespace Victor::Components {

  I2cStorage::I2cStorage(const char* filePath) : FileStorage(filePath) {
    _maxSize = 128;
  }

  void I2cStorage::_serializeTo(const I2cConfig& model, DynamicJsonDocument& doc) {
    doc[F("sda")] = model.sdaPin;
    doc[F("scl")] = model.sclPin;
  }

  void I2cStorage::_deserializeFrom(I2cConfig& model, const DynamicJsonDocument& doc) {
    model.sdaPin = doc[F("sda")];
    model.sclPin = doc[F("scl")];
  }

} // namespace Victor::Components
