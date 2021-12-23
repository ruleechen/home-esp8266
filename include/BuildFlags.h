#ifndef BuildFlags_h
#define BuildFlags_h

#ifndef UNIX_TIME
#define UNIX_TIME 0
#endif

#ifndef VICTOR_FIRMWARE_NAME
#define VICTOR_FIRMWARE_NAME "Victor"
#endif

#ifndef VICTOR_FIRMWARE_VERSION
#define VICTOR_FIRMWARE_VERSION "1.0.0"
#endif

#include <Arduino.h>

namespace Victor {
  const unsigned long UnixTime = UNIX_TIME;
  const String FirmwareName = VICTOR_FIRMWARE_NAME;
  const String FirmwareVersion = VICTOR_FIRMWARE_VERSION;
} // namespace Victor

#endif // BuildFlags_h
