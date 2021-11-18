#ifndef DigitalOutput_h
#define DigitalOutput_h

#include <Arduino.h>

namespace Victor::Components {
  class DigitalOutput {
   public:
    DigitalOutput(uint8_t pin, uint8_t trueValue);
    void setValue(bool value);
    bool getValue();

   private:
    uint8_t _pin;
    uint8_t _trueValue;
    bool _currentValue = false;
  };

} // namespace Victor::Components

#endif // DigitalOutput_h
