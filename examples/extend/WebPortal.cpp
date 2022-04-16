#include "WebPortal.h"

namespace Victor::Components {

  WebPortal::WebPortal(int port)
  : VictorWeb(port) {}

  WebPortal::~WebPortal() {}

  void WebPortal::_registerHandlers() {
    VictorWeb::_registerHandlers();
    _server->serveStatic("/fav.ico", LittleFS, "/web/fav.ico", "max-age=43200");
    _server->serveStatic("/test.min.js", LittleFS, "/web/test.min.js");
    _server->on(F("/service/list"), HTTP_GET, std::bind(&WebPortal::_handleServiceList, this));
    _server->on(F("/service/save"), HTTP_POST, std::bind(&WebPortal::_handleServiceSave, this));
  }

  void WebPortal::_solvePageTokens(String& html) {
    VictorWeb::_solvePageTokens(html);
    html.replace(F("{appendHead}"), F("\
      <link rel=\"icon\" href=\"fav.ico\">\
      <script src=\"victoria.min.js\"></script>\
    "));
  }

  void WebPortal::_handleServiceList() {
    _dispatchRequestStart();
    DynamicJsonDocument res(512);
    const JsonArray serviceArr = res.createNestedArray(F("services"));
    const JsonObject serviceObj = serviceArr.createNestedObject();
    serviceObj[F("id")] = F("abcd");
    serviceObj[F("name")] = F("Test");
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void WebPortal::_handleServiceSave() {
    _dispatchRequestStart();
    // payload
    const auto payloadJson = _server->arg(F("plain"));
    DynamicJsonDocument payload(512);
    deserializeJson(payload, payloadJson);
    // read
    const auto ruleItems = payload[F("rules")];
    // ...
    // res
    DynamicJsonDocument res(64);
    res[F("message")] = F("success");
    _sendJson(res);
    _dispatchRequestEnd();
  }

} // namespace Victor::Components
