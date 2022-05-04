#ifndef Button_h
#define Button_h

#include <DigitalInput.h>

#define BUTTON_THROTTLE_MILLIS 100

namespace Victor::Components {
  class Button {
   public:
    Button(uint8_t inputPin, uint8_t inputTrueValue = 0);
    ~Button();
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

#endif // Button_h
