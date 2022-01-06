#ifndef AppStorage_h
#define AppStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  struct AppModel {
    String name;
    int8_t ledPin = 2;      // -128 ~ 127
    uint8_t ledOnValue = 0; // 0 ~ 256 LOW
    bool overTheWeb = false;
    String wifiSsid;
    String wifiPass;
    bool autoMode = true;
  };

  class AppStorage : public FileStorage<AppModel> {
   public:
    AppStorage();

   protected:
    void _serializeTo(const AppModel& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(AppModel& model, const DynamicJsonDocument& doc) override;
  };

  // global
  extern AppStorage appStorage;

} // namespace Victor::Components

#endif // AppStorage_h
