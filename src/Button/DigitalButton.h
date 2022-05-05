#ifndef DigitalButton_h
#define DigitalButton_h

#include <DigitalInput.h>

#ifndef VICTOR_DIGITAL_BUTTON_THROTTLE
#define VICTOR_DIGITAL_BUTTON_THROTTLE 100
#endif

namespace Victor::Components {
  class DigitalButton {
   public:
    DigitalButton(uint8_t inputPin, uint8_t inputTrueValue = 0);
    ~DigitalButton();
    void loop();
    bool getState();
    // events
    typedef std::function<void()> TEventHandler;
    TEventHandler onPressed = nullptr;
    TEventHandler onReleased = nullptr;

   private:
    DigitalInput* _input = nullptr;
    unsigned long _lastLoop = 0;
    bool _state = false;
  };

} // namespace Victor::Components

#endif // DigitalButton_h
