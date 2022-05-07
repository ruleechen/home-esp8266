#ifndef ActionButton_h
#define ActionButton_h

#include "Button/Button.h"

#ifndef VICTOR_ACTION_BUTTON_DOUBLE
#define VICTOR_ACTION_BUTTON_DOUBLE 400
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
    ButtonActionDoublePressed = 2,
    ButtonActionReleased = 3,
    ButtonActionRestart = 4,
    ButtonActionRestore = 5,
  };

  class ActionButton : public Button<bool> {
   public:
    ActionButton(bool initState);
    ~ActionButton();
    bool isPressed(bool state) override;
    // events
    typedef std::function<void(ButtonAction action)> TActionHandler;
    TActionHandler onAction = nullptr;
  };

} // namespace Victor::Components

#endif // ActionButton_h
