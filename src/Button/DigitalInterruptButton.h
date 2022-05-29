#ifndef DigitalInterruptButton_h
#define DigitalInterruptButton_h

#include <vector>
#include "Console.h"
#include "DigitalInput.h"
#include "Button/ActionButton.h"

#ifndef VICTOR_DIGITAL_INPUT_MAX_CHANGES
#define VICTOR_DIGITAL_INPUT_MAX_CHANGES 5
#endif

namespace Victor::Components {

  struct InterruptContext {
    bool inputValue;
    unsigned long timestamp;
  };

  class DigitalInterruptButton {
   public:
    DigitalInterruptButton(uint8_t inputPin, uint8_t inputTrueValue);
    ~DigitalInterruptButton();
    void loop();
    // events
    ActionButton::TActionHandler onAction = nullptr;

   private:
    ActionButton* _button = nullptr;
    // interrupt
    static DigitalInput* _input;
    static std::vector<InterruptContext> _contexts;
    static void IRAM_ATTR _interruptHandler();
  };

} // namespace Victor::Components

#endif // DigitalInterruptButton_h
