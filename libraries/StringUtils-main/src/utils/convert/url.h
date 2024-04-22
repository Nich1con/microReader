#pragma once
#include <Arduino.h>

namespace su {
namespace url {

// символ должен быть urlencoded
bool needsEncode(char c);

// закодировать в url. Можно указать len = 0, если неизвестна
void encode(const char* src, uint16_t len, String& dest);

// закодировать в url
void encode(const String& src, String& dest);

// закодировать в url
String encode(const String& src);

// раскодировать url
void decode(const char* src, uint16_t len, String& dest);

// раскодировать url
void decode(const String& src, String& dest);

// раскодировать url
String decode(const String& src);

}  // namespace url
}  // namespace su