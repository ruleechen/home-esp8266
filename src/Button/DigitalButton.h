#ifndef DigitalButton_h
#define DigitalButton_h

#include "DigitalInput.h"
#include "Button/ActionButton.h"

namespace Victor::Components {

  class DigitalButton {
   public:
    DigitalButton(uint8_t inputPin, uint8_t inputTrueValue);
    virtual ~DigitalButton();
    virtual void loop();
    // events
    ActionButton::TActionHandler onAction = nullptr;

   protected:
    DigitalInput* _input = nullptr;
    ActionButton* _button = nullptr;
  };

} // namespace Victor::Components

#endif // DigitalButton_h
