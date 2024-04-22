#pragma once
#include <Arduino.h>

#include "convert.h"

namespace su {
namespace unicode {

// декодировать строку с unicode символами. зарезервировать строку на длину len. Иначе - по длине строки
String decode(const char* str, uint16_t len = 0);

// декодировать строку с unicode символами
String decode(const String& str);

// кодировать unicode символ по его коду. В массиве должно быть 5 ячеек
void encode(char* str, uint32_t c);

// кодировать unicode символ по его коду
String encode(uint32_t code);

}  // namespace unicode
}  // namespace su