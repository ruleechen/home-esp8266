#include "VictorWifi.h"

namespace Victor::Components {

  void VictorWifi::setup() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    // IPAddress apIp(192, 168, 1, 33);
    // IPAddress apSubnet(255, 255, 255, 0);
    // WiFi.softAPConfig(apIp, apIp, apSubnet);

    const auto apName = getApName();
    WiFi.hostname(apName); // name which is displayed on router
    WiFi.softAP(apName); // name which is displayed on AP list

    // need to keep the event handler for prevent unsubscribed automatically
    // https://github.com/esp8266/Arduino/issues/2545
    _gotIPHandler = WiFi.onStationModeGotIP(std::bind(&VictorWifi::_handleStationModeGotIP, this, std::placeholders::_1));
    _disconnectedHandler = WiFi.onStationModeDisconnected(std::bind(&VictorWifi::_handleStationModeDisconnected, this, std::placeholders::_1));

    const auto model = appStorage.load();
    WiFi.begin(model.wifiSsid, model.wifiPass);

    builtinLed.twinkle();
    _log().section(F("begin"))
      .section(F("ssid"), WiFi.SSID())
      .section(F("host"), WiFi.getHostname())
      .section(F("ap"), WiFi.softAPSSID())
      .section(F("ap-ip"), WiFi.softAPIP().toString());
  }

  void VictorWifi::reset() {
    // wifi_config_reset();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    builtinLed.twinkle();
    _log().section(F("mode"), F("AP_STA"));
  }

  void VictorWifi::join(String ssid, String password, int32_t channel, uint8_t* bssid) {
    _log().section(F("joining"), ssid);
    WiFi.begin(ssid, password, channel, bssid, true);
  }

  bool VictorWifi::isConnected() {
    return WiFi.isConnected();
  }

  void VictorWifi::waitForConnected() {
    console.newline();
    auto checkTimes = 60;
    while (!isConnected()) {
      delay(500);
      console.write(F("."));
      if (checkTimes == 0) {
        break;
      } else {
        checkTimes--;
      }
    }
  }

  String VictorWifi::getHostId() {
    auto id = WiFi.macAddress();
    id.replace(F(":"), F(""));
    id.toUpperCase();
    id = id.substring(id.length() - 6);
    return id;
  }

  String VictorWifi::getHostName() {
    const auto id = getHostId();
    const auto model = appStorage.load();
    const auto productName = model.name.length() > 0
      ? model.name
      : FirmwareName;
    return productName + F("-") + id;
  }

  String VictorWifi::getApName() {
    const auto host = getHostName();
    auto version = String(FirmwareVersion);
    version.replace(F("."), F(""));
    return host + F("-") + version;
  }

  void VictorWifi::_handleStationModeGotIP(const WiFiEventStationModeGotIP& args) {
    builtinLed.stop();
    _log().section(F("station")).section(F("got ip"), args.ip.toString());
    const auto model = appStorage.load();
    if (model.autoMode) {
      const auto mode = WiFi.getMode();
      if (mode != WIFI_STA) {
        WiFi.mode(WIFI_STA);
        _log().section(F("mode"), F("STA"));
      }
    }
  }

  void VictorWifi::_handleStationModeDisconnected(const WiFiEventStationModeDisconnected& args) {
    builtinLed.twinkle();
    _log().section(F("station"), F("disconnected"));
    const auto model = appStorage.load();
    if (model.autoMode) {
      const auto mode = WiFi.getMode();
      if (mode != WIFI_AP_STA) {
        WiFi.mode(WIFI_AP_STA);
        _log().section(F("mode"), F("AP_STA"));
      }
    }
  }

  Console VictorWifi::_log() {
    return console.log().bracket(F("wifi"));
  }

  // global
  VictorWifi victorWifi;

} // namespace Victor::Components
