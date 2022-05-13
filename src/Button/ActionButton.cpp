#include "ActionButton.h"

namespace Victor::Components {

  ActionButton::ActionButton(bool initState) : Button(initState) {
    onPressed = [&](const unsigned long timespan) {
      if (onAction != nullptr) {
        onAction(ButtonActionPressed);
        if (
          VICTOR_ACTION_BUTTON_DOUBLE_MIN < timespan &&
          timespan <= VICTOR_ACTION_BUTTON_DOUBLE_MAX
        ) {
          onAction(ButtonActionDoublePressed);
        }
      }
    };
    onReleased = [&](const unsigned long timespan) {
      if (onAction != nullptr) {
        onAction(ButtonActionReleased);
        if (timespan >= VICTOR_ACTION_BUTTON_RESTORE_MIN) {
          onAction(ButtonActionRestore);
        } else if (timespan >= VICTOR_ACTION_BUTTON_RESTART_MIN) {
          onAction(ButtonActionRestart);
        }
      }
    };
  }

  ActionButton::~ActionButton() {
    Button<bool>::~Button();
    onAction = nullptr;
  }

  bool ActionButton::isPressed(bool state) {
    return state == true;
  }

} // namespace Victor::Components
