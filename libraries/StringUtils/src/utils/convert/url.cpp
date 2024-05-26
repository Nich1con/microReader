#include "url.h"

namespace su {
namespace url {

// символ должен быть urlencoded
bool needsEncode(char c) {
    switch (c) {
        case '0' ... '9':
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '.':
        case '-':
        case '_':
        case '~':
            // case '!': case '+': case ',':
            // case '*': case ':': case '@':
            return 0;
    }
    return 1;
}

// закодировать в url. Можно указать len = 0, если неизвестна
void encode(const char* src, uint16_t len, String& dest) {
    if (!len) len = strlen(src);
    dest.reserve(len);
    const char* end = src + len;
    while (src < end) {
        char c = *src++;
        if (needsEncode(c)) {
            dest += '%';
            dest += (char)((c >> 4) + (((c >> 4) > 9) ? 55 : '0'));
            dest += (char)((c & 0xF) + (((c & 0xF) > 9) ? 55 : '0'));
        } else {
            dest += c;
        }
    }
}

// закодировать в url
void encode(const String& src, String& dest) {
    encode(src.c_str(), src.length(), dest);
}

// закодировать в url
String encode(const String& src) {
    String dest;
    encode(src, dest);
    return dest;
}

static uint8_t _decodeNibble(char c) {
    return c - ((c <= '9') ? '0' : ((c <= 'F') ? 55 : 87));
}

// раскодировать url
void decode(const char* src, uint16_t len, String& dest) {
    if (!len) len = strlen(src);
    dest.reserve(len);
    const char* end = src + len;
    while (src < end) {
        char c = *src++;
        if (c != '%') {
            dest += (c == '+') ? ' ' : c;
        } else {
            char c1 = *src++;
            char c2 = *src++;
            if (!c1 || !c2 || src >= end) return;
            dest += char(_decodeNibble(c2) | (_decodeNibble(c1) << 4));
        }
    }
}

// раскодировать url
void decode(const String& src, String& dest) {
    decode(src.c_str(), src.length(), dest);
}

// раскодировать url
String decode(const String& src) {
    String dest;
    decode(src, dest);
    return dest;
}

}  // namespace url
}  // namespace su