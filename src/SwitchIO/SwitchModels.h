#ifndef SwitchModels_h
#define SwitchModels_h

#include <Arduino.h>

namespace Victor::Components {

  struct SwitchSetting {
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

} // namespace Victor::Components

#endif // SwitchModels_h
