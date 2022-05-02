#ifndef SwitchIO_h
#define SwitchIO_h

#include <DigitalOutput.h>
#include "SwitchModels.h"
#include "SwitchStorage.h"
#include "Button/ToothpickButton.h"

namespace Victor::Components {
  class SwitchIO {
   public:
    SwitchIO(SwitchStorage* storage);
    ~SwitchIO();
    void loop();
    void setOutputState(bool on);
    // events
    typedef std::function<void(ButtonAction action, bool outputValue)> TStateHandler;
    TStateHandler onInputChange = nullptr;

   private:
    SwitchStorage* _storage = nullptr;
    ToothpickButton* _input = nullptr;
    DigitalOutput* _output = nullptr;
    DigitalOutput* _output2 = nullptr;
  };

} // namespace Victor::Components

#endif // SwitchIO_h
