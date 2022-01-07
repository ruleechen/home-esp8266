#ifndef BuiltinLed_h
#define BuiltinLed_h

#include <Arduino.h>
#include <Ticker.h>
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
    void twinkle();
    void stop();

   private:
    Ticker* _ticker = nullptr;
    DigitalOutput* _outputPin = nullptr;
  };

  // global
  extern BuiltinLed builtinLed;

} // namespace Victor::Components

#endif // BuiltinLed_h
