
#include "ActionButton.h"

namespace Victor::Components {

  ActionButton::ActionButton(uint8_t inputPin, uint8_t inputTrueValue) {
    _input = new DigitalButton(inputPin, inputTrueValue);
    _input->onPressed = [&]() {
      _lastPressed = millis();
      if (onAction != nullptr) {
        onAction(ButtonActionPressed);
      }
    };
    _input->onReleased = [&]() {
      auto action = ButtonActionNone;
      const auto duration = millis() - _lastPressed;
      if (duration >= VICTOR_ACTION_BUTTON_RESTORE) {
        action = ButtonActionRestore;
      } else if (duration >= VICTOR_ACTION_BUTTON_RESTART) {
        action = ButtonActionRestart;
      } else if (duration >= VICTOR_ACTION_BUTTON_RELEASED) {
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

  ActionButton::~ActionButton() {
    onAction = nullptr;
    if (_input != nullptr) {
      delete _input;
      _input = nullptr;
    }
  }

  void ActionButton::loop() {
    _input->loop();
  }

} // namespace Victor::Components
