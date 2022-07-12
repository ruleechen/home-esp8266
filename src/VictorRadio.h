#ifndef VictorRadio_h
#define VictorRadio_h

#include <Arduino.h>
#include <Ticker.h>
#include "GlobalHelpers.h"
#include "Console.h"
#include "VictorOTA.h"
#include "VictorWifi.h"
#include "AppMain/AppStorage.h"
#include "RadioStorage.h"

#ifndef VICTOR_RADIO_DOUBLE_CLICK_FROM
#define VICTOR_RADIO_DOUBLE_CLICK_FROM 400
#endif

#ifndef VICTOR_RADIO_DOUBLE_CLICK_TO
#define VICTOR_RADIO_DOUBLE_CLICK_TO 800
#endif

#ifndef VICTOR_RADIO_LONG_PRESS
#define VICTOR_RADIO_LONG_PRESS 2000
#endif

#ifndef VICTOR_RADIO_RESET_PRESS
#define VICTOR_RADIO_RESET_PRESS 2500
#endif

namespace Victor::Components {
  class VictorRadio {
   public:
    VictorRadio();
    ~VictorRadio();
    void emit(const String& name);
    void emit(uint8_t index);
    typedef std::function<void(const RadioEmit* emit)> TRadioEmit;
    TRadioEmit onEmit = nullptr;
    void receive(const String& value, const uint8_t channel);
    typedef std::function<bool(const RadioRule* rule)> TRadioAction;
    TRadioAction onAction = nullptr;
    typedef std::function<bool(const RadioCommandParsed* command)> TRadioCommand;
    TRadioCommand onCommand = nullptr;

   private:
    Ticker* _ticker = nullptr;
    RadioEmit* _lastEmitted = nullptr;
    RadioPressState _lastPressState = PRESS_STATE_AWAIT;
    void _handleEmit(const RadioEmit* emit);
    void _fireOnEmit(const RadioEmit* emit, uint32_t ms);
    void _handleReceived(const RadioMessage* message, const RadioPressState press);
    void _proceedAction(const RadioRule* rule);
    void _proceedCommand(const RadioCommandParsed* command);
    static RadioMessage* _parseMessage(const String& value, const uint8_t channel);
    static RadioCommandParsed* _parseCommand(const String& value);
  };
} // namespace Victor::Components

#endif // VictorRadio_h
