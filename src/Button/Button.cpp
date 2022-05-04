#include "Button.h"

namespace Victor::Components {

  Button::Button(uint8_t inputPin, uint8_t inputTrueValue) {
    _input = new DigitalInput(inputPin, inputTrueValue);
    _state = getState();
  }

  Button::~Button() {
    onPressed = nullptr;
    onReleased = nullptr;
    if (_input != nullptr) {
      delete _input;
      _input = nullptr;
    }
  }

  void Button::loop() {
    const auto now = millis();
    if (now - _lastLoop > VICTOR_BUTTON_THROTTLE) {
      _lastLoop = now;
      const auto pressed = getState();
      if (pressed != _state) {
        _state = pressed;
        if (pressed) {
          if (onPressed != nullptr) {
            onPressed();
          }
        } else {
          if (onReleased != nullptr) {
            onReleased();
          }
        }
      }
    }
  }

  bool Button::getState() {
    return _input->getValue();
  }

} // namespace Victor::Components
