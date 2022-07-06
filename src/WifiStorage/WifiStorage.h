#ifndef WifiStorage_h
#define WifiStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  struct WifiSetting {
    String ssid;
    String pswd;
    bool autoMode = true;
    String brand;
  };

  class WifiStorage : public FileStorage<WifiSetting> {
   public:
    WifiStorage(const char* filePath = "/wifi.json");

   protected:
    void _serializeTo(const WifiSetting* model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(WifiSetting* model, const DynamicJsonDocument& doc) override;
  };

} // namespace Victor::Components

#endif // WifiStorage_h
