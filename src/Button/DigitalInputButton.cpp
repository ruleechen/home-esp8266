#include "DigitalInputButton.h"

namespace Victor::Components {

  DigitalInputButton::DigitalInputButton(uint8_t inputPin, uint8_t inputTrueValue) {
    _input = new DigitalInput(inputPin, inputTrueValue);
    _button = new ActionButton(_input->getValue());
    _button->onAction = [&](ButtonAction action) {
      if (onAction != nullptr) {
        onAction(action);
      }
    };
  }

  DigitalInputButton::~DigitalInputButton() {
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

  void DigitalInputButton::loop() {
    _button->update(_input->getValue());
  }

} // namespace Victor::Components
