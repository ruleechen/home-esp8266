#include "SwitchIO.h"

namespace Victor::Components {

  SwitchIO::SwitchIO(SwitchStorage* storage) {
    _storage = storage;
    const auto model = _storage->load();
    setOutputState(model.outputIsOn);
    _input = new DigitalInput(model.inputPin, model.inputTrueValue);
    _output = new DigitalOutput(model.outputPin, model.outputTrueValue);
    if (model.output2Pin > -1) {
      _output2 = new DigitalOutput(model.output2Pin, model.output2TrueValue);
    }
    _button = new ActionButton(_input->getValue());
    _button->onAction = [&](ButtonAction action) {
      if (onInputAction != nullptr) {
        onInputAction(action);
      }
    };
  }

  SwitchIO::~SwitchIO() {
    // ignore deleting external resource
    _storage = nullptr;
    onInputAction = nullptr;
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
    if (_button != nullptr) {
      delete _button;
      _button = nullptr;
    }
  }

  void SwitchIO::loop() {
    _button->update(_input->getValue());
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
