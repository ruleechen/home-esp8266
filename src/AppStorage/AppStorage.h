#ifndef AppStorage_h
#define AppStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  struct AppSetting {
    String name;
    uint8_t ledPin = 2;     // 0 ~ 256
    bool ledOnHigh = false; // false: LOW, true: HIGH
    bool ledEnabled = true;
  };

  class AppStorage : public FileStorage<AppSetting> {
   public:
    AppStorage(const char* filePath = "/app.json");

   protected:
    void _serializeTo(const AppSetting& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(AppSetting& model, const DynamicJsonDocument& doc) override;
  };

  // global
  extern AppStorage appStorage;

} // namespace Victor::Components

#endif // AppStorage_h
