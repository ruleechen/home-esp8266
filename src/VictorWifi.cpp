#include "VictorWifi.h"

namespace Victor::Components {

  void VictorWifi::setup() {
    auto wifiMode = WiFi.getMode();
    auto apEnabled = ((wifiMode & WIFI_AP) != 0);
    auto staEnabled = ((wifiMode & WIFI_STA) != 0);
    if (!apEnabled && !staEnabled) {
      WiFi.mode(WIFI_AP_STA);
      wifiMode = WIFI_AP_STA;
    }

    auto hostName = getHostName();
    auto isApEnabled = ((wifiMode & WIFI_AP) != 0);
    if (isApEnabled) {
      // IPAddress apIp(192, 168, 1, 33);
      // IPAddress apSubnet(255, 255, 255, 0);
      // WiFi.softAPConfig(apIp, apIp, apSubnet);
      WiFi.softAP(hostName); // name which is displayed on AP list
      auto currentApIp = WiFi.softAPIP();
      if (currentApIp) {
        _log().section(F("ap address")).section(currentApIp.toString());
      }
    }

    WiFi.onStationModeGotIP(std::bind(&VictorWifi::_handleStationModeGotIP, this, std::placeholders::_1));
    WiFi.onStationModeDisconnected(std::bind(&VictorWifi::_handleStationModeDisconnected, this, std::placeholders::_1));

    WiFi.hostname(hostName); // name which is displayed on router
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    auto ssidJoined = WiFi.SSID();
    if (!ssidJoined || ssidJoined == "") {
      auto model = appStorage.load();
      WiFi.begin(model.wifiSsid, model.wifiPass);
    } else {
      WiFi.begin();
    }
  }

  void VictorWifi::reset() {
    // wifi_config_reset();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    _log().section(F("mode")).section(F("WIFI_AP_STA"));
  }

  void VictorWifi::join(String ssid, String password, int32_t channel, uint8_t* bssid) {
    _log().section(F("joining")).section(ssid);
    WiFi.persistent(true);
    WiFi.begin(ssid, password, channel, bssid, true);
    // wait for connecting
    console.newline();
    auto checkTimes = 60;
    while (WiFi.status() != WL_CONNECTED) {
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

  String VictorWifi::getLocalHostName() {
    return getHostName(false);
  }

  String VictorWifi::getHostName(bool includeVersion) {
    auto id = getHostId();

    auto version = String(FirmwareVersion);
    version.replace(F("."), F(""));

    auto model = appStorage.load();
    auto productName = model.name.length() > 0
      ? model.name
      : FirmwareName;

    auto hostName = includeVersion
      ? productName + F("-") + id + F("-") + version
      : productName + F("-") + id;

    return hostName;
  }

  Console VictorWifi::_log() {
    return console.log().bracket(F("wifi"));
  }

  void VictorWifi::_handleStationModeGotIP(const WiFiEventStationModeGotIP& args) {
    _log().section(F("station mode")).section(F("got ip"));
    auto model = appStorage.load();
    if (model.autoMode) {
      auto wifiMode = WiFi.getMode();
      if (wifiMode != WIFI_STA) {
        WiFi.mode(WIFI_STA);
      }
    }
  }

  void VictorWifi::_handleStationModeDisconnected(const WiFiEventStationModeDisconnected& args) {
    _log().section(F("station mode")).section(F("disconnected"));
    auto model = appStorage.load();
    if (model.autoMode) {
      auto wifiMode = WiFi.getMode();
      if (wifiMode != WIFI_AP_STA) {
        WiFi.mode(WIFI_AP_STA);
      }
    }
  }

  // global
  VictorWifi victorWifi;

} // namespace Victor::Components
