#ifndef IntervalOver_h
#define IntervalOver_h

#include <functional>
#include <Arduino.h>

namespace Victor::Components {
  class IntervalOver {
   public:
    IntervalOver(unsigned long ms, unsigned long last = 0);
    bool isOver(unsigned long now = 0);

   private:
    unsigned long _ms;
    unsigned long _last;
  };
} // namespace Victor::Components

#endif // IntervalOver_h
