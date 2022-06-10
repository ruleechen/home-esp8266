#include "IntervalOver.h"

namespace Victor::Components {

  IntervalOver::IntervalOver(unsigned long ms, unsigned long last) {
    _ms = ms;
    _last = last;
  }

  bool IntervalOver::isOver(unsigned long now) {
    if (now == 0) {
      now = millis();
    }
    const auto over = now - _last > _ms;
    if (over) {
      _last = now;
    }
    return over;
  }

} // namespace Victor::Components
