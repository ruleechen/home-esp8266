#include "DigitalOutput.h"

namespace Victor::Components {

  DigitalOutput::DigitalOutput(uint8_t pin, uint8_t trueValue) {
    _pin = pin;
    _trueValue = trueValue;
    pinMode(_pin, OUTPUT);
  }

  void DigitalOutput::setValue(bool value) {
    auto falseValue = _trueValue == LOW ? HIGH : LOW;
    digitalWrite(_pin, value ? _trueValue : falseValue);
    _lastValue = value;
  }

  bool DigitalOutput::lastValue() {
    return _lastValue;
  }

} // namespace Victor::Components
