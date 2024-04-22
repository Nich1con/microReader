#pragma once
// быстрые функции пинов
// #define GIO_USE_ARDUINO

#ifdef GIO_USE_ARDUINO
#include "gio_arduino.h"
#else

#if defined(__AVR__) || defined(ARDUINO_ARCH_AVR)
#include "gio_avr.h"
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
#include "gio_esp8266.h"
#elif defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
#include "gio_esp32.h"
#else
#include "gio_arduino.h"
#endif

#endif  // GIO_USE_ARDUINO

/*
us (MHz)
|         | digitalWrite | gio_write | gio_toggle | digitalRead | gio_read   | pinMode      | gio_mode   |
|---------|--------------|-----------|------------|-------------|------------|--------------|------------|
| AVR     | 1.47 (0.68)  | 0.06 (16) | 0.06 (16)  | 3.23 (0.31) | 0.13 (8)   | 3.33 (0.3)   | 0.13 (8)   |
| ESP8266 | 1.56 (0.64)  | 0.08 (12) | 0.5 (2)    | 0.56 (1.8)  | 0.21 (4.8) | 1.43 (0.7)   | 0.67 (1.5) |
| ESP32   | 0.33 (3)     | 0.04 (24) | 0.13 (8)   | 1.67 (0.6)  | 0.08 (12)  | 16.67 (0.06) | 0.08 (12)  |
| ESP32C3 | 0.92 (1)     | 0.06 (16) | 0.19 (5.2) | 0.25 (4)    | 0.04 (24)  | 20 (0.05)    | 0.04 (24)  |
*/