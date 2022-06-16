#ifndef IntervalOver_h
#define IntervalOver_h

#include <Arduino.h>

namespace Victor::Components {
  class IntervalOver {
   public:
    IntervalOver(unsigned long ms);
    void start(unsigned long now = 0);
    virtual bool isOver(unsigned long now = 0);

   private:
    unsigned long _ms = 0;
    unsigned long _start = 0;
  };
} // namespace Victor::Components

#endif // IntervalOver_h
