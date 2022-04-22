#ifndef I2cStorage_h
#define I2cStorage_h

#include <FileStorage.h>
#include "I2cModels.h"

namespace Victor::Components {
  class I2cStorage : public FileStorage<I2cSetting> {
   public:
    I2cStorage(const char* filePath);

   protected:
    void _serializeTo(const I2cSetting& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(I2cSetting& model, const DynamicJsonDocument& doc) override;
  };

} // namespace Victor::Components

#endif // I2cStorage_h
