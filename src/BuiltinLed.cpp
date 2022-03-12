#include "BuiltinLed.h"

#define LED_FLASH_DURATION 500

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
    if (model.ledEnabled) {
      const auto trueValue = model.ledOnHigh ? HIGH : LOW;
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

  void BuiltinLed::toggle() {
    if (_outputPin) {
      const auto value = _outputPin->lastValue();
      _outputPin->setValue(!value);
    }
  }

  void BuiltinLed::flash() {
    toggle();
    delay(LED_FLASH_DURATION);
    toggle();
  }

  void BuiltinLed::twinkle() {
    toggle();
    _ticker->detach();
    _ticker->attach_ms(LED_FLASH_DURATION, [&]() {
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
