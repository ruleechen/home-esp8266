#include "ActionButton.h"

namespace Victor::Components {

  ActionButton::ActionButton(bool initState) : Button(initState) {
    onPressed = [&](const unsigned long timespan) {
      if (onAction != nullptr) {
        if (timespan <= VICTOR_ACTION_BUTTON_DOUBLE) {
          onAction(ButtonActionDoublePressed);
        } else {
          onAction(ButtonActionPressed);
        }
      }
    };
    onReleased = [&](const unsigned long timespan) {
      if (onAction != nullptr) {
        if (timespan >= VICTOR_ACTION_BUTTON_RESTORE) {
          onAction(ButtonActionRestore);
        } else if (timespan >= VICTOR_ACTION_BUTTON_RESTART) {
          onAction(ButtonActionRestart);
        } else {
          onAction(ButtonActionReleased);
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
