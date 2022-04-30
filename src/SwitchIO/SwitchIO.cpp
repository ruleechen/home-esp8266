#include "SwitchIO.h"

namespace Victor::Components {

  SwitchIO::SwitchIO(SwitchStorage* storage) {
    _storage = storage;
    const auto model = _storage->load();
    _input = new DigitalInput(model.inputPin, model.inputTrueValue);
    _output = new DigitalOutput(model.outputPin, model.outputTrueValue);
    if (model.output2Pin > -1) {
      _output2 = new DigitalOutput(model.output2Pin, model.output2TrueValue);
    }
    _inputState = getInputState();
    setOutputState(model.outputIsOn);
  }

  SwitchIO::~SwitchIO() {
    if (_storage != nullptr) {
      // delete _storage; // ignore deleting external component
      _storage = nullptr;
    }
    if (_input != nullptr) {
      delete _input;
      _input = nullptr;
    }
    if (_output != nullptr) {
      delete _output;
      _output = nullptr;
    }
    if (_output2 != nullptr) {
      delete _output2;
      _output2 = nullptr;
    }
  }

  void SwitchIO::loop() {
    const auto now = millis();
    if (now - _lastLoop > 100) {
      _lastLoop = now;
      const auto inputOn = getInputState();
      if (inputOn != _inputState) {
        _inputState = inputOn;
        if (onInputChange != nullptr && inputOn) {
          const auto outputState = _output->lastValue();
          onInputChange(!outputState); // toggle value
        }
      }
    }
  }

  bool SwitchIO::getInputState() {
    return _input->getValue();
  }

  void SwitchIO::setOutputState(bool on) {
    _output->setValue(on);
    if (_output2 != nullptr) {
      _output2->setValue(on);
    }
    // save output state
    auto model = _storage->load();
    if (model.saveOutput) {
      model.outputIsOn = on;
      _storage->save(model);
    }
  }

} // namespace Victor::Components
