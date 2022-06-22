#ifndef AppStorage_h
#define AppStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  struct LedSetting {
    // led gpio pin
    uint8_t pin = 2;     // (0~255)
    // false = LOW
    //  true = HIGH
    bool onHigh = false;
    // led enabled or not
    bool enabled = true;
  };

  struct AppSetting {
    String name;
    uint16_t sleepMillis = 200; // (0 ~ 65535) for light sleep mode
    LedSetting led;
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
