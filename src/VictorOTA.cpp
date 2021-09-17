#include "VictorOTA.h"

namespace Victor::Components {

  void VictorOTA::setup() {
    // ESPhttpUpdate.setAuthorization(user, password);
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
    // hook events
    ESPhttpUpdate.onStart(VictorOTA::_onStart);
    ESPhttpUpdate.onEnd(VictorOTA::_onEnd);
    ESPhttpUpdate.onProgress(VictorOTA::_onProgress);
    ESPhttpUpdate.onError(VictorOTA::_onError);
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

  void VictorOTA::_onStart() {
    console.log(F("[VictorOTA] start updating"));
  }

  void VictorOTA::_onEnd() {
    console.log(F("[VictorOTA] update finished"));
  }

  void VictorOTA::_onProgress(int progress, int total) {
    console.log().write(F("[VictorOTA] progress ")).write(String(progress / (total / 100))).write(F("%")).newline();
  }

  void VictorOTA::_onError(int error) {
    auto message = ESPhttpUpdate.getLastErrorString();
    console.error().write(F("[VictorOTA] error ")).write(String(error)).write(F(", message ")).write(message).newline();
  }

} // namespace Victor::Components
