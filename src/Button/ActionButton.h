#ifndef ActionButton_h
#define ActionButton_h

#include "Button/Button.h"

#ifndef VICTOR_ACTION_BUTTON_DEBOUNCE
#define VICTOR_ACTION_BUTTON_DEBOUNCE 50
#endif

#ifndef VICTOR_ACTION_BUTTON_DOUBLE_MIN
#define VICTOR_ACTION_BUTTON_DOUBLE_MIN 100
#endif

#ifndef VICTOR_ACTION_BUTTON_DOUBLE_MAX
#define VICTOR_ACTION_BUTTON_DOUBLE_MAX 400
#endif

#ifndef VICTOR_ACTION_BUTTON_RESTART_MIN
#define VICTOR_ACTION_BUTTON_RESTART_MIN 5 * 1000
#endif

#ifndef VICTOR_ACTION_BUTTON_RESTORE_MIN
#define VICTOR_ACTION_BUTTON_RESTORE_MIN 10 * 1000
#endif

namespace Victor::Components {

  enum ButtonAction {
    BUTTON_ACTION_NONE           = 0,
    BUTTON_ACTION_PRESSED        = 1,
    BUTTON_ACTION_DOUBLE_PRESSED = 2,
    BUTTON_ACTION_RELEASED       = 3,
    BUTTON_ACTION_RESTART        = 4,
    BUTTON_ACTION_RESTORE        = 5,
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
