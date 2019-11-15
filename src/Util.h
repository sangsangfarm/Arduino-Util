/**
 * @file Util.h
 * @brief Arduino Utility
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <time.h>

// Debug
#ifndef UTIL_DEBUG_PORT
#define UTIL_DEBUG_PORT Serial
#endif

#ifdef UTIL_DEBUG
#define print(...) UTIL_DEBUG_PORT.print(__VA_ARGS__)
#define printf(...) UTIL_DEBUG_PORT.printf(__VA_ARGS__)
#define println(...) UTIL_DEBUG_PORT.println(__VA_ARGS__)

#else
#define print(...)
#define printf(...)
#define println(...)
#endif
#define TIMEOUT 1000

/**
 * @brief Arduino Utility
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
class Util {
 private:
  time_t _disconnect_time = -1;
  time_t _current_time = -1;
  time_t _last_connection_check_time = -1;
  int _num_get_connection_fail = 0;

 public:
  void beginSerial(int speed = 115200);
  void syncTime(int timezone = 0, int try_num = 10);
  char *StringToChar(String str);
  void checkConnection(char *base_url, char *device_id, time_t interval = 60);
};

#endif /*UTIL_H_*/