#include "VictorRadio.h"

namespace Victor::Components {

  VictorRadio::VictorRadio() { }

  VictorRadio::~VictorRadio() {
    onEmit = nullptr;
    onAction = nullptr;
    onCommand = nullptr;
    if (_ticker != nullptr) {
      delete _ticker;
      _ticker = nullptr;
    }
    if (_lastEmitted != nullptr) {
      delete _lastEmitted;
      _lastEmitted = nullptr;
    }
  }

  void VictorRadio::emit(const String& name) {
    const auto setting = radioStorage.load();
    if (setting->outputPin > -1) {
      for (const auto emit : setting->emits) {
        if (emit->name == name) {
          _handleEmit(emit);
          break;
        }
      }
    }
  }

  void VictorRadio::emit(uint8_t index) {
    const auto setting = radioStorage.load();
    if (setting->outputPin > -1 && index < setting->emits.size()) {
      const auto emit = setting->emits[index];
      _handleEmit(emit);
    }
  }

  void VictorRadio::_handleEmit(const RadioEmit* emit) {
    if (onEmit == nullptr || emit == nullptr) {
      return;
    }
    if (_ticker == nullptr) {
      _ticker = new Ticker();
    }
    const auto id = GlobalHelpers::randomString(4);
    _lastEmitted = new RadioEmit({
      .name = id,
      .value = emit->value,
      .channel = emit->channel,
      .press = emit->press,
    });
    switch (emit->press) {
      case PRESS_STATE_CLICK: {
        onEmit(_lastEmitted);
        break;
      }
      case PRESS_STATE_DOUBLE_CLICK: {
        onEmit(_lastEmitted);
        _ticker->once_ms(300, [this]{
          this->onEmit(this->_lastEmitted);
        });
        break;
      }
      case PRESS_STATE_LONG_PRESS: {
        onEmit(_lastEmitted);
        _ticker->once_ms(2000, [this]{
          this->onEmit(this->_lastEmitted);
        });
        break;
      }
      case PRESS_STATE_AWAIT:
      default: {
        break;
      }
    }
  }

  void VictorRadio::receive(const String& value, const uint8_t channel) {
    const auto message = _parseMessage(value, channel);
    radioStorage.broadcast(message);
    // press
    const auto timespan = message->timestamp - _lastReceived.timestamp;
    if (
      _lastReceived.id != message->id  ||
      timespan > VICTOR_RADIO_RESET_PRESS
    ) {
      RadioMessage empty {};
      _lastReceived = empty;
      _lastPressState = PRESS_STATE_AWAIT;
    }
    if (
      _lastPressState != PRESS_STATE_CLICK &&
      (_lastReceived.value != message->value || _lastReceived.channel != message->channel)
    ) {
      _handleReceived(message, PRESS_STATE_CLICK);
    } else if (
      _lastPressState != PRESS_STATE_DOUBLE_CLICK &&
      timespan >= VICTOR_RADIO_DOUBLE_CLICK_FROM &&
      timespan < VICTOR_RADIO_DOUBLE_CLICK_TO
    ) {
      _handleReceived(&_lastReceived, PRESS_STATE_DOUBLE_CLICK);
    } else if (
      _lastPressState != PRESS_STATE_LONG_PRESS &&
      (timespan >= VICTOR_RADIO_LONG_PRESS)
    ) {
      _handleReceived(&_lastReceived, PRESS_STATE_LONG_PRESS);
    }
  }

  void VictorRadio::_handleReceived(const RadioMessage* message, const RadioPressState press) {
    if (message == nullptr) {
      return;
    }
    // log states
    _lastReceived = *message;
    _lastPressState = press;
    console.log()
      .bracket(F("radio"))
      .section(F("detected pressed"), String(press));
    // check rules
    const auto setting = radioStorage.load();
    for (const auto rule : setting->rules) {
      if (
        rule->value == message->value &&
        rule->channel == message->channel &&
        rule->press == press
      ) {
        _proceedAction(rule);
      }
    }
    // check commands
    auto parsedCommand = _parseCommand(message);
    if (parsedCommand != nullptr) {
      for (const auto command : setting->commands) {
        if (
          command->entry == parsedCommand->entry &&
          command->action == parsedCommand->action &&
          command->press == press
        ) {
          _proceedCommand(parsedCommand);
        }
      }
    }
  }

  void VictorRadio::_proceedAction(const RadioRule* rule) {
    if (rule == nullptr) {
      return;
    }
    if (onAction != nullptr) {
      const auto handled = onAction(rule);
      if (handled) {
        return;
      }
    }
    switch (rule->action) {
      case RADIO_ACTION_WIFI_STA: {
        victorWifi.setMode(WIFI_STA);
        break;
      }
      case RADIO_ACTION_WIFI_STA_AP: {
        victorWifi.setMode(WIFI_AP_STA);
        break;
      }
      case RADIO_ACTION_WIFI_RESET: {
        victorWifi.reset();
        break;
      }
      case RADIO_ACTION_ESP_RESTART: {
        ESP.restart();
        break;
      }
      default: {
        break;
      }
    }
  }

  void VictorRadio::_proceedCommand(const RadioCommandParsed* command) {
    if (command == nullptr) {
      return;
    }
    if (onCommand != nullptr) {
      const auto handled = onCommand(command);
      if (handled) {
        return;
      }
    }
    switch (command->entry) {
      case ENTRY_WIFI: {
        switch (command->action) {
          case ENTRY_WIFI_JOIN: {
            const auto credential = GlobalHelpers::splitString(command->parameters, F("/"));
            if (credential.size() == 2) {
              const auto ssid = credential[0];
              const auto pswd = credential[1];
              victorWifi.join(ssid, pswd, false);
            }
            break;
          }
          case ENTRY_WIFI_MODE: {
            const auto hasAP = command->parameters.indexOf(F("ap")) > -1;
            const auto hasSTA = command->parameters.indexOf(F("sta")) > -1;
            const auto isOff = command->parameters == F("off");
            if (hasAP && hasSTA) { victorWifi.setMode(WIFI_AP_STA); }
            else if (hasAP) { victorWifi.setMode(WIFI_AP); }
            else if (hasSTA) { victorWifi.setMode(WIFI_STA); }
            else if (isOff) { victorWifi.setMode(WIFI_OFF); }
            break;
          }
          case ENTRY_WIFI_RESET: {
            victorWifi.reset();
            break;
          }
          case ENTRY_WIFI_NONE:
          default: {
            break;
          }
        }
        break;
      }

      case ENTRY_APP: {
        switch (command->action) {
          case ENTRY_APP_NAME: {
            auto setting = appStorage.load();
            setting->name = command->parameters;
            appStorage.save(setting);
            break;
          }
          case ENTRY_APP_OTA: {
            const auto otaType =
              command->parameters == F("all") ? OTA_ALL :
              command->parameters == F("fs") ? OTA_FS :
              command->parameters == F("sketch") ? OTA_SKETCH : OTA_SKETCH;
            victorOTA.trigger(otaType);
            break;
          }
          case ENTRY_APP_NONE:
          default: {
            break;
          }
        }
        break;
      }

      case ENTRY_ESP: {
        switch (command->action) {
          case ENTRY_ESP_RESTART: {
            ESP.restart();
            break;
          }
          case ENTRY_ESP_NONE:
          default: {
            break;
          }
        }
        break;
      }

      case ENTRY_NONE:
      default: {
        break;
      }
    }
  }

  RadioMessage* VictorRadio::_parseMessage(const String& value, const uint8_t channel) {
    // read id
    auto idPart = String(F("NONE"));
    auto valuePart = String(value);
    if (value.indexOf(F("!")) == 4) {
      idPart = value.substring(0, 4);
      valuePart = value.substring(5);
    }
    // message
    const auto message = new RadioMessage({
      .id = idPart,
      .value = valuePart,
      .channel = channel,
      .timestamp = millis(),
    });
    // ret
    return message;
  }

  RadioCommandParsed* VictorRadio::_parseCommand(const RadioMessage* message) {
    auto value = String(message->value); // clone
    value.replace(F(":"), F("-"));
    const auto parts = GlobalHelpers::splitString(value, F("-"));
    if (parts.size() < 2) {
      return nullptr;
    }
    auto command = new RadioCommandParsed();
    const auto entry = parts[0];
    const auto action = parts[1];
    if (entry == F("wifi")) {
      command->entry = ENTRY_WIFI;
      if (action == F("join")) {
        command->action = ENTRY_WIFI_JOIN;
      } else if (action == F("mode")) {
        command->action = ENTRY_WIFI_MODE;
      } else if (action == F("reset")) {
        command->action = ENTRY_WIFI_RESET;
      }
    } else if (entry == F("app")) {
      command->entry = ENTRY_APP;
      if (action == F("name")) {
        command->action = ENTRY_APP_NAME;
      } else if (action == F("ota")) {
        command->action = ENTRY_APP_OTA;
      }
    } else if (entry == F("esp")) {
      command->entry = ENTRY_ESP;
      if (action == F("restart")) {
        command->action = ENTRY_ESP_RESTART;
      }
    } else if (entry == F("boolean")) {
      command->entry = ENTRY_BOOLEAN;
      if (action == F("set")) {
        command->action = ENTRY_BOOLEAN_SET;
      } else if (action == F("toggle")) {
        command->action = ENTRY_BOOLEAN_TOGGLE;
      }
    }
    if (parts.size() >= 3) {
      const auto parameters = parts[2];
      command->parameters = parameters;
    }
    return command;
  }

} // namespace Victor::Components
