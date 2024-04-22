#include "b64.h"

namespace su {
namespace b64 {

#ifndef SU_B64_COMPACT
static const char _b64_char[] PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const uint8_t _b64_byte[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0,
    0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
#endif

static char _getChar(uint8_t n) {
#ifdef SU_B64_COMPACT
    if (n < 26) return (n + 'A');
    else if (n < 52) return (n - 26 + 'a');
    else if (n < 62) return (n - 52 + '0');
    else if (n == 62) return '+';
    else return '/';
#else
    return pgm_read_byte(_b64_char + n);
#endif
}

static uint8_t _getByte(char b) {
#ifdef SU_B64_COMPACT
    if (b < 43) return 0;
    else if (b == 43) return 62;
    else if (b < 47) return 0;
    else if (b == 47) return 63;
    else if (b < 58) return b + 4;
    else if (b < 66) return 0;
    else if (b < 91) return b - 65;
    else if (b < 97) return 0;
    else return b - 71;
#else
    return pgm_read_byte(_b64_byte + b);
#endif
}

size_t encodedLen(size_t len) {
    return ((len + 3 - 1) / 3) * 4;  // padded
}

size_t decodedLen(const char* data, size_t len) {
    if (len < 4) return 0;
    uint8_t padd = 0;
    if (data[len - 2] == '=') padd = 2;
    else if (data[len - 1] == '=') padd = 1;
    return ((len + 3) / 4) * 3 - padd;
}

size_t encode(String* out, uint8_t* data, size_t len, bool pgm) {
    size_t slen = out->length();
    out->reserve(slen + encodedLen(len));
    int8_t valb = -6;
    size_t val = 0;
    for (size_t i = 0; i < len; i++) {
        val = (val << 8) + (pgm ? pgm_read_byte(&data[i]) : data[i]);
        valb += 8;
        while (valb >= 0) {
            *out += _getChar((val >> valb) & 0x3F);
            valb -= 6;
        }
    }
    if (valb > -6) *out += _getChar(((val << 8) >> (valb + 8)) & 0x3F);
    while ((out->length() - slen) & 3) *out += '=';  // & 3 == % 4
    return out->length() - slen;
}

size_t encode(char* out, uint8_t* data, size_t len, bool pgm) {
    char* p = out;
    int8_t valb = -6;
    size_t val = 0;
    for (size_t i = 0; i < len; i++) {
        val = (val << 8) + (pgm ? pgm_read_byte(&data[i]) : data[i]);
        valb += 8;
        while (valb >= 0) {
            *p++ = _getChar((val >> valb) & 0x3F);
            valb -= 6;
        }
    }
    if (valb > -6) *p++ = _getChar(((val << 8) >> (valb + 8)) & 0x3F);
    while ((p - out) & 3) *p++ = '=';  // & 3 == % 4
    return p - out;
}

void decode(uint8_t* out, const char* data, size_t len) {
    if (!decodedLen(data, len)) return;
    size_t val = 0, idx = 0;
    int8_t valb = -8;
    for (size_t i = 0; i < len; i++) {
        if (data[i] == '=') break;
        val = (val << 6) + _getByte(data[i]);
        valb += 6;
        if (valb >= 0) {
            out[idx++] = (uint8_t)((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
}

void decode(uint8_t* out, const String& data) {
    decode(out, data.c_str(), data.length());
}

}  // namespace b64
}  // namespace su