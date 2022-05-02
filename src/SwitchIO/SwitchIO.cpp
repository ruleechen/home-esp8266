#include "SwitchIO.h"

namespace Victor::Components {

  SwitchIO::SwitchIO(SwitchStorage* storage) {
    _storage = storage;
    const auto model = _storage->load();
    _input = new ToothpickButton(model.inputPin, model.inputTrueValue);
    _output = new DigitalOutput(model.outputPin, model.outputTrueValue);
    if (model.output2Pin > -1) {
      _output2 = new DigitalOutput(model.output2Pin, model.output2TrueValue);
    }
    _input->onClick = [&](ButtonAction action) {
      const auto outputValue = _output->lastValue();
      onInputChange(action, outputValue);
    };
    setOutputState(model.outputIsOn);
  }

  SwitchIO::~SwitchIO() {
    // ignore deleting external resource
    _storage = nullptr;
    onInputChange = nullptr;
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
    _input->loop();
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
