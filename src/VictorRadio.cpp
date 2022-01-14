#include "VictorRadio.h"

namespace Victor::Components {

  VictorRadio::VictorRadio() { }

  VictorRadio::~VictorRadio() {
    if (_ticker) {
      delete _ticker;
      _ticker = nullptr;
    }
  }

  void VictorRadio::emit(const String& name) {
    const auto model = radioStorage.load();
    if (model.outputPin > -1) {
      for (const auto& emit : model.emits) {
        if (emit.name == name) {
          _handleEmit(emit);
          break;
        }
      }
    }
  }

  void VictorRadio::emit(uint8_t index) {
    const auto model = radioStorage.load();
    if (model.outputPin > -1 && index < model.emits.size()) {
      const auto emit = model.emits[index];
      _handleEmit(emit);
    }
  }

  void VictorRadio::_handleEmit(const RadioEmit& emit) {
    if (!onEmit) {
      return;
    }
    if (_ticker == nullptr) {
      _ticker = new Ticker();
    }
    const auto id = GlobalHelpers::randomString(4);
    _lastEmitted = {
      .name = id,
      .value = emit.value,
      .channel = emit.channel,
      .press = emit.press,
    };
    switch (emit.press) {
      case PressStateClick: {
        onEmit(_lastEmitted);
        break;
      }
      case PressStateDoubleClick: {
        onEmit(_lastEmitted);
        _ticker->once_ms(300, [this]{
          this->onEmit(this->_lastEmitted);
        });
        break;
      }
      case PressStateLongPress: {
        onEmit(_lastEmitted);
        _ticker->once_ms(2000, [this]{
          this->onEmit(this->_lastEmitted);
        });
        break;
      }
      case PressStateAwait:
      default: {
        break;
      }
    }
  }

  void VictorRadio::receive(String value, uint8_t channel) {
    // read id
    auto id = String(F("none"));
    if (value.indexOf(F("!")) == 4) {
      id = value.substring(0, 4);
      value = value.substring(5);
    }
    // message
    const RadioMessage message = {
      .id = id,
      .value = value,
      .channel = channel,
      .timestamp = millis(),
    };
    // broadcase state
    radioStorage.broadcast(message);
    // press
    const auto timespan = message.timestamp - _lastReceived.timestamp;
    if (
      _lastReceived.id != message.id  ||
      timespan > RESET_PRESS_TIMESPAN
    ) {
      RadioMessage empty {};
      _lastReceived = empty;
      _lastPressState = PressStateAwait;
    }
    if (
      _lastPressState != PressStateClick &&
      (_lastReceived.value != message.value || _lastReceived.channel != message.channel)
    ) {
      _handleReceived(message, PressStateClick);
    } else if (
      _lastPressState != PressStateDoubleClick &&
      timespan >= DOUBLE_CLICK_TIMESPAN_FROM &&
      timespan < DOUBLE_CLICK_TIMESPAN_TO
    ) {
      _handleReceived(_lastReceived, PressStateDoubleClick);
    } else if (
      _lastPressState != PressStateLongPress &&
      (timespan >= LONG_PRESS_TIMESPAN)
    ) {
      _handleReceived(_lastReceived, PressStateLongPress);
    }
  }

  void VictorRadio::_handleReceived(const RadioMessage& message, RadioPressState press) {
    // log states
    _lastReceived = message;
    _lastPressState = press;
    console.log()
      .bracket(F("radio"))
      .section(F("detected pressed"), String(press));
    // check rules
    const auto model = radioStorage.load();
    for (const auto& rule : model.rules) {
      if (
        rule.value == message.value &&
        rule.channel == message.channel &&
        rule.press == press
      ) {
        _proceedAction(rule);
      }
    }
    // check commands
    auto parsedCommand = _parseCommand(message);
    for (const auto& command : model.commands) {
      if (
        command.entry == parsedCommand.entry &&
        command.action == parsedCommand.action &&
        command.press == press
      ) {
        parsedCommand.serviceId = command.serviceId;
        _proceedCommand(parsedCommand);
      }
    }
  }

  void VictorRadio::_proceedAction(const RadioRule& rule) {
    if (onAction) {
      const auto handled = onAction(rule);
      if (handled) {
        return;
      }
    }
    switch (rule.action) {
      case RadioActionWiFiSta: {
        victorWifi.setMode(WIFI_STA);
        break;
      }
      case RadioActionWiFiStaAp: {
        victorWifi.setMode(WIFI_AP_STA);
        break;
      }
      case RadioActionWiFiReset: {
        victorWifi.reset();
        break;
      }
      case RadioActionEspRestart: {
        ESP.restart();
        break;
      }
      default: {
        break;
      }
    }
  }

  void VictorRadio::_proceedCommand(const RadioCommandParsed& command) {
    if (onCommand) {
      const auto handled = onCommand(command);
      if (handled) {
        return;
      }
    }
    switch (command.entry) {
      case EntryWifi: {
        switch (command.action) {
          case EntryWifiJoin: {
            const auto credential = GlobalHelpers::splitString(command.parameters, F("/"));
            if (credential.size() == 2) {
              const auto ssid = credential[0];
              const auto password = credential[1];
              victorWifi.join(ssid, password, false);
            }
            break;
          }
          case EntryWifiMode: {
            const auto hasAP = command.parameters.indexOf(F("ap")) > -1;
            const auto hasSTA = command.parameters.indexOf(F("sta")) > -1;
            const auto isOff = command.parameters == F("off");
            if (hasAP && hasSTA) { victorWifi.setMode(WIFI_AP_STA); }
            else if (hasAP) { victorWifi.setMode(WIFI_AP); }
            else if (hasSTA) { victorWifi.setMode(WIFI_STA); }
            else if (isOff) { victorWifi.setMode(WIFI_OFF); }
            break;
          }
          case EntryWifiReset: {
            victorWifi.reset();
            break;
          }
          case EntryWifiNone:
          default: {
            break;
          }
        }
        break;
      }

      case EntryApp: {
        switch (command.action) {
          case EntryAppName: {
            auto model = appStorage.load();
            model.name = command.parameters;
            appStorage.save(model);
            break;
          }
          case EntryAppOTA: {
            const auto otaType =
              command.parameters == F("all") ? VOta_All :
              command.parameters == F("fs") ? VOta_FileSystem :
              command.parameters == F("sketch") ? VOta_Sketch : VOta_Sketch;
            victorOTA.trigger(otaType);
            break;
          }
          case EntryAppNone:
          default: {
            break;
          }
        }
        break;
      }

      case EntryEsp: {
        switch (command.action) {
          case EntryEspRestart: {
            ESP.restart();
            break;
          }
          case EntryEspNone:
          default: {
            break;
          }
        }
        break;
      }

      case EntryNone:
      default: {
        break;
      }
    }
  }

  RadioCommandParsed VictorRadio::_parseCommand(const RadioMessage& message) {
    RadioCommandParsed command;
    auto value = String(message.value); // clone
    value.replace(F(":"), F("-"));
    const auto parts = GlobalHelpers::splitString(value, F("-"));
    if (parts.size() >= 2) {
      const auto entry = parts[0];
      const auto action = parts[1];
      if (entry == F("wifi")) {
        command.entry = EntryWifi;
        if (action == F("join")) {
          command.action = EntryWifiJoin;
        } else if (action == F("mode")) {
          command.action = EntryWifiMode;
        } else if (action == F("reset")) {
          command.action = EntryWifiReset;
        }
      } else if (entry == F("app")) {
        command.entry = EntryApp;
        if (action == F("name")) {
          command.action = EntryAppName;
        } else if (action == F("ota")) {
          command.action = EntryAppOTA;
        }
      } else if (entry == F("esp")) {
        command.entry = EntryEsp;
        if (action == F("restart")) {
          command.action = EntryEspRestart;
        }
      } else if (entry == F("boolean")) {
        command.entry = EntryBoolean;
        if (action == F("set")) {
          command.action = EntryBooleanSet;
        } else if (action == F("toggle")) {
          command.action = EntryBooleanToggle;
        }
      }
      if (parts.size() >= 3) {
        const auto parameters = parts[2];
        command.parameters = parameters;
      }
    }
    return command;
  }

} // namespace Victor::Components
