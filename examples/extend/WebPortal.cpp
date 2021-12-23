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
    JsonArray serviceArr = res.createNestedArray(F("services"));
    JsonObject serviceObj = serviceArr.createNestedObject();
    serviceObj[F("id")] = "abcd";
    serviceObj[F("name")] = "Test";
    _sendJson(res);
    _dispatchRequestEnd();
  }

  void WebPortal::_handleServiceSave() {
    _dispatchRequestStart();
      // payload
      auto payloadJson = _server->arg(F("plain"));
      DynamicJsonDocument payload(512);
      deserializeJson(payload, payloadJson);
      // read
      auto ruleItems = payload["rules"];
      // ...
      // res
      DynamicJsonDocument res(64);
      res[F("message")] = String(F("success"));
      _sendJson(res);
      _dispatchRequestEnd();
  }

} // namespace Victor::Components