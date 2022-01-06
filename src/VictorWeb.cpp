#include "VictorWeb.h"

namespace Victor::Components {

  VictorWeb::VictorWeb(int port) {
    _server = new ESP8266WebServer(port);
  }

  VictorWeb::~VictorWeb() {
    if (_server) {
      _server->stop();
      delete _server;
      _server = NULL;
    }
    if (_httpUpdater) {
      delete _httpUpdater;
      _httpUpdater = NULL;
    }
  }

  void VictorWeb::setup() {
    _registerHandlers();
    const auto model = appStorage.load();
    if (model.overTheWeb) {
      _httpUpdater = new ESP8266HTTPUpdateServer();
      _httpUpdater->setup(_server, F("/update"));
    }
    _server->begin();
  }

  void VictorWeb::loop() {
    _server->handleClient();
  }

  void VictorWeb::_registerHandlers() {
    _server->serveStatic("/style.min.css", LittleFS, "/web/style.min.css", "max-age=1800"); // 1800 seconds --> 0.5 hours
    _server->serveStatic("/mithril.min.js", LittleFS, "/web/mithril.min.js", "max-age=1800");
    _server->serveStatic("/app.min.js", LittleFS, "/web/app.min.js");
    _server->on(F("/"), HTTP_GET, std::bind(&VictorWeb::_handleIndexPage, this));
    _server->on(F("/system/status"), HTTP_GET, std::bind(&VictorWeb::_handleSystemStatus, this));
    _server->on(F("/system/reset"), HTTP_POST, std::bind(&VictorWeb::_handleSystemReset, this));
    _server->on(F("/fs"), HTTP_GET, std::bind(&VictorWeb::_handleFileSystem, this));
    _server->on(F("/files"), HTTP_GET, std::bind(&VictorWeb::_handleFiles, this));
    _server->on(F("/file"), HTTP_GET, std::bind(&VictorWeb::_handleFileGet, this));
    _server->on(F("/file"), HTTP_POST, std::bind(&VictorWeb::_handleFileSave, this));
    _server->on(F("/file"), HTTP_DELETE, std::bind(&VictorWeb::_handleFileDelete, this));
    _server->on(F("/wifi"), HTTP_GET, std::bind(&VictorWeb::_handleWifi, this));
    _server->on(F("/wifi/list"), HTTP_GET, std::bind(&VictorWeb::_handleWifiList, this));
    _server->on(F("/wifi/join"), HTTP_POST, std::bind(&VictorWeb::_handleWifiJoin, this));
    _server->on(F("/wifi/mode"), HTTP_POST, std::bind(&VictorWeb::_handleWifiMode, this));
    _server->on(F("/wifi/reset"), HTTP_POST, std::bind(&VictorWeb::_handleWifiReset, this));
    _server->on(F("/ota"), HTTP_GET, std::bind(&VictorWeb::_handleOta, this));
    _server->on(F("/ota/fire"), HTTP_POST, std::bind(&VictorWeb::_handleOtaFire, this));
    _server->onNotFound(std::bind(&VictorWeb::_handleNotFound, this));
    _server->on(F("/radio"), HTTP_GET, std::bind(&VictorWeb::_handleRadioGet, this));
    _server->on(F("/radio"), HTTP_POST, std::bind(&VictorWeb::_handleRadioSave, this));
    _server->on(F("/radio/emit"), HTTP_GET, std::bind(&VictorWeb::_handleRadioEmitGet, this));
    _server->on(F("/radio/emit"), HTTP_POST, std::bind(&VictorWeb::_handleRadioEmitSave, this));
    _server->on(F("/radio/emit/send"), HTTP_POST, std::bind(&VictorWeb::_handleRadioEmitSend, this));
    _server->on(F("/radio/rule"), HTTP_GET, std::bind(&VictorWeb::_handleRadioRuleGet, this));
    _server->on(F("/radio/rule"), HTTP_POST, std::bind(&VictorWeb::_handleRadioRuleSave, this));
    _server->on(F("/radio/command"), HTTP_GET, std::bind(&VictorWeb::_handleRadioCommandGet, this));
    _server->on(F("/radio/command"), HTTP_POST, std::bind(&VictorWeb::_handleRadioCommandSave, this));
    _server->on(F("/service/state"), HTTP_GET, std::bind(&VictorWeb::_handleServiceState, this));
    _server->on(F("/service/reset"), HTTP_POST, std::bind(&VictorWeb::_handleServiceReset, this));
  }

