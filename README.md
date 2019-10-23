# Arduino-Util

Utils in Arudino.

## Support platform

- esp8226
- esp32

## Usage

### Begin serial

```cpp
int montior_speed = 115200;
beginSerial(montior_speed);
```

### Sync time

```cpp
int KST = 9 * 60 * 60; // UTC = 0;
syncTime(KST);
```

### String to char

```cpp
String str = "hello!";
char *ret = StringToChar(str);
```
