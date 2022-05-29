#include "DigitalInterruptButton.h"

namespace Victor::Components {

  DigitalInterruptButton::DigitalInterruptButton(uint8_t inputPin, uint8_t inputTrueValue) {
    // interrupt
    _input = new DigitalInput(inputPin, inputTrueValue);
    attachInterrupt(digitalPinToInterrupt(inputPin), _interruptHandler, CHANGE);
    // button
    _button = new ActionButton(_input->getValue());
    _button->onAction = [&](ButtonAction action) {
      if (onAction != nullptr) {
        onAction(action);
      }
    };
  }

  DigitalInterruptButton::~DigitalInterruptButton() {
    onAction = nullptr;
    if (_input != nullptr) {
      delete _input;
      _input = nullptr;
    }
    if (_button != nullptr) {
      delete _button;
      _button = nullptr;
    }
  }

  void DigitalInterruptButton::loop() {
    if (_contexts.size() > 0) {
      // execute
      for (size_t i = 0; i < _contexts.size(); i++) {
        const auto item = _contexts[i];
        _button->update(item.inputValue, item.timestamp);
      }
      // reset interrupt
      _contexts.clear();
    }
  }

  DigitalInput* DigitalInterruptButton::_input = nullptr;
  std::vector<InterruptContext> DigitalInterruptButton::_contexts = {};

  void IRAM_ATTR DigitalInterruptButton::_interruptHandler() {
    if (_contexts.size() < VICTOR_DIGITAL_INPUT_MAX_CHANGES) {
      const InterruptContext context = {
        .inputValue = _input->getValue(),
        .timestamp = millis(),
      };
      _contexts.push_back(context);
      console.log()
        .bracket("interrupt")
        .section(String(context.inputValue));
    }
  }

} // namespace Victor::Components
