#ifndef InfraModels_h
#define InfraModels_h

#include <Arduino.h>

namespace Victor {

  enum VOtaType {
    VOta_All = 0,
    VOta_Sketch = 1,
    VOta_FileSystem = 2,
  };

  enum ButtonState {
    AWAIT_PRESS,
    DEBOUNCE_PRESS,
    AWAIT_RELEASE,
    DEBOUNCE_RELEASE,
    AWAIT_MULTI_PRESS,
    DEBOUNCE_MULTI_PRESS,
  };

  struct KeyValueModel {
    String key;
    String value;
  };

  struct AppModel {
    String name;
    int8_t ledPin = 2;      // -128 ~ 127
    uint8_t ledOnValue = 0; // 0 ~ 256 LOW
    bool overTheWeb = false;
    String wifiSsid;
    String wifiPass;
    bool autoMode = true;
  };

} // namespace Victor

#endif // InfraModels_h
