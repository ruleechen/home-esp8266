#include "SwitchIO.h"

namespace Victor::Components {

  SwitchIO::SwitchIO(const char* settingFile) {
    _storage = new SwitchStorage(settingFile);
    const auto setting = _storage->load();
    input = new DigitalButton(setting->inputPin, setting->inputTrueValue);
    _output = new DigitalOutput(setting->outputPin, setting->outputTrueValue);
    if (setting->output2Pin > -1) {
      _output2 = new DigitalOutput(setting->output2Pin, setting->output2TrueValue);
    }
    setOutputState(setting->outputIsOn);
  }

  SwitchIO::~SwitchIO() {
    if (_storage != nullptr) {
      free(_storage);
      _storage = nullptr;
    }
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
    auto setting = _storage->load();
    if (
      setting->saveOutput &&
      setting->outputIsOn != value
    ) {
      setting->outputIsOn = value;
      _storage->save(setting);
    }
  }

} // namespace Victor::Components
