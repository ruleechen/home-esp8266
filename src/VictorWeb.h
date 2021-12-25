#ifndef VictorWeb_h
#define VictorWeb_h

#include <functional>
#include <vector>
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "Commons.h"
#include "AppStorage.h"
#include "RadioStorage.h"
#include "VictorOTA.h"
#include "VictorWifi.h"

namespace Victor::Components {
  class VictorWeb {
   public:
    VictorWeb(int port = 80);
    virtual ~VictorWeb();
    void setup();
    void loop();
    // server events
    typedef std::function<void()> TServerEventHandler;
    TServerEventHandler onRequestStart;
    TServerEventHandler onRequestEnd;
    // radio events
    typedef std::function<void(int index)> TRadioEmitHandler;
    TRadioEmitHandler onRadioEmit;
    // service
    typedef std::function<void(std::vector<KeyValueModel> items)> TServiceStateHandler;
    TServiceStateHandler onGetServiceState;
    TServerEventHandler onResetService;

   protected:
    ESP8266WebServer* _server = NULL;
    ESP8266HTTPUpdateServer* _httpUpdater = NULL;
    virtual void _registerHandlers();
    virtual void _solvePageTokens(String& html);
    void _sendHtml(const String& html);
    void _sendJson(DynamicJsonDocument doc);
    void _dispatchRequestStart();
    void _dispatchRequestEnd();
    void _handleIndexPage();
    void _handleStatus();
    void _handleFileSystem();
    void _handleFiles();
    void _handleFileGet();
    void _handleFileSave();
    void _handleFileDelete();
    void _handleWifi();
    void _handleWifiList();
    void _handleWifiJoin();
    void _handleWifiMode();
    void _handleOta();
    void _handleOtaFire();
    void _handleReset();
    void _handleNotFound();
    void _handleRadioGet();
    void _handleRadioSave();
    void _handleRadioEmitGet();
    void _handleRadioEmitSave();
    void _handleRadioEmitSend();
    void _handleRadioRuleGet();
    void _handleRadioRuleSave();
    void _handleRadioCommandGet();
    void _handleRadioCommandSave();
    void _handleServiceState();
    void _handleServiceReset();
  };
} // namespace Victor::Components

#endif // VictorWeb_h
