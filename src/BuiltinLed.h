#ifndef BuiltinLed_h
#define BuiltinLed_h

#include <Arduino.h>
#include <Ticker.h>
#include "DigitalOutput.h"
#include "AppStorage.h"

#ifndef VICTOR_LED_FLASH
#define VICTOR_LED_FLASH 10
#endif

#ifndef VICTOR_LED_TWINKLE
#define VICTOR_LED_TWINKLE 500
#endif

namespace Victor::Components {
  class BuiltinLed {
   public:
    BuiltinLed();
    ~BuiltinLed();
    void setup();
    void turnOn();
    void turnOff();
    void toggle();
    void flash(uint16_t duration = VICTOR_LED_FLASH);
    void twinkle(uint16_t duration = VICTOR_LED_TWINKLE);
    void stop();

   private:
    Ticker* _ticker = nullptr;
    DigitalOutput* _outputPin = nullptr;
  };

  // global
  extern BuiltinLed builtinLed;

} // namespace Victor::Components

#endif // BuiltinLed_h
