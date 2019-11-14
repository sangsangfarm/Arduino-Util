/**
 * @file Util.cpp
 * @brief Arduino Utility
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#include <Util.h>

/**
 * @fn void Util::beginSerial(int speed)
 * @brief Begin serial communication
 * @param speed Monitoring speed
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void Util::beginSerial(int speed) {
  UTIL_DEBUG_PORT.begin(speed);
  UTIL_DEBUG_PORT.setDebugOutput(true);
  delay(2000);
  println(F("[Util] 디버그 실행."));
}

/**
 * @fn void Util::syncTime(int timezone, int try_num)
 * @brief Synchronize time and will reboot when the number of attempts exceeds
 * @param timezone Time zone
 * @param try_num Time synchronization attempts
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void Util::syncTime(int timezone, int try_num) {
  time_t epoch_time;
  configTime(timezone, 0, "pool.ntp.org", "time.nist.gov");
  print(F("[Util] Synchronize time.."));
  int count = 0;
  while (true) {
    count++;
    if (count > try_num) {
      println("[Util] Failed to Synchronize. Device will reboot.");
      ESP.restart();
    }
    epoch_time = time(nullptr);

    if (epoch_time == 0 || epoch_time < 1000000000) {
      print(F("."));
      delay(2000);
    } else {
      printf("\n[Util] Fetched NTP epoch time : %lu.\r\n", epoch_time);
      printf("[Util] Current time : %s.\r\n", ctime(&epoch_time));
      break;
    }
  }
}

/**
 * @fn char *Util::StringToChar(String str)
 * @brief Convert String to char
 * @param str String variable
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
char *Util::StringToChar(String str) {
  char *ret = (char *)malloc(str.length() + 1);
  for (int i = 0; i < str.length(); i++) {
    ret[i] = (char)str.charAt(i);
  }
  ret[str.length()] = '\0';
  return ret;
}

/**
 * @fn void Util::checkConnection(char *base_url, char *device_id, time_t
 * interval)
 * @brief Check Device's connection
 * @param base_url url
 * @param device_id device id
 * @param interval The time interval
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void Util::checkConnection(char *base_url, char *device_id, time_t interval) {
  _current_time = time(nullptr);
  if (_last_connection_check_time == -1) {
    _last_connection_check_time = _current_time;
    return;
  }
  if (_current_time - _last_connection_check_time < interval ||
      _current_time == _last_connection_check_time) {
    return;
  }
  _last_connection_check_time = _current_time;
  HTTPClient http;
  String url = base_url;
  url += device_id;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.setConnectTimeout(TIMEOUT);
  println(url);
  int http_code = http.GET();
  String payload = http.getString();
  http.end();

  DynamicJsonDocument root(4096);
  DeserializationError error = deserializeJson(root, payload);
  bool ret = true;
  const char *updated_at;
  if (error) {
    printf("Parse %s failed.\r\n", payload.c_str());
  }
  if (!root["Connection"].isNull()) {
    ret = root["Connection"];
  }
  if (!root["connection"].isNull()) {
    ret = root["connection"];
  }
  if (!root["UpdatedAt"].isNull()) {
    updated_at = root["UpdatedAt"];
  }
  if (!root["updatedAt"].isNull()) {
    updated_at = root["updatedAt"];
  }
  root.clear();
  if (!ret && updated_at != NULL) {
    int y, M, d, h, m;
    float s;
    sscanf(updated_at, "%d-%d-%dT%d:%d:%fZ", &y, &M, &d, &h, &m, &s);
    tm t;
    t.tm_year = y - 1900;  // Year since 1900
    t.tm_mon = M - 1;      // 0-11
    t.tm_mday = d;         // 1-31
    t.tm_hour = h;         // 0-23
    t.tm_min = m;          // 0-59
    t.tm_sec = (int)s;     // 0-61 (0-60 in C++11)
    _disconnect_time = mktime(&t);
    if (_disconnect_time > 0 && _current_time - _disconnect_time > 120) {
      println(F("[Util] Device is disconnected and will reboot"));
      ESP.restart();
    }
  } else
    _disconnect_time = -1;
}