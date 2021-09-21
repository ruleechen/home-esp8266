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
        _log().write(F("AP Address > ")).write(currentApIp.toString()).newline();
      }
    }

    WiFi.hostname(hostName); // name which is displayed on router
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    WiFi.begin();
    WiFi.onEvent(VictorWifi::_onWifiEvent, WiFiEvent::WIFI_EVENT_ANY);
  }

  void VictorWifi::reset() {
    // wifi_config_reset();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    _log().write(F("mode > WIFI_AP_STA")).newline();
  }

  void VictorWifi::join(String ssid, String password, int32_t channel, uint8_t* bssid) {
    _log().write(F("ssid > ")).write(ssid).newline();
    _log().write(F("password > ")).write(password).newline();
    WiFi.persistent(true);
    WiFi.begin(ssid, password, channel, bssid, true);
    // wait for connecting
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
    console.newline();
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
    return console.log().type(F("WiFi"));
  }

  void VictorWifi::_onWifiEvent(WiFiEvent_t event) {
    switch (event) {
      case WiFiEvent::WIFI_EVENT_STAMODE_CONNECTED: {
        _log().write(F("STA connected")).newline();
        break;
      }
      case WiFiEvent::WIFI_EVENT_STAMODE_DISCONNECTED: {
        _log().write(F("STA disconnected")).newline();
        break;
      }
      case WiFiEvent::WIFI_EVENT_STAMODE_GOT_IP: {
        _log().write(F("STA got ip")).newline();
        break;
      }
      case WiFiEvent::WIFI_EVENT_SOFTAPMODE_STACONNECTED: {
        _log().write(F("AP connected")).newline();
        break;
      }
      case WiFiEvent::WIFI_EVENT_SOFTAPMODE_STADISCONNECTED: {
        _log().write(F("AP disconnected")).newline();
        break;
      }
      default: {
        break;
      }
    }
  }

} // namespace Victor::Components
