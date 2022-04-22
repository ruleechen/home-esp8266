#ifndef SwitchStorage_h
#define SwitchStorage_h

#include "FileStorage.h"
#include "SwitchModels.h"

namespace Victor::Components {
  class SwitchStorage : public FileStorage<SwitchSetting> {
   public:
    SwitchStorage(char* filePath);

   protected:
    void _serializeTo(const SwitchSetting& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(SwitchSetting& model, const DynamicJsonDocument& doc) override;
  };

} // namespace Victor::Components

#endif // SwitchStorage_h
