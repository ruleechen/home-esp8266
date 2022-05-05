#ifndef SwitchIO_h
#define SwitchIO_h

#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "SwitchStorage.h"
#include "Button/ActionButton.h"

namespace Victor::Components {
  class SwitchIO {
   public:
    SwitchIO(SwitchStorage* storage);
    ~SwitchIO();
    void loop();
    bool getOutputState();
    void setOutputState(bool value);
    // events
    ActionButton::TActionHandler onInputAction = nullptr;

   private:
    SwitchStorage* _storage = nullptr;
    DigitalInput* _input = nullptr;
    DigitalOutput* _output = nullptr;
    DigitalOutput* _output2 = nullptr;
    ActionButton* _button = nullptr;
  };

} // namespace Victor::Components

#endif // SwitchIO_h
