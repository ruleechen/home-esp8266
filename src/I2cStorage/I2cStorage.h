#ifndef I2cStorage_h
#define I2cStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  // https://zhuanlan.zhihu.com/p/137568249
  struct I2cSetting {
    uint8_t sdaPin = 4; // (0 ~ 256) Inter-Integrated Circuit - Serial Data (I2C-SDA)
    uint8_t sclPin = 5; // (0 ~ 256) Inter-Integrated Circuit - Serial Clock (I2C-SCL)
    // enable pin
    int8_t enablePin = -1; // (-127 ~ 128)
    uint8_t enableTrueValue = 0; // (0 ~ 256) LOW
  };

  class I2cStorage : public FileStorage<I2cSetting> {
   public:
    I2cStorage(const char* filePath = "/i2c.json");

   protected:
    void _serializeTo(const I2cSetting& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(I2cSetting& model, const DynamicJsonDocument& doc) override;
  };

} // namespace Victor::Components

#endif // I2cStorage_h
