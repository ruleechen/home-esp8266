#ifndef Button_h
#define Button_h

#include <functional>
#include <Arduino.h>

namespace Victor::Components {

  template <typename TState>
  class Button {
   public:
    Button(TState state);
    virtual ~Button();
    // state
    virtual void update(TState state);
    virtual bool isEqual(TState state);
    virtual bool isPressed(TState state);
    // events
    typedef std::function<void(unsigned long timespan)> TEventHandler;
    TEventHandler onPressed = nullptr;
    TEventHandler onReleased = nullptr;

   protected:
    TState _state;
    unsigned long _lastPressed = 0;
  };

  template <typename TState>
  Button<TState>::Button(TState state) {
    _state = state;
  }

  template <typename TState>
  Button<TState>::~Button() {
    onPressed = nullptr;
    onReleased = nullptr;
  }

  template <typename TState>
  void Button<TState>::update(TState state) {
    if (!isEqual(state)) {
      _state = state;
      const auto now = millis();
      const auto timespan = now - _lastPressed;
      if (isPressed(_state)) {
        _lastPressed = now;
        if (onPressed != nullptr) {
          onPressed(timespan);
        }
      } else {
        if (onReleased != nullptr) {
          onReleased(timespan);
        }
      }
    }
  }

  template <typename TState>
  bool Button<TState>::isEqual(TState state) {
    return state == _state;
  }

  template <typename TState>
  bool Button<TState>::isPressed(TState state) {
    return false;
  }

} // namespace Victor::Components

#endif // Button_h
