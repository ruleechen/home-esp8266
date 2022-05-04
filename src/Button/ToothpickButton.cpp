
#include "ToothpickButton.h"

namespace Victor::Components {

  ToothpickButton::ToothpickButton(uint8_t inputPin, uint8_t inputTrueValue) {
    _input = new Button(inputPin, inputTrueValue);
    _input->onPressed = [&]() {
      _lastPress = millis();
      if (onClick != nullptr) {
        onClick(ButtonActionPressed);
      }
    };
    _input->onReleased = [&]() {
      const auto duration = millis() - _lastPress;
      auto action = ButtonActionNone;
      if (duration >= TOOTHPICK_BUTTON_RESTORE) {
        action = ButtonActionRestore;
      } else if (duration >= TOOTHPICK_BUTTON_RESTART) {
        action = ButtonActionRestart;
      } else if (duration >= TOOTHPICK_BUTTON_RELEASED) {
        action = ButtonActionReleased;
      }
      if (
        onClick != nullptr &&
        action != ButtonActionNone
      ) {
        onClick(action);
      }
    };
  }

  ToothpickButton::~ToothpickButton() {
    onClick = nullptr;
    if (_input != nullptr) {
      delete _input;
      _input = nullptr;
    }
  }

  void ToothpickButton::loop() {
    _input->loop();
  }

} // namespace Victor::Components
