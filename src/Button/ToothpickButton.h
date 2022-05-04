#ifndef ToothpickButton_h
#define ToothpickButton_h

#include "Button/Button.h"

#define TOOTHPICK_BUTTON_RELEASED 50
#define TOOTHPICK_BUTTON_RESTART  5 * 1000
#define TOOTHPICK_BUTTON_RESTORE  10 * 1000

namespace Victor::Components {

  enum ButtonAction {
    ButtonActionNone = 0,
    ButtonActionPressed = 1,
    ButtonActionReleased = 2,
    ButtonActionRestart = 3,
    ButtonActionRestore = 4,
  };

  class ToothpickButton {
   public:
    ToothpickButton(uint8_t inputPin, uint8_t inputTrueValue = 0);
    ~ToothpickButton();
    void loop();
    // events
    typedef std::function<void(ButtonAction action)> TActionHandler;
    TActionHandler onAction = nullptr;

   private:
    Button* _input = nullptr;
    unsigned long _lastPress = 0;
  };

} // namespace Victor::Components

#endif // ToothpickButton_h
