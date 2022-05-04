
#include "ToothpickButton.h"

namespace Victor::Components {

  ToothpickButton::ToothpickButton(uint8_t inputPin, uint8_t inputTrueValue) {
    _input = new Button(inputPin, inputTrueValue);
    _input->onPressed = [&]() {
      _lastPress = millis();
      if (onAction != nullptr) {
        onAction(ButtonActionPressed);
      }
    };
    _input->onReleased = [&]() {
      const auto duration = millis() - _lastPress;
      auto action = ButtonActionNone;
      if (duration >= VICTOR_BUTTON_RESTORE) {
        action = ButtonActionRestore;
      } else if (duration >= VICTOR_BUTTON_RESTART) {
        action = ButtonActionRestart;
      } else if (duration >= VICTOR_BUTTON_RELEASED) {
        action = ButtonActionReleased;
      }
      if (
        onAction != nullptr &&
        action != ButtonActionNone
      ) {
        onAction(action);
      }
    };
  }

  ToothpickButton::~ToothpickButton() {
    onAction = nullptr;
    if (_input != nullptr) {
      delete _input;
      _input = nullptr;
    }
  }

  void ToothpickButton::loop() {
    _input->loop();
  }

} // namespace Victor::Components
