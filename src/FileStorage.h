#ifndef FileStorage_h
#define FileStorage_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "Commons.h"

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
    String _filePath;
    size_t _maxSize = DEFAULT_FILE_SIZE;
    bool _userCache = false;
    TModel* _cache = NULL;
    Console _error();
    virtual void _serializeTo(const TModel& model, DynamicJsonDocument& doc);
    virtual void _deserializeFrom(TModel& model, const DynamicJsonDocument& doc);
  };

  template <class TModel>
  FileStorage<TModel>::FileStorage() {}

  template <class TModel>
  TModel FileStorage<TModel>::load() {
    if (_cache != NULL) {
      return *_cache;
    }
    // default result
    TModel model;
    // check exists
    if (LittleFS.exists(_filePath)) {
      // open file
      auto file = LittleFS.open(_filePath, "r");
      if (file) {
        // validate size
        auto size = file.size();
        // read file
        // Allocate a buffer to store contents of the file.
        char buffer[size];
        // We don't use String here because ArduinoJson library requires the input
        // buffer to be mutable. If you don't use ArduinoJson, you may as well
        // use file.readString instead.
        file.readBytes(buffer, size);
        // close
        file.close();
        // deserialize
        if (size <= _maxSize) {
          DynamicJsonDocument doc(_maxSize); // Store data in the heap - Dynamic Memory Allocation
          // StaticJsonDocument<DEFAULT_FILE_SIZE> doc; // Store data in the stack - Fixed Memory Allocation
          auto error = deserializeJson(doc, buffer);
          if (!error) {
            // convert
            _deserializeFrom(model, doc);
          } else {
            _error().write(F("parse failed ")).write(error.f_str()).newline();
          }
        } else {
          _error().write(F("file too large")).newline();
        }
      } else {
        _error().write(F("open failed")).newline();
      }
    } else {
      _error().write(F("file notfound ")).write(_filePath).newline();
    }
    if (_userCache) {
      _cache = &model;
    }
    return model;
  }

  template <class TModel>
  bool FileStorage<TModel>::save(const TModel& model) {
    _cache = NULL;
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
      _error().write(F("open write failed")).newline();
    }
    return success;
  }

  template <class TModel>
  Console FileStorage<TModel>::_error() {
    return console.error().type(F("FileStorage"));
  }

  template <class TModel>
  void FileStorage<TModel>::_serializeTo(const TModel& model, DynamicJsonDocument& doc) {}

  template <class TModel>
  void FileStorage<TModel>::_deserializeFrom(TModel& model, const DynamicJsonDocument& doc) {}

} // namespace Victor::Components

#endif // FileStorage_h
