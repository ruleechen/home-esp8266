#ifndef VictorRadio_h
#define VictorRadio_h

#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include "GlobalHelpers.h"
#include "Console.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "AppStorage.h"
#include "RadioStorage.h"

// timespan in millianseconds
#define DOUBLE_CLICK_TIMESPAN_FROM 400
#define DOUBLE_CLICK_TIMESPAN_TO 800
#define LONG_PRESS_TIMESPAN 2000
#define RESET_PRESS_TIMESPAN 2500

namespace Victor::Components {
  class VictorRadio {
    typedef std::function<void(const RadioEmit&)> TRadioEmit;
    typedef std::function<bool(const RadioRule&)> TRadioAction;
    typedef std::function<bool(const RadioCommandParsed&)> TRadioCommand;

   public:
    VictorRadio();
    ~VictorRadio();
    void emit(String name);
    void emit(uint8_t index);
    TRadioEmit onEmit;
    void receive(String value, int channel);
    TRadioAction onAction;
    TRadioCommand onCommand;

   private:
    Ticker* _ticker = nullptr;
    RadioEmit _lastEmitted = {};
    RadioMessage _lastReceived = {};
    RadioPressState _lastPressState = PressStateAwait;
    void _handleEmit(const RadioEmit& emit);
    void _handleReceived(const RadioMessage& message, RadioPressState press);
    void _proceedAction(const RadioRule& rule);
    void _proceedCommand(const RadioCommandParsed& command);
    static RadioCommandParsed _parseCommand(const RadioMessage& message);
  };
} // namespace Victor::Components

#endif // VictorRadio_h
