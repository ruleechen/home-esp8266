#include "SwitchStorage.h"

namespace Victor::Components {

  SwitchStorage::SwitchStorage(const char* filePath) : FileStorage(filePath) {
    _maxSize = 256;
  }

  void SwitchStorage::_serializeTo(const SwitchConfig& model, DynamicJsonDocument& doc) {
    // input pin
    const JsonArray inputArr = doc.createNestedArray(F("i"));
    inputArr[0] = model.inputPin;
    inputArr[1] = model.inputTrueValue;
    // output pin
    const JsonArray outputArr = doc.createNestedArray(F("o"));
    outputArr[0] = model.outputPin;
    outputArr[1] = model.outputTrueValue;
    // output2 pin
    const JsonArray output2Arr = doc.createNestedArray(F("o2"));
    output2Arr[0] = model.output2Pin;
    output2Arr[1] = model.output2TrueValue;
    // output state
    const JsonArray stateArr = doc.createNestedArray(F("state"));
    stateArr[0] = model.saveOutput ? 1 : 0;
    stateArr[1] = model.outputIsOn ? 1 : 0;
  }

  void SwitchStorage::_deserializeFrom(SwitchConfig& model, const DynamicJsonDocument& doc) {
    // input pin
    const auto inputArr = doc[F("i")];
    model.inputPin = inputArr[0];
    model.inputTrueValue = inputArr[1];
    // output pin
    const auto outputArr = doc[F("o")];
    model.outputPin = outputArr[0];
    model.outputTrueValue = outputArr[1];
    // output2 pin
    const auto output2Arr = doc[F("o2")];
    model.output2Pin = output2Arr[0];
    model.output2TrueValue = output2Arr[1];
    // output state
    const auto stateArr = doc[F("state")];
    model.saveOutput = stateArr[0] == 1;
    model.outputIsOn = stateArr[1] == 1;
  }

} // namespace Victor::Components
