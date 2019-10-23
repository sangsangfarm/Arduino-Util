/**
 * @file Util.cpp
 * @brief 자주 쓰는 기본 아두이노 기능
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#include <Util.h>

/**
 * @fn void Util::beginSerial(int speed)
 * @brief 시리얼 통신 시작
 * @param speed 모니터링 속도
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
 * @brief 시간 동기화. 시도횟수 초과시 리부팅됨
 * @param timezone 타임존
 * @param try_num 시간 초기화 시도 횟수
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void Util::syncTime(int timezone, int try_num) {
  time_t epoch_time;
  configTime(timezone, 0, "pool.ntp.org", "time.nist.gov");
  print(F("[Util] 시간 설정 중"));
  int count = 0;
  while (true) {
    count++;
    if (count > try_num) {
      println("[Util] 시간 조정 실패. 디바이스 재부팅.");
      ESP.restart();
    }
    epoch_time = time(nullptr);

    if (epoch_time == 0 || epoch_time < 1000000000) {
      print(F("."));
      delay(2000);
    } else {
      printf("\n[Util] Fetched NTP epoch time은: %lu.\r\n", epoch_time);
      printf("[Util] 현재 시간은 : %s.\r\n", ctime(&epoch_time));
      break;
    }
  }
}

/**
 * @fn char *Util::StringToChar(String str)
 * @brief String변수 char*로 변환
 * @param str char*로 변환할 String 변수
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
// String 변수 char*로 변환
char *Util::StringToChar(String str) {
  char *ret = (char *)malloc(str.length() + 1);
  for (int i = 0; i < str.length(); i++) {
    ret[i] = (char)str.charAt(i);
  }
  ret[str.length()] = '\0';
  return ret;
}
