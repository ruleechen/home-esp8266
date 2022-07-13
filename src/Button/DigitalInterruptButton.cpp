#include "DigitalInterruptButton.h"

namespace Victor::Components {

  DigitalInterruptButton::DigitalInterruptButton(uint8_t inputPin, uint8_t inputTrueValue) {
    // interrupt
    _input = new DigitalInput(inputPin, inputTrueValue);
    _lastInputValue = _input->getValue();
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
    if (_contexts.size() > 1) {
      // every loop apply 2 interrupt contexts
      for (size_t i = 0; i + 1 < _contexts.size(); i = i + 2) {
        const auto item1 = _contexts[i];
        const auto item2 = _contexts[i + 1];
        _button->update(item1->inputValue, item1->timestamp);
        _button->update(item2->inputValue, item2->timestamp);
      }
      // reset
      for (auto context : _contexts) { delete context; }
      _contexts.clear();
      // _contexts.shrink_to_fit(); // already fixed to specified size
    }
  }

  DigitalInput* DigitalInterruptButton::_input = nullptr;
  volatile bool DigitalInterruptButton::_lastInputValue = false;
  std::vector<InterruptContext*> DigitalInterruptButton::_contexts = std::vector<InterruptContext*>(VICTOR_DIGITAL_INPUT_MAX_CHANGES);

  void IRAM_ATTR DigitalInterruptButton::_interruptHandler() {
    if (_contexts.size() < VICTOR_DIGITAL_INPUT_MAX_CHANGES) {
      const auto inputValue = _input->getValue();
      if (inputValue != _lastInputValue) {
        _lastInputValue = inputValue;
        _contexts.push_back(new InterruptContext({
          .inputValue = inputValue,
          .timestamp = millis(),
        }));
        console.log()
          .bracket("interrupt")
          .section(String(inputValue));
      }
    }
  }

} // namespace Victor::Components
