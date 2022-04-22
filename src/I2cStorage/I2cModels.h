#ifndef I2cModels_h
#define I2cModels_h

#include <Arduino.h>

namespace Victor::Components {

  /**
   * {"sda":4,"scl":5,"loop":10,"reset":24}
   */
  struct I2cSetting {
    uint8_t sdaPin = 4;
    uint8_t sclPin = 5;
    uint8_t loopSeconds = 10;
    uint8_t resetHours = 24;
  };

} // namespace Victor::Components

#endif // I2cModels_h
