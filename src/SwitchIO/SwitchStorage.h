#ifndef SwitchStorage_h
#define SwitchStorage_h

#include "FileStorage.h"

namespace Victor::Components {

  struct SwitchConfig {
    // input pin
    // 0~128 = gpio
    //    -1 = disabled
    int8_t inputPin = -1; // (-127 ~ 128)
    // 0 = LOW
    // 1 = HIGH
    uint8_t inputTrueValue = 0; // (0 ~ 256) LOW

    // output pin
    // 0~128 = gpio
    //    -1 = disabled
    int8_t outputPin = -1; // (-127 ~ 128)
    // 0 = LOW
    // 1 = HIGH
    uint8_t outputTrueValue = 0; // (0 ~ 256) LOW

    // output2 pin
    // 0~128 = gpio
    //    -1 = disabled
    int8_t output2Pin = -1; // (-127 ~ 128)
    // 0 = LOW
    // 1 = HIGH
    uint8_t output2TrueValue = 0; // (0 ~ 256) LOW

    // save output state or not
    bool saveOutput = false;
    // output state saved
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
