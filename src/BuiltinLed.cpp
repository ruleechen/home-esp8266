#include "BuiltinLed.h"

namespace Victor::Components {

  BuiltinLed::BuiltinLed() {
    const auto model = appStorage.load();
    if (model.ledPin > -1) {
      const auto trueValue = model.ledOnValue == 0 ? LOW : HIGH;
      _outputPin = new DigitalOutput(model.ledPin, trueValue);
    }
  }

  BuiltinLed::~BuiltinLed() {
    if (_outputPin) {
      delete _outputPin;
      _outputPin = NULL;
    }
  }

  void BuiltinLed::turnOn() {
    if (_outputPin) {
      _outputPin->setValue(true);
    }
  }

  void BuiltinLed::turnOff() {
    if (_outputPin) {
      _outputPin->setValue(false);
    }
  }

  void BuiltinLed::flash() {
    turnOn();
    delay(100); // at least light for some time
    turnOff();
  }

  void BuiltinLed::toggle() {
    if (_outputPin) {
      const auto value = _outputPin->lastValue();
      _outputPin->setValue(!value);
    }
  }

} // namespace Victor::Components
