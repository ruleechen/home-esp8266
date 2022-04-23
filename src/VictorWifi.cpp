#include "VictorWifi.h"

namespace Victor::Components {

  void VictorWifi::setup() {
    setMode(WIFI_AP_STA);
    WiFi.persistent(true);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    // IPAddress apIp(192, 168, 1, 33);
    // IPAddress apSubnet(255, 255, 255, 0);
    // WiFi.softAPConfig(apIp, apIp, apSubnet);

    const auto hostName = getHostName();
    WiFi.hostname(hostName); // name which is displayed on router
    WiFi.softAP(hostName); // name which is displayed on AP list

    // need to keep the event handler for prevent unsubscribed automatically
    // https://github.com/esp8266/Arduino/issues/2545
    _gotIPHandler = WiFi.onStationModeGotIP(std::bind(&VictorWifi::_handleStaGotIP, this, std::placeholders::_1));
    _disconnectedHandler = WiFi.onStationModeDisconnected(std::bind(&VictorWifi::_handleStaDisconnected, this, std::placeholders::_1));

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
    setMode(WIFI_AP_STA);
    builtinLed.twinkle();
  }

  WiFiMode_t VictorWifi::getMode() const {
    return WiFi.getMode();
  }

  void VictorWifi::setMode(WiFiMode_t mode) {
    if (getMode() != mode) {
      WiFi.mode(mode);
      _log().section(F("mode"), modeName(mode));
    }
  }

  String VictorWifi::modeName(WiFiMode_t mode) {
    if (mode == WIFI_STA) {
      return F("STA");
    } else if (mode == WIFI_AP) {
      return F("AP");
    } else if (mode == WIFI_AP_STA) {
      return F("AP+STA");
    } else if (mode == WIFI_OFF) {
      return F("OFF");
    } else {
      return F("Unknown");
    }
  }

  void VictorWifi::join(const String& ssid, const String& password, int32_t channel, uint8_t* bssid) {
    _log().section(F("joining"), ssid);
    _joiningSsid = ssid;
    _joiningPass = password;
    WiFi.begin(ssid, password, channel, bssid, true);
  }

  bool VictorWifi::isConnected() const {
    return WiFi.isConnected();
  }

  bool VictorWifi::isMDNSRunning() const {
    return MDNS.isRunning();
  }

  int8_t VictorWifi::status() {
    const auto result = WiFi.status();
    _log().section(F("got status"), String(result));
    return result;
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
      : VICTOR_FIRMWARE_NAME;
    return productName + F("-") + id;
  }

  void VictorWifi::_handleStaGotIP(const WiFiEventStationModeGotIP& args) {
    builtinLed.stop();
    _log().section(F("station")).section(F("got ip"), args.ip.toString());
    auto model = appStorage.load();
    if (_joiningSsid && _joiningSsid != F("")) {
      // save new wifi credential
      if (
        model.wifiSsid != _joiningSsid ||
        model.wifiPass != _joiningPass
      ) {
        model.wifiSsid = _joiningSsid;
        model.wifiPass = _joiningPass;
        appStorage.save(model);
      }
    } else {
      // turn off AP only when it is not a new join
      if (model.autoMode) {
        setMode(WIFI_STA);
      }
    }
  }

  void VictorWifi::_handleStaDisconnected(const WiFiEventStationModeDisconnected& args) {
    builtinLed.twinkle();
    _log().section(F("station"), F("disconnected"));
    const auto model = appStorage.load();
    if (model.autoMode) {
      setMode(WIFI_AP_STA);
    }
  }

  Console VictorWifi::_log() {
    return console.log().bracket(F("wifi"));
  }

  // global
  VictorWifi victorWifi;

} // namespace Victor::Components
