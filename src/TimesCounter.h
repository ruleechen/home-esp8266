#ifndef TimesCounter_h
#define TimesCounter_h

#include <functional>
#include <Arduino.h>

namespace Victor::Components {
  class TimesCounter {
   public:
    TimesCounter(unsigned long resetMillis);
    typedef std::function<void(uint8_t count)> TCountHandler;
    TCountHandler onCount = nullptr;
    void count();
    void reset();

   private:
    // args
    unsigned long _resetMillis;
    // state
    uint8_t _count = 0;
    unsigned long _last = 0;
  };
} // namespace Victor::Components

#endif // TimesCounter_h
