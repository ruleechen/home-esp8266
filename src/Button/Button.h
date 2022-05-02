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
    typedef std::function<void(unsigned long duration)> TStateHandler;
    TStateHandler onClick = nullptr;

   private:
    DigitalInput* _input = nullptr;
    bool _state = false;
    unsigned long _lastLoop = 0;
    unsigned long _lastPress = 0;
  };

} // namespace Victor::Components

#endif // Button_h
