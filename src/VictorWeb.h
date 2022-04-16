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
#include "AppStorage.h"
#include "RadioStorage.h"
#include "VictorOTA.h"
#include "VictorWifi.h"

namespace Victor::Components {

  struct KeyValueModel {
    String key;
    String value;
  };

  class VictorWeb {
   public:
    VictorWeb(int port = 80);
    virtual ~VictorWeb();
    void setup();
    void loop();
    // server events
    typedef std::function<void()> TServerEventHandler;
    TServerEventHandler onRequestStart = nullptr;
    TServerEventHandler onRequestEnd = nullptr;
    // radio events
    typedef std::function<void(uint8_t index)> TRadioEmitHandler;
    TRadioEmitHandler onRadioEmit = nullptr;
    // service
    typedef std::function<void(std::vector<KeyValueModel>& items)> TServiceGetHandler;
    typedef std::function<void(const String& value)> TServicePostHandler;
    TServiceGetHandler onServiceGet = nullptr;
    TServicePostHandler onServicePost = nullptr;

   protected:
    ESP8266WebServer* _server = nullptr;
    ESP8266HTTPUpdateServer* _httpUpdater = nullptr;
    virtual void _registerHandlers();
    virtual void _solvePageTokens(String& html);
    void _sendHtml(const String& html);
    void _sendJson(DynamicJsonDocument doc);
    void _dispatchRequestStart();
    void _dispatchRequestEnd();
    void _handleIndexPage();
    void _handleSystemStatus();
    void _handleSystemReset();
    void _handleFileSystem();
    void _handleFiles();
    void _handleFileGet();
    void _handleFileSave();
    void _handleFileDelete();
    void _handleWifi();
    void _handleWifiList();
    void _handleWifiJoin();
    void _handleWifiJoinStatus();
    void _handleWifiMode();
    void _handleWifiReset();
    void _handleOta();
    void _handleOtaFire();
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
    void _handleServiceGet();
    void _handleServicePost();
  };
} // namespace Victor::Components

#endif // VictorWeb_h
