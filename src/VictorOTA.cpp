#include "VictorOTA.h"

namespace Victor::Components {

  void VictorOTA::setup() {
    // ESPhttpUpdate.setAuthorization(user, password);
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
    // hook events
    ESPhttpUpdate.onStart(std::bind(&VictorOTA::_handleStart, this));
    ESPhttpUpdate.onEnd(std::bind(&VictorOTA::_handleEnd, this));
    ESPhttpUpdate.onProgress(std::bind(&VictorOTA::_handleProgress, this, std::placeholders::_1, std::placeholders::_2));
    ESPhttpUpdate.onError(std::bind(&VictorOTA::_handleError, this, std::placeholders::_1));
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

  void VictorOTA::_handleStart() {
    _log().section(F("start updating"));
  }

  void VictorOTA::_handleEnd() {
    _log().section(F("update finished"));
  }

  void VictorOTA::_handleProgress(int progress, int total) {
    _log().section(F("progress"), String(progress / (total / 100)) + F("%"));
  }

  void VictorOTA::_handleError(int error) {
    auto message = ESPhttpUpdate.getLastErrorString();
    _log().bracket(String(error)).section(F("last error"), message);
  }

  Console VictorOTA::_log() {
    return console.log().bracket("ota");
  }

  // global
  VictorOTA victorOTA;

} // namespace Victor::Components
