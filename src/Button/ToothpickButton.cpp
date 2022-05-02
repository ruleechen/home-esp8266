
#include "ToothpickButton.h"

namespace Victor::Components {

  ToothpickButton::ToothpickButton(uint8_t inputPin, uint8_t inputTrueValue) {
    _input = new Button(inputPin, inputTrueValue);
    _input->onClick = [&](unsigned long duration) {
      auto action = ButtonClick;
      if (duration >= TOOTHPICK_BUTTON_RESTORE) {
        action = ButtonRestore;
      } else if (duration >= TOOTHPICK_BUTTON_RESTART) {
        action = ButtonRestart;
      } else if (duration >= TOOTHPICK_BUTTON_CLICK) {
        action = ButtonClick;
      }
      if (onClick != nullptr) {
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
