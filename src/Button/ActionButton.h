#ifndef ActionButton_h
#define ActionButton_h

#include "Button/Button.h"

#ifndef VICTOR_ACTION_BUTTON_RELEASED
#define VICTOR_ACTION_BUTTON_RELEASED 50
#endif

#ifndef VICTOR_ACTION_BUTTON_RESTART
#define VICTOR_ACTION_BUTTON_RESTART 5 * 1000
#endif

#ifndef VICTOR_ACTION_BUTTON_RESTORE
#define VICTOR_ACTION_BUTTON_RESTORE 10 * 1000
#endif

namespace Victor::Components {

  enum ButtonAction {
    ButtonActionNone = 0,
    ButtonActionPressed = 1,
    ButtonActionReleased = 2,
    ButtonActionRestart = 3,
    ButtonActionRestore = 4,
  };

  class ActionButton {
   public:
    ActionButton(uint8_t inputPin, uint8_t inputTrueValue = 0);
    ~ActionButton();
    void loop();
    // events
    typedef std::function<void(ButtonAction action)> TActionHandler;
    TActionHandler onAction = nullptr;

   private:
    Button* _input = nullptr;
    unsigned long _lastPress = 0;
  };

} // namespace Victor::Components

#endif // ActionButton_h
