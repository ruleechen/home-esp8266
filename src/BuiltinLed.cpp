#include "BuiltinLed.h"

namespace Victor::Components {

  BuiltinLed::BuiltinLed() {
    _ticker = new Ticker();
  }

  BuiltinLed::~BuiltinLed() {
    if (_ticker) {
      delete _ticker;
      _ticker = nullptr;
    }
    if (_outputPin) {
      delete _outputPin;
      _outputPin = nullptr;
    }
  }

  void BuiltinLed::setup() {
    const auto model = appStorage.load();
    if (model.ledPin > -1) {
      const auto trueValue = model.ledOnValue == 0 ? LOW : HIGH;
      _outputPin = new DigitalOutput(model.ledPin, trueValue);
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
    // at least light for some time
    _ticker->detach();
    _ticker->once_ms(100, [&]() {
      turnOff();
    });
  }

  void BuiltinLed::toggle() {
    if (_outputPin) {
      const auto value = _outputPin->lastValue();
      _outputPin->setValue(!value);
    }
  }

  void BuiltinLed::twinkle() {
    _ticker->detach();
    _ticker->attach_ms(100, [&]() {
      toggle();
    });
  }

  void BuiltinLed::stop() {
    _ticker->detach();
    turnOff();
  }

  // global
  BuiltinLed builtinLed;

} // namespace Victor::Components
