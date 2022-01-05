#ifndef BuiltinLed_h
#define BuiltinLed_h

#include <Arduino.h>
#include "DigitalOutput.h"
#include "AppStorage.h"

namespace Victor::Components {
  class BuiltinLed {
   public:
    BuiltinLed();
    ~BuiltinLed();
    void setup();
    void turnOn();
    void turnOff();
    void flash();
    void toggle();

   private:
    DigitalOutput* _outputPin;
  };

  // global
  extern BuiltinLed builtinLed;

} // namespace Victor::Components

#endif // BuiltinLed_h
