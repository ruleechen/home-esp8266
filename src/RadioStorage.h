#ifndef RadioStorage_h
#define RadioStorage_h

#include "FileStorage.h"

namespace Victor::Components {
  class RadioStorage : public FileStorage<RadioModel> {
   public:
    RadioStorage();
    void broadcast(RadioMessage message); // message object should be copied
    RadioMessage getLastReceived();

   protected:
    RadioMessage _lastReceived;
    void _serializeTo(const RadioModel& model, DynamicJsonDocument& doc) override;
    void _deserializeFrom(RadioModel& model, const DynamicJsonDocument& doc) override;
  };

  // global
  extern RadioStorage radioStorage;

} // namespace Victor::Components

#endif // RadioStorage_h
