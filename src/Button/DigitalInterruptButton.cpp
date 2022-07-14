#include "DigitalInterruptButton.h"

namespace Victor::Components {

  DigitalInterruptButton::DigitalInterruptButton(uint8_t inputPin, uint8_t inputTrueValue) : DigitalButton(inputPin, inputTrueValue) {
    _inputRef = _input;
    _lastInputValue = _input->getValue();
    attachInterrupt(digitalPinToInterrupt(inputPin), _interruptHandler, CHANGE);
  }

  DigitalInterruptButton::~DigitalInterruptButton() {
    _inputRef = nullptr;
    // detachInterrupt(digitalPinToInterrupt(inputPin));
  }

  void DigitalInterruptButton::loop() {
    if (_contexts.size() > 1) { // at least 2
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
      _contexts.shrink_to_fit();
    }
  }

  DigitalInput* DigitalInterruptButton::_inputRef = nullptr;
  volatile bool DigitalInterruptButton::_lastInputValue = false;
  std::vector<InterruptContext*> DigitalInterruptButton::_contexts = std::vector<InterruptContext*>();

  void IRAM_ATTR DigitalInterruptButton::_interruptHandler() {
    if (_contexts.size() < VICTOR_DIGITAL_INPUT_MAX_CHANGES) {
      const auto inputValue = _inputRef->getValue();
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
