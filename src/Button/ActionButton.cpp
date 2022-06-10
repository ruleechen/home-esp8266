#include "ActionButton.h"

namespace Victor::Components {

  ActionButton::ActionButton(bool initState) : Button(initState) {
    onPressed = [&](const unsigned long timespan) {
      if (onAction != nullptr && timespan > VICTOR_ACTION_BUTTON_DEBOUNCE) {
        onAction(BUTTON_ACTION_PRESSED);
        if (
          VICTOR_ACTION_BUTTON_DOUBLE_MIN < timespan &&
          timespan <= VICTOR_ACTION_BUTTON_DOUBLE_MAX
        ) {
          onAction(BUTTON_ACTION_DOUBLE_PRESSED);
        }
      }
    };
    onReleased = [&](const unsigned long timespan) {
      if (onAction != nullptr && timespan > VICTOR_ACTION_BUTTON_DEBOUNCE) {
        onAction(BUTTON_ACTION_RELEASED);
        if (timespan >= VICTOR_ACTION_BUTTON_RESTORE_MIN) {
          onAction(BUTTON_ACTION_RESTORE);
        } else if (timespan >= VICTOR_ACTION_BUTTON_RESTART_MIN) {
          onAction(BUTTON_ACTION_RESTART);
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
