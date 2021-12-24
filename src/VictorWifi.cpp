#include "VictorWifi.h"

namespace Victor::Components {

  void VictorWifi::setup() {
    auto wifiMode = WiFi.getMode();
    const auto apEnabled = ((wifiMode & WIFI_AP) != 0);
    const auto staEnabled = ((wifiMode & WIFI_STA) != 0);
    if (!apEnabled && !staEnabled) {
      WiFi.mode(WIFI_AP_STA);
      wifiMode = WIFI_AP_STA;
    }

    const auto apName = getApName();
    const auto isApEnabled = ((wifiMode & WIFI_AP) != 0);
    if (isApEnabled) {
      // IPAddress apIp(192, 168, 1, 33);
      // IPAddress apSubnet(255, 255, 255, 0);
      // WiFi.softAPConfig(apIp, apIp, apSubnet);
      WiFi.softAP(apName); // name which is displayed on AP list
      _log().section(F("ap"))
        .section(F("name"), WiFi.softAPSSID())
        .section(F("address"), WiFi.softAPIP().toString());
    }

    // WiFi.onStationModeGotIP(std::bind(&VictorWifi::_handleStationModeGotIP, this, std::placeholders::_1));
    // WiFi.onStationModeDisconnected(std::bind(&VictorWifi::_handleStationModeDisconnected, this, std::placeholders::_1));
    WiFi.onEvent(VictorWifi::_onWifiEvent, WiFiEvent::WIFI_EVENT_ANY);

    WiFi.hostname(apName); // name which is displayed on router
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    const auto ssidJoined = WiFi.SSID();
    if (!ssidJoined || ssidJoined == "") {
      const auto model = appStorage.load();
      WiFi.begin(model.wifiSsid, model.wifiPass);
    } else {
      WiFi.begin();
    }
    _log().section(F("begin"))
      .section(F("ssid"), WiFi.SSID())
      .section(F("host"), WiFi.getHostname());
  }

  void VictorWifi::reset() {
    // wifi_config_reset();
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    _log().section(F("reset on mode"), F("WIFI_AP_STA"));
  }

  void VictorWifi::join(String ssid, String password, int32_t channel, uint8_t* bssid) {
    _log().section(F("joining"), ssid);
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
    auto version = FirmwareVersion;
    version.replace(F("."), F(""));
    return host + F("-") + version;
  }

  void VictorWifi::_handleStationModeGotIP(const WiFiEventStationModeGotIP& args) {
    _log().section(F("station")).section(F("got ip"), WiFi.localIP().toString());
    const auto model = appStorage.load();
    if (model.autoMode) {
      const auto wifiMode = WiFi.getMode();
      if (wifiMode != WIFI_STA) {
        WiFi.mode(WIFI_STA);
      }
    }
  }

  void VictorWifi::_handleStationModeDisconnected(const WiFiEventStationModeDisconnected& args) {
    _log().section(F("station"), F("disconnected"));
    const auto model = appStorage.load();
    if (model.autoMode) {
      const auto wifiMode = WiFi.getMode();
      if (wifiMode != WIFI_AP_STA) {
        WiFi.mode(WIFI_AP_STA);
      }
    }
  }

  void VictorWifi::_onWifiEvent(WiFiEvent_t event) {
    switch (event) {
      case WiFiEvent::WIFI_EVENT_STAMODE_CONNECTED: {
        _log().section(F("station"), F("connected"));
        break;
      }
      case WiFiEvent::WIFI_EVENT_STAMODE_DISCONNECTED: {
        _log().section(F("station"), F("disconnected"));
        const auto model = appStorage.load();
        if (model.autoMode) {
          const auto wifiMode = WiFi.getMode();
          if (wifiMode != WIFI_AP_STA) {
            WiFi.mode(WIFI_AP_STA);
          }
        }
        break;
      }
      case WiFiEvent::WIFI_EVENT_STAMODE_GOT_IP: {
        _log().section(F("station")).section(F("got ip"), WiFi.localIP().toString());
        const auto model = appStorage.load();
        if (model.autoMode) {
          const auto wifiMode = WiFi.getMode();
          if (wifiMode != WIFI_STA) {
            WiFi.mode(WIFI_STA);
          }
        }
        break;
      }
      case WiFiEvent::WIFI_EVENT_SOFTAPMODE_STACONNECTED: {
        _log().section(F("ap"), F("connected"));
        break;
      }
      case WiFiEvent::WIFI_EVENT_SOFTAPMODE_STADISCONNECTED: {
        _log().section(F("ap"), F("disconnected"));
        break;
      }
      default: {
        break;
      }
    }
  }

  Console VictorWifi::_log() {
    return console.log().bracket(F("wifi"));
  }

  // global
  VictorWifi victorWifi;

} // namespace Victor::Components
