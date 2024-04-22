#pragma once
#include <Arduino.h>

// #define SU_B64_COMPACT

namespace su {
namespace b64 {

// размер закодированных данных по размеру исходных
size_t encodedLen(size_t len);

// будущий размер декодированных данных по строке b64 и её длине
size_t decodedLen(const char* b64, size_t len);

// закодировать данные в String
size_t encode(String* b64, uint8_t* data, size_t len, bool pgm = false);

// закодировать данные в char[] (библиотека не добавляет '\0' в конец)
size_t encode(char* b64, uint8_t* data, size_t len, bool pgm = false);

// раскодировать данные из строки b64 в буфер data
void decode(uint8_t* data, const char* b64, size_t len);

// раскодировать данные из строки b64 в буфер data
void decode(uint8_t* data, const String& b64);

}  // namespace b64
}  // namespace su