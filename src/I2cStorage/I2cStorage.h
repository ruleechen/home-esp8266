#ifndef I2cStorage_h
#define I2cStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  // https://zhuanlan.zhihu.com/p/137568249
  struct I2cConfig {
    uint8_t sdaPin = 4; // (0 ~ 256) Inter-Integrated Circuit - Serial Data (I2C-SDA)
    uint8_t sclPin = 5; // (0 ~ 256) Inter-Integrated Circuit - Serial Clock (I2C-SCL)
    uint8_t loopSeconds = 10; // (0 ~ 256) How many seconds to read devices on i2c bus.
    uint8_t resetHours = 24;  // (0 ~ 256) How many hours to soft reset devices on i2c bus.
  };

  class I2cStorage : public FileStorage<I2cConfig> {
   public:
    I2cStorage(const char* filePath = "/i2c.json");

   protected:
    void _serializeTo(const I2cConfig& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(I2cConfig& model, const DynamicJsonDocument& doc) override;
  };

} // namespace Victor::Components

#endif // I2cStorage_h
