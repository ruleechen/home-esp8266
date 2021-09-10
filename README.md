# Home ESP8266
The foundation of home accessories base on chip ESP8266

### Development Tools
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE for VSCode](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
- [Web SPA solution with Mithril](https://mithril.js.org/)

### PlatformIO CLI
```bash
pio run --environment esp01_1m
pio run --environment esp01_1m --target buildfs
pio package pack
pio package publish home-esp8266/home-esp8266-0.0.7.tar.gz
```
