#ifndef SwitchIO_h
#define SwitchIO_h

#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "SwitchStorage.h"
#include "Button/ActionButtonInput.h"

namespace Victor::Components {
  class SwitchIO {
   public:
    SwitchIO(const char* settingFile = "/switch.json");
    ~SwitchIO();
    void loop();
    bool getOutputState();
    void setOutputState(bool value);
    // events
    ActionButton::TActionHandler onInputAction = nullptr;

   private:
    SwitchStorage* _storage = nullptr;
    ActionButtonInput* _input = nullptr;
    DigitalOutput* _output = nullptr;
    DigitalOutput* _output2 = nullptr;
  };

} // namespace Victor::Components

#endif // SwitchIO_h
