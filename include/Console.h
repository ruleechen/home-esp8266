#ifndef Console_h
#define Console_h

#include <Arduino.h>

namespace Victor {
  class Console {
   public:
    Console();
    void begin(unsigned long baud);
    Console log();
    Console log(const String& message);
    Console error();
    Console error(const String& message);
    Console debug();
    Console debug(const String& message);
    Console ts();
    Console newline();
    Console writeType(const String& type);
    Console write(const String& message);
  };

  // global
  extern Console console;

} // namespace Victor

#endif // Console_h
