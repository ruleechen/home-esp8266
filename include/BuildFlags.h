#ifndef BuildFlags_h
#define BuildFlags_h

#ifndef UNIX_TIME
#define UNIX_TIME 0
#endif

#ifndef FIRMWARE_NAME
#define FIRMWARE_NAME "Victor"
#endif

#ifndef FIRMWARE_VERSION
#define FIRMWARE_VERSION "1.0.0"
#endif

#include <Arduino.h>

namespace Victor {
  const unsigned long UnixTime = UNIX_TIME;
  const String FirmwareName = FIRMWARE_NAME;
  const String FirmwareVersion = FIRMWARE_VERSION;
} // namespace Victor

#endif // BuildFlags_h
