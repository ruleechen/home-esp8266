#ifndef FileStorage_h
#define FileStorage_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "Console.h"

// https://arduinojson.org/v6/assistant/
// https://cpp4arduino.com/2018/11/06/what-is-heap-fragmentation.html
#define DEFAULT_FILE_SIZE 2048 // block size 4096

namespace Victor::Components {

  template <class TModel>
  class FileStorage {
   public:
    FileStorage();
    TModel load();
    bool save(const TModel& model);

   protected:
    const char* _filePath;
    size_t _maxSize = DEFAULT_FILE_SIZE;
    bool _enableCache = false;
    TModel* _cache = nullptr;
    Console _error();
    virtual void _serializeTo(const TModel& model, DynamicJsonDocument& doc);
    virtual void _deserializeFrom(TModel& model, const DynamicJsonDocument& doc);
  };

  template <class TModel>
  FileStorage<TModel>::FileStorage() {}

  template <class TModel>
  TModel FileStorage<TModel>::load() {
    if (_cache != nullptr) {
      return *_cache;
    }
    // default result
    TModel model;
    // check exists
    if (LittleFS.exists(_filePath)) {
      // open file
      auto file = LittleFS.open(_filePath, "r");
      if (file) {
        // deserialize
        DynamicJsonDocument doc(_maxSize); // Store data in the heap - Dynamic Memory Allocation
        // StaticJsonDocument<DEFAULT_FILE_SIZE> doc; // Store data in the stack - Fixed Memory Allocation
        const auto error = deserializeJson(doc, file);
        if (!error) {
          // convert
          _deserializeFrom(model, doc);
        } else {
          _error().section(F("parse failed"), error.f_str());
        }
        // close
        file.close();
      } else {
        _error().section(F("open failed"));
      }
    } else {
      _error().section(F("not found"), _filePath);
    }
    if (_enableCache) {
      _cache = &model;
    }
    return model;
  }

  template <class TModel>
  bool FileStorage<TModel>::save(const TModel& model) {
    _cache = nullptr;
    // convert
    DynamicJsonDocument doc(_maxSize); // Store data in the heap - Dynamic Memory Allocation
    // StaticJsonDocument<DEFAULT_FILE_SIZE> doc; // Store data in the stack - Fixed Memory Allocation
    _serializeTo(model, doc);
    auto success = false;
    // open file
    auto file = LittleFS.open(_filePath, "w");
    if (file) {
      // write
      serializeJson(doc, file);
      // close
      file.close();
      success = true;
    } else {
      _error().section(F("open(w) failed"));
    }
    return success;
  }

  template <class TModel>
  Console FileStorage<TModel>::_error() {
    return console.error().bracket(F("storage"));
  }

  template <class TModel>
  void FileStorage<TModel>::_serializeTo(const TModel& model, DynamicJsonDocument& doc) {}

  template <class TModel>
  void FileStorage<TModel>::_deserializeFrom(TModel& model, const DynamicJsonDocument& doc) {}

} // namespace Victor::Components

#endif // FileStorage_h
