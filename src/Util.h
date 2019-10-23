/**
 * @file Util.h
 * @brief 자주 쓰는 기본 아두이노 기능
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <Arduino.h>
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
/**
 * @brief 자주 쓰는 기본 아두이노 기능
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
class Util {
 private:
 public:
  void beginSerial(int speed = 115200);
  void syncTime(int timezone = 0, int try_num = 10);
  char *StringToChar(String str);
};

#endif /*UTIL_H_*/