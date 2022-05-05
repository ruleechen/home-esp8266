#ifndef DigitalInputButton_h
#define DigitalInputButton_h

#include "DigitalInput.h"
#include "Button/ActionButton.h"

namespace Victor::Components {

  class DigitalInputButton {
   public:
    DigitalInputButton(uint8_t inputPin, uint8_t inputTrueValue);
    ~DigitalInputButton();
    void loop();
    // events
    ActionButton::TActionHandler onAction = nullptr;

   private:
    DigitalInput* _input = nullptr;
    ActionButton* _button = nullptr;
  };

} // namespace Victor::Components

#endif // DigitalInputButton_h
