#include "Console.h"

namespace Victor {

  Console::Console() {}

  void Console::begin(unsigned long baud) {
    Serial.begin(baud);
  }

  Console Console::log() {
    return ts().bracket(F("log"));
  }

  Console Console::log(const String& msg) {
    return log().section(msg);
  }

  Console Console::error() {
    return ts().bracket(F("error"));
  }

  Console Console::error(const String& msg) {
    return error().section(msg);
  }

  Console Console::debug() {
    return ts().bracket(F("debug"));
  }

  Console Console::debug(const String& msg) {
    return debug().section(msg);
  }

  Console Console::ts() {
    return newline().bracket(String(millis()));
  }

  Console Console::bracket(const String& msg) {
    return write(F("[") + msg + F("]"));
  }

  Console Console::section(const String& msg) {
    return write(F(" ") + msg);
  }

  Console Console::write(const String& msg) {
    Serial.print(msg);
    return console;
  }

  Console Console::newline() {
    Serial.println(F(""));
    return console;
  }

  // global
  Console console;

} // namespace Victor