  void VictorWeb::_solvePageTokens(String& html) {
    html.replace(F("{productName}"), VICTOR_FIRMWARE_NAME);
    html.replace(F("{version}"), String(UNIX_TIME));
  }

  void VictorWeb::_sendHtml(const String& html) {
    _server->send(200, F("text/html"), html);
  }

  void VictorWeb::_sendJson(DynamicJsonDocument res) {
    String buf;
    serializeJson(res, buf);
    _server->send(200, F("application/json"), buf);
  }

  void VictorWeb::_dispatchRequestStart() {
    _server->sendHeader(F("Connection"), F("keep-alive")); // close / keep-alive
    // set cross origin
    _server->sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    _server->sendHeader(F("Access-Control-Max-Age"), F("600")); // 10 minutes
    _server->sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
    _server->sendHeader(F("Access-Control-Allow-Headers"), F("*"));
    // fire event
    if (onRequestStart) {
      onRequestStart();
    }
  }

  void VictorWeb::_dispatchRequestEnd() {
    if (onRequestEnd) {
      onRequestEnd();
    }
  }

  void VictorWeb::_handleIndexPage() {
    _dispatchRequestStart();
    const auto path = String(F("/web/index.htm"));
    auto file = LittleFS.open(path, "r");
    auto html = file.readString();
    file.close();
    _solvePageTokens(html);
    html.replace(F("{appendHead}"), "");
    html.replace(F("{appendBody}"), "");
    _sendHtml(html);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleSystemStatus() {
    _dispatchRequestStart();
    // res
    DynamicJsonDocument res(1024);
    // status
    res[F("millis")] = millis();
    res[F("resetReason")] = ESP.getResetReason();
    res[F("freeStack")] = ESP.getFreeContStack();
    res[F("freeHeap")] = ESP.getFreeHeap();
    res[F("maxFreeBlockSize")] = ESP.getMaxFreeBlockSize();
    res[F("heapFragmentation")] = ESP.getHeapFragmentation();
    // hardware
    res[F("chipId")] = ESP.getChipId();
    res[F("cupFreqMHz")] = ESP.getCpuFreqMHz();
    res[F("flashId")] = ESP.getFlashChipId();
    res[F("flashSize")] = ESP.getFlashChipSize(); // Sketch thinks Flash RAM size is
    res[F("flashSizeReal")] = ESP.getFlashChipRealSize(); // Actual size based on chip Id
    res[F("flashSpeedMHz")] = ESP.getFlashChipSpeed() / 1000000;
    // software
    res[F("sketchMD5")] = ESP.getSketchMD5();
    res[F("sketchSize")] = ESP.getSketchSize();
    res[F("sketchFreeSpace")] = ESP.getFreeSketchSpace();
    res[F("sdkVersion")] = ESP.getSdkVersion();
    res[F("coreVersion")] = ESP.getCoreVersion();
    res[F("firmwareVersion")] = VICTOR_FIRMWARE_VERSION;
    res[F("unixTime")] = UNIX_TIME;
    // end
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleSystemReset() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(64);
    deserializeJson(payload, payloadJson);
    // read
    const auto values = String(payload[F("values")]);
    // action
    if (values.indexOf(F("1")) >= 0) {
      // sdk_system_restart();
      ESP.restart();
    }
    if (values.indexOf(F("2")) >= 0) {
      ESP.reset();
    }
    if (values.indexOf(F("3")) >= 0) {
      ESP.eraseConfig();
    }
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleFileSystem() {
    _dispatchRequestStart();
    FSInfo fsInfo;
    DynamicJsonDocument res(512);
    if (LittleFS.info(fsInfo)) {
      res[F("totalBytes")] = fsInfo.totalBytes;
      res[F("usedBytes")] = fsInfo.usedBytes;
      res[F("maxPathLength")] = fsInfo.maxPathLength;
      res[F("maxOpenFiles")] = fsInfo.maxOpenFiles;
      res[F("blockSize")] = fsInfo.blockSize;
      res[F("pageSize")] = fsInfo.pageSize;
    } else {
      const auto error = String(F("read fs info failed"));
      res[F("error")] = error;
      console.error().bracket(F("fs")).section(error);
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleFiles() {
    _dispatchRequestStart();
    DynamicJsonDocument res(1024);
    // load files
    // https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
    const JsonArray filesArr = res.createNestedArray(F("files"));
    std::function<void(String)> loopFiles;
    loopFiles = [&](String path)->void {
      Dir dir = LittleFS.openDir(path);
      while(dir.next()) {
        const auto fullName = path + dir.fileName();
        if (dir.isDirectory()) {
          loopFiles(fullName + F("/"));
        } else if (dir.isFile()) {
          const JsonObject fileObj = filesArr.createNestedObject();
          fileObj[F("path")] = fullName;
          fileObj[F("size")] = dir.fileSize();
        }
      }
    };
    loopFiles(F("/"));
    // send
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleFileGet() {
    _dispatchRequestStart();
    DynamicJsonDocument res(2048); // 2k
    const auto path = _server->arg(F("path"));
    auto file = LittleFS.open(path, "r");
    if (file) {
      res[F("size")] = file.size();
      res[F("name")] = String(file.name());
      res[F("content")] = file.readString();
      file.close();
    } else {
      const auto error = String(F("failed to open file"));
      res[F("error")] = error;
      console.error().bracket(F("fs")).section(error);
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleFileSave() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(2048); // 2k
    deserializeJson(payload, payloadJson);
    // write
    DynamicJsonDocument res(64);
    const auto path = _server->arg(F("path"));
    auto file = LittleFS.open(path, "w");
    if (file) {
      const auto content = String(payload[F("content")]);
      file.print(content);
      file.close();
      res[F("message")] = String(F("success"));
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleFileDelete() {
    _dispatchRequestStart();
    const auto path = _server->arg(F("path"));
    LittleFS.remove(path);
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleWifi() {
    _dispatchRequestStart();
    // wifi
    const auto mode = WiFi.getMode();
    auto strMode = String(F("OFF"));
    if (mode == WIFI_STA) {
      strMode = F("STA");
    } else if (mode == WIFI_AP) {
      strMode = F("AP");
    } else if (mode == WIFI_AP_STA) {
      strMode = F("AP_STA");
    }
    // station
    const auto staMacAddress = WiFi.macAddress();
    const auto isStaEnabled = ((mode & WIFI_STA) != 0);
    auto staAddress = String("");
    if (isStaEnabled) {
      IPAddress localIP = WiFi.localIP();
      if (localIP) {
        staAddress = localIP.toString();
      }
    }
    // access point
    const auto apMacAddress = WiFi.softAPmacAddress();
    const auto isApEnabled = ((mode & WIFI_AP) != 0);
    auto apAddress = String("");
    if (isApEnabled) {
      IPAddress apIP = WiFi.softAPIP();
      if (apIP) {
        apAddress = apIP.toString();
      }
    }
    // res
    DynamicJsonDocument res(512);
    // wifi
    res[F("hostName")] = victorWifi.getHostName();
    res[F("mdns")] = MDNS.isRunning();
    res[F("mode")] = strMode;
    res[F("joined")] = WiFi.SSID();
    res[F("rssi")] = WiFi.RSSI();
    res[F("staAddress")] = staAddress;
    res[F("staMacAddress")] = staMacAddress;
    res[F("apAddress")] = apAddress;
    res[F("apMacAddress")] = apMacAddress;
    // end
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleWifiList() {
    _dispatchRequestStart();
    DynamicJsonDocument res(1024);
    res[F("bssid")] = WiFi.BSSIDstr();
    const JsonArray apArr = res.createNestedArray(F("founds"));
    const auto count = WiFi.scanNetworks();
    for (int8_t i = 0; i < count; ++i) {
      const JsonObject apObj = apArr.createNestedObject();
      apObj[F("bssid")] = WiFi.BSSIDstr(i); // Basic Service Set Identifiers (a MAC)
      apObj[F("ssid")] = WiFi.SSID(i); // Service Set Identifier
      apObj[F("rssi")] = WiFi.RSSI(i); // Received Signal Strength Indicator
      apObj[F("channel")] = WiFi.channel(i);
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleWifiJoin() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(128);
    deserializeJson(payload, payloadJson);
    // read
    const auto bssid = String(payload[F("bssid")]);
    const auto ssid = String(payload[F("ssid")]);
    const auto password = String(payload[F("password")]);
    const int32_t channel = payload[F("channel")];
    // res
    DynamicJsonDocument res(64);
    if (!ssid || ssid == "") {
      res[F("error")] = String(F("Please select wifi to join"));
    } else {
      victorWifi.join(ssid, password, channel, (uint8_t*)bssid.c_str());
      victorWifi.waitForConnected();
      const auto isConnected = WiFi.isConnected();
      console.log().bracket(F("wifi")).section(F("connected"), String(isConnected));
      if (isConnected) {
        res[F("ip")] = WiFi.localIP().toString();
      } else {
        res[F("error")] = String(F("failed"));
      }
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleWifiMode() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(128);
    deserializeJson(payload, payloadJson);
    // read
    const auto mode = String(payload[F("mode")]);
    // set
    if (mode == "STA") {
      WiFi.mode(WIFI_STA);
    } else if (mode == "AP") {
      WiFi.mode(WIFI_AP);
    } else if (mode == "AP_STA") {
      WiFi.mode(WIFI_AP_STA);
    }
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleWifiReset() {
    _dispatchRequestStart();
    // set
    victorWifi.reset();
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleOta() {
    _dispatchRequestStart();
    DynamicJsonDocument res(512);
    res[F("flashSize")] = ESP.getFlashChipSize(); // Sketch thinks Flash RAM size is
    res[F("flashSizeReal")] = ESP.getFlashChipRealSize(); // Actual size based on chip Id
    res[F("sketchSize")] = ESP.getSketchSize();
    res[F("sketchFreeSpace")] = ESP.getFreeSketchSpace();
    res[F("otaVersion")] = victorOTA.getCurrentVersion();
    res[F("otaNewVersion")] = victorOTA.checkNewVersion();
    res[F("overTheWeb")] = _httpUpdater != NULL;
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleOtaFire() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(64);
    deserializeJson(payload, payloadJson);
    // read
    const auto version = String(payload[F("version")]);
    const auto otaType = String(payload[F("otaType")]);
    const auto type =
      otaType == F("all") ? VOta_All :
      otaType == F("fs") ? VOta_FileSystem :
      otaType == F("sketch") ? VOta_Sketch : VOta_Sketch;
    // update
    victorOTA.update(version, type);
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleNotFound() {
    _dispatchRequestStart();
    DynamicJsonDocument res(512);
    res[F("method")] = (_server->method() == HTTP_GET) ? String(F("GET")) : String(F("POST"));
    res[F("uri")] = _server->uri();
    res[F("error")] = String(F("Resource Not Found"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioGet() {
    _dispatchRequestStart();
    DynamicJsonDocument res(512);
    const auto model = radioStorage.load();
    res[F("millis")] = millis();
    res[F("inputPin")] = model.inputPin;
    res[F("outputPin")] = model.outputPin;
    // last received
    const auto lastReceived = radioStorage.getLastReceived();
    const JsonObject lastReceivedObj = res.createNestedObject(F("lastReceived"));
    lastReceivedObj[F("value")] = lastReceived.value;
    lastReceivedObj[F("channel")] = lastReceived.channel;
    lastReceivedObj[F("timestamp")] = lastReceived.timestamp;
    // end
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioSave() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(64);
    deserializeJson(payload, payloadJson);
    // read
    const auto inputPin = String(payload[F("inputPin")]);
    const auto outputPin = String(payload[F("outputPin")]);
    // action
    auto model = radioStorage.load();
    model.inputPin = inputPin.toInt();
    model.outputPin = outputPin.toInt();
    radioStorage.save(model);
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioEmitGet() {
    _dispatchRequestStart();
    DynamicJsonDocument res(1024 + 512);
    // emits
    const auto model = radioStorage.load();
    const JsonArray emitArr = res.createNestedArray(F("emits"));
    for (const auto& emit : model.emits) {
      const JsonObject emitObj = emitArr.createNestedObject();
      emitObj[F("name")] = emit.name;
      emitObj[F("value")] = emit.value;
      emitObj[F("channel")] = emit.channel;
      emitObj[F("press")] = emit.press;
    }
    // last received
    const auto lastReceived = radioStorage.getLastReceived();
    const JsonObject lastReceivedObj = res.createNestedObject(F("lastReceived"));
    lastReceivedObj[F("value")] = lastReceived.value;
    lastReceivedObj[F("channel")] = lastReceived.channel;
    // end
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioEmitSave() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(1024 + 512);
    deserializeJson(payload, payloadJson);
    // read
    const auto emitItems = payload[F("emits")];
    // save
    auto model = radioStorage.load();
    model.emits.clear();
    for (size_t i = 0; i < emitItems.size(); i++) {
      const auto item = emitItems[i];
      model.emits.push_back({
        .name = String(item[F("name")]),
        .value = String(item[F("value")]),
        .channel = String(item[F("channel")]).toInt(),
        .press = RadioPressState(String(item[F("press")]).toInt()),
      });
    }
    radioStorage.save(model);
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioEmitSend() {
    _dispatchRequestEnd();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(64);
    deserializeJson(payload, payloadJson);
    // read
    const auto index = String(payload[F("index")]);
    DynamicJsonDocument res(64);
    if (onRadioEmit) {
      onRadioEmit(index.toInt());
      res[F("message")] = String(F("success"));
    } else {
      res[F("error")] = String(F("onRadioEmit is required"));
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioRuleGet() {
    _dispatchRequestStart();
    DynamicJsonDocument res(1024 + 512);
    // rules
    const auto model = radioStorage.load();
    const JsonArray ruleArr = res.createNestedArray(F("rules"));
    for (const auto& rule : model.rules) {
      const JsonObject ruleObj = ruleArr.createNestedObject();
      ruleObj[F("value")] = rule.value;
      ruleObj[F("channel")] = rule.channel;
      ruleObj[F("press")] = rule.press;
      ruleObj[F("action")] = rule.action;
      ruleObj[F("serviceId")] = rule.serviceId;
    }
    // last received
    const auto lastReceived = radioStorage.getLastReceived();
    const JsonObject lastReceivedObj = res.createNestedObject(F("lastReceived"));
    lastReceivedObj[F("value")] = lastReceived.value;
    lastReceivedObj[F("channel")] = lastReceived.channel;
    // end
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioRuleSave() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(1024 + 512);
    deserializeJson(payload, payloadJson);
    // read
    const auto ruleItems = payload["rules"];
    // save
    auto model = radioStorage.load();
    model.rules.clear();
    for (size_t i = 0; i < ruleItems.size(); i++) {
      const auto item = ruleItems[i];
      model.rules.push_back({
        .value = String(item[F("value")]),
        .channel = String(item[F("channel")]).toInt(),
        .press = RadioPressState(String(item[F("press")]).toInt()),
        .action = RadioAction(String(item[F("action")]).toInt()),
        .serviceId = String(item[F("serviceId")]),
      });
    }
    radioStorage.save(model);
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioCommandGet() {
    _dispatchRequestStart();
    DynamicJsonDocument res(1024 + 512);
    // commands
    const auto model = radioStorage.load();
    const JsonArray commandArr = res.createNestedArray(F("commands"));
    for (const auto& command : model.commands) {
      const JsonObject commandObj = commandArr.createNestedObject();
      commandObj[F("entry")] = command.entry;
      commandObj[F("action")] = command.action;
      commandObj[F("press")] = command.press;
      commandObj[F("serviceId")] = command.serviceId;
    }
    // end
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleRadioCommandSave() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(1024 + 512);
    deserializeJson(payload, payloadJson);
    // read
    const auto commandItems = payload["commands"];
    // save
    auto model = radioStorage.load();
    model.commands.clear();
    for (size_t i = 0; i < commandItems.size(); i++) {
      const auto item = commandItems[i];
      model.commands.push_back({
        .entry = RadioCommandEntry(String(item[F("entry")]).toInt()),
        .action = String(item[F("action")]).toInt(),
        .press = RadioPressState(String(item[F("press")]).toInt()),
        .serviceId = String(item[F("serviceId")]),
      });
    }
    radioStorage.save(model);
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = String(F("success"));
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleServiceState() {
    _dispatchRequestStart();
    std::vector<KeyValueModel> states = {
      { .key = "Identify", .value = victorWifi.getHostName() }
    };
    if (onGetServiceState) {
      onGetServiceState(states);
    }
    DynamicJsonDocument res(1024);
    const JsonArray itemsArr = res.createNestedArray(F("items"));
    for (const auto& state : states) {
      const JsonArray stateArr = itemsArr.createNestedArray();
      stateArr[0] = state.key;
      stateArr[1] = state.value;
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void VictorWeb::_handleServiceReset() {
    _dispatchRequestStart();
    DynamicJsonDocument res(512);
    if (onResetService) {
      onResetService();
      res[F("message")] = String(F("success"));
    } else {
      res[F("error")] = String(F("onResetService is required"));
    }
    _sendJson(res);
    _dispatchRequestEnd();
  }

} // namespace Victor::Components
