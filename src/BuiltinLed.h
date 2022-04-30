#ifndef BuiltinLed_h
#define BuiltinLed_h

#include <Arduino.h>
#include <Ticker.h>
#include "DigitalOutput.h"
#include "AppStorage.h"

#define LED_FLASH_DURATION 10
#define LED_TWINKLE_DURATION 500

namespace Victor::Components {
  class BuiltinLed {
   public:
    BuiltinLed();
    ~BuiltinLed();
    void setup();
    void turnOn();
    void turnOff();
    void toggle();
    void flash(uint16_t duration = LED_FLASH_DURATION);
    void twinkle(uint16_t duration = LED_FLASH_DURATION);
    void stop();

   private:
    Ticker* _ticker = nullptr;
    DigitalOutput* _outputPin = nullptr;
  };

  // global
  extern BuiltinLed builtinLed;

} // namespace Victor::Components

#endif // BuiltinLed_h
