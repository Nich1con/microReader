#include "unicode.h"

namespace su {
namespace unicode {

// декодировать строку с unicode символами. зарезервировать строку на длину len. Иначе - по длине строки
String decode(const char* str, uint16_t len) {
    String out;
    if (!len) len = strlen(str);
    out.reserve(len);
    const char* end = str + len;
    uint32_t ub = 0, buf = 0;
    const char* x0 = "\0x";
    while (str < end) {
        if (*str != '\\') {
            out += *str;
        } else {
            str++;
            switch (*str) {
                case '\0':
                    return out;
                case 'n':
                    out += '\n';
                    break;
                case 'r':
                    out += '\r';
                    break;
                case 't':
                    out += '\t';
                    break;
                case 'u':
                    ub = 0;
                    for (uint8_t i = 0; i < 4; i++) {
                        if (++str >= end) return out;
                        ub <<= 4;
                        ub += (*str & 0xf) + (*str > '9' ? 9 : 0);
                    }

                    if ((ub >= 0xD800) && (ub <= 0xDBFF)) {
                        buf = ub;
                    } else if ((ub >= 0xDC00) && (ub <= 0xDFFF)) {
                        ub = (0x10000 + ((buf - 0xD800) * 0x0400) + (ub - 0xDC00));
                        out += (char)(0b11110000 | ((ub >> 18) & 0b111));
                        out += x0;
                        out += (char)(0b10000000 | ((ub >> 12) & 0b111111));
                        out += x0;
                        out += (char)(0b10000000 | ((ub >> 6) & 0b111111));
                        out += x0;
                        out += (char)(0b10000000 | (ub & 0b111111));
                    } else if (ub < 0x800) {
                        out += (char)(0b11000000 | ((ub >> 6) & 0b11111));
                        out += x0;
                        out += (char)(0b10000000 | (ub & 0b111111));
                    } else if (ub >= 0x800) {
                        out += (char)(0b11100000 | ((ub >> 12) & 0b1111));
                        out += x0;
                        out += (char)(0b10000000 | ((ub >> 6) & 0b111111));
                        out += x0;
                        out += (char)(0b10000000 | (ub & 0b111111));
                    }
                    break;
                default:
                    out += *str;
                    break;
            }
        }
        str++;
    }
    return out;
}

// декодировать строку с unicode символами
String decode(const String& str) {
    return decode(str.c_str(), str.length());
}

// кодировать unicode символ по его коду. В массиве должно быть 5 ячеек
void encode(char* str, uint32_t c) {
    char b1 = 0, b2 = 0, b3 = 0, b4 = 0;
    if (c < 0x80) {
        b1 = (c & 0x7F) | 0x00;
    } else if (c < 0x0800) {
        b1 = ((c >> 6) & 0x1F) | 0xC0;
        b2 = ((c >> 0) & 0x3F) | 0x80;
    } else if (c < 0x010000) {
        b1 = ((c >> 12) & 0x0F) | 0xE0;
        b2 = ((c >> 6) & 0x3F) | 0x80;
        b3 = ((c >> 0) & 0x3F) | 0x80;
    } else if (c < 0x110000) {
        b1 = ((c >> 18) & 0x07) | 0xF0;
        b2 = ((c >> 12) & 0x3F) | 0x80;
        b3 = ((c >> 6) & 0x3F) | 0x80;
        b4 = ((c >> 0) & 0x3F) | 0x80;
    }
    str[0] = b1;
    str[1] = b2;
    str[2] = b3;
    str[3] = b4;
    str[4] = 0;
}

// кодировать unicode символ по его коду
String encode(uint32_t code) {
    char sym[5];
    encode(sym, code);
    return sym;
}

}  // namespace unicode
}  // namespace su