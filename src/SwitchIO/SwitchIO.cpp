#include "SwitchIO.h"

namespace Victor::Components {

  SwitchIO::SwitchIO(SwitchStorage* storage) {
    _storage = storage;
    const auto model = _storage->load();
    setOutputState(model.outputIsOn);
    input = new DigitalInputButton(model.inputPin, model.inputTrueValue);
    _output = new DigitalOutput(model.outputPin, model.outputTrueValue);
    if (model.output2Pin > -1) {
      _output2 = new DigitalOutput(model.output2Pin, model.output2TrueValue);
    }
  }

  SwitchIO::~SwitchIO() {
    // ignore deleting external resource
    _storage = nullptr;
    if (input != nullptr) {
      delete input;
      input = nullptr;
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
    input->loop();
  }

  bool SwitchIO::getOutputState() {
    return _output->lastValue();
  }

  void SwitchIO::setOutputState(bool value) {
    _output->setValue(value);
    if (_output2 != nullptr) {
      _output2->setValue(value);
    }
    // save output state
    auto model = _storage->load();
    if (
      model.saveOutput &&
      model.outputIsOn != value
    ) {
      model.outputIsOn = value;
      _storage->save(model);
    }
  }

} // namespace Victor::Components
