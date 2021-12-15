#include "VictorOTA.h"

namespace Victor::Components {

  void VictorOTA::setup() {
    // ESPhttpUpdate.setAuthorization(user, password);
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
    // hook events
    ESPhttpUpdate.onStart(std::bind(&VictorOTA::_onStart, this));
    ESPhttpUpdate.onEnd(std::bind(&VictorOTA::_onEnd, this));
    ESPhttpUpdate.onProgress(std::bind(&VictorOTA::_onProgress, this, std::placeholders::_1, std::placeholders::_2));
    ESPhttpUpdate.onError(std::bind(&VictorOTA::_onError, this, std::placeholders::_1));
  }

  String VictorOTA::getCurrentVersion() {
    return FirmwareVersion;
  }

  String VictorOTA::checkNewVersion() {
    return F("99.4.99");
  }

  void VictorOTA::update(String version, VOtaType type) {
    ESPhttpUpdate.rebootOnUpdate(true);
    switch (type) {
      case VOta_All: {
        ESPhttpUpdate.rebootOnUpdate(false);
        _updateSketch();
        ESPhttpUpdate.rebootOnUpdate(true);
        _updateFileSystem();
        break;
      }
      case VOta_Sketch: {
        _updateSketch();
        break;
      }
      case VOta_FileSystem: {
        _updateFileSystem();
        break;
      }
      default: {
        break;
      }
    }
  }

  void VictorOTA::trigger(VOtaType type) {
    // versions
    auto currentVersion = getCurrentVersion();
    auto newVersion = checkNewVersion();
    // 21.3.10 --> 21310
    currentVersion.replace(F("."), F(""));
    newVersion.replace(F("."), F(""));
    // compare
    if (newVersion.toInt() > currentVersion.toInt()) {
      update(newVersion, type);
    }
  }

  Console VictorOTA::_log() {
    return console.log().type("VictorOTA");
  }

  void VictorOTA::_updateSketch() {
    WiFiClient client;
    auto currentVersion = getCurrentVersion();
    ESPhttpUpdate.update(client, F("http://wwww.rulee.cn/esp8266/firmware.bin"), currentVersion);
  }

  void VictorOTA::_updateFileSystem() {
    WiFiClient client;
    auto currentVersion = getCurrentVersion();
    ESPhttpUpdate.updateFS(client, F("http://wwww.rulee.cn/esp8266/littlefs.bin"), currentVersion);
  }

  void VictorOTA::_onStart() {
    _log().write(F("start updating")).newline();
  }

  void VictorOTA::_onEnd() {
    _log().write(F("update finished")).newline();
  }

  void VictorOTA::_onProgress(int progress, int total) {
    _log().write(F("progress ")).write(String(progress / (total / 100))).write(F("%")).newline();
  }

  void VictorOTA::_onError(int error) {
    auto message = ESPhttpUpdate.getLastErrorString();
    _log().write(F("error ")).write(String(error)).write(F(", message ")).write(message).newline();
  }

  // global
  VictorOTA victorOTA;

} // namespace Victor::Components
