#ifndef SwitchStorage_h
#define SwitchStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  struct SwitchConfig {
    // input pin
    int8_t inputPin = -1;
    uint8_t inputTrueValue = 0;  // LOW
    // output pin
    int8_t outputPin = -1;
    uint8_t outputTrueValue = 0; // LOW
    // output2 pin
    int8_t output2Pin = -1;
    uint8_t output2TrueValue = 0; // LOW
    // output state
    bool saveOutput = false;
    bool outputIsOn = false;
  };

  class SwitchStorage : public FileStorage<SwitchConfig> {
   public:
    SwitchStorage(const char* filePath);

   protected:
    void _serializeTo(const SwitchConfig& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(SwitchConfig& model, const DynamicJsonDocument& doc) override;
  };

} // namespace Victor::Components

#endif // SwitchStorage_h
