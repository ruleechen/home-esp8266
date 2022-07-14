#include "DigitalButton.h"

namespace Victor::Components {

  DigitalButton::DigitalButton(uint8_t inputPin, uint8_t inputTrueValue) {
    _input = new DigitalInput(inputPin, inputTrueValue);
    _button = new ActionButton(_input->getValue());
    _button->onAction = [&](ButtonAction action) {
      if (onAction != nullptr) {
        onAction(action);
      }
    };
  }

  DigitalButton::~DigitalButton() {
    onAction = nullptr;
    if (_input != nullptr) {
      delete _input;
      _input = nullptr;
    }
    if (_button != nullptr) {
      delete _button;
      _button = nullptr;
    }
  }

  void DigitalButton::loop() {
    _button->update(_input->getValue());
  }

} // namespace Victor::Components
