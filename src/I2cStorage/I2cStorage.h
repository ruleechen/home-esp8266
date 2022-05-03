#ifndef I2cStorage_h
#define I2cStorage_h

#include <FileStorage.h>

namespace Victor::Components {

  struct I2cConfig {
    uint8_t sdaPin = 4;
    uint8_t sclPin = 5;
    uint8_t loopSeconds = 10;
    uint8_t resetHours = 24;
  };

  class I2cStorage : public FileStorage<I2cConfig> {
   public:
    I2cStorage(const char* filePath);

   protected:
    void _serializeTo(const I2cConfig& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(I2cConfig& model, const DynamicJsonDocument& doc) override;
  };

} // namespace Victor::Components

#endif // I2cStorage_h
