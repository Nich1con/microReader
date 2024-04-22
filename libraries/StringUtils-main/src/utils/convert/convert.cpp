#include "convert.h"

namespace su {

// быстрое возведение 10 в степень
uint32_t getPow10(uint8_t value) {
    switch (value) {
        case 0:
            return 0;
        case 1:
            return 10;
        case 2:
            return 100;
        case 3:
            return 1000;
        case 4:
            return 10000;
        case 5:
            return 100000;
        case 6:
            return 1000000;
        case 7:
            return 10000000;
        case 8:
            return 100000000;
        case 9:
            return 1000000000;
    }
    return 0;
}

// быстрый целочисленный логарифм 10 (длина числа в кол-ве символов)
uint8_t getLog10(const uint32_t& value) {
    switch (value) {
        case 0 ... 9:
            return 1;
        case 10 ... 99:
            return 2;
        case 100 ... 999:
            return 3;
        case 1000 ... 9999:
            return 4;
        case 10000 ... 99999:
            return 5;
        case 100000 ... 999999:
            return 6;
        case 1000000 ... 9999999:
            return 7;
        case 10000000 ... 99999999:
            return 8;
        case 100000000 ... 999999999:
            return 9;
        case 1000000000 ... UINT32_MAX:
            return 10;
    }
    return 1;
}

uint8_t getLog10(const int32_t& value) {
    return getLog10((value < 0) ? (uint32_t)(-value) : (uint32_t)value);
}

/**
 * @brief Длина строки с русскими символами
 *
 * @param str строка
 * @return uint16_t длина
 */
uint16_t strlenRu(const char* str) {
    uint16_t count = 0;
    while (*str) {
        if ((*str & 0xc0) != 0x80) count++;
        str++;
    }
    return count;
}

/**
 * @brief Получить длину целого числа (с учётом знака -)
 *
 * @param val
 * @return uint8_t
 */
uint8_t intLen(const int32_t& val) {
    return getLog10(val) + (val < 0 ? 1 : 0);
}

/**
 * @brief Получить длину float числа (с учётом знака -)
 *
 * @param val
 * @param dec
 * @return uint8_t
 */
uint8_t floatLen(const double& val, uint8_t dec) {
    return intLen((int32_t)val) + (dec ? (dec + 1) : 0);
}

/**
 * @brief Преобразовать float в строку с указанием кол-ва знаков после точки
 *
 * @param val
 * @param buf
 * @param dec кол-во знаков после точки
 * @return uint8_t длина полученной строки
 */
uint8_t floatToStr(double val, char* buf, uint8_t dec) {
    dtostrf(val, dec + 2, dec, buf);
    return floatLen(val, dec);
}

/**
 * @brief Преобразовать HEX строку в целое число
 *
 * @param str строка
 * @param len длина числа в строке (не указывать, если строка заканчивается '\0')
 * @return uint32_t
 */
uint32_t strToIntHex(const char* str, int8_t len) {
    uint32_t v = 0;
    if (str[0] == '0' && str[1] == 'x') str += 2;
    while (*str && len) {
        v <<= 4;
        v += (*str & 0xf) + (*str > '9' ? 9 : 0);
        str++;
        if (len > 0) len--;
    }
    return v;
}

/**
 * @brief Длина символа в количестве байт
 *
 * @param sym символ utf-8
 * @return uint8_t 0 если некорректный символ или продолжение предыдущего
 */
uint8_t charSize(char sym) {
    if ((sym & 0x80) == 0x00) return 1;
    else if ((sym & 0xE0) == 0xC0) return 2;
    else if ((sym & 0xF0) == 0xE0) return 3;
    else return 0;
}

// http://we.easyelectronics.ru/Soft/preobrazuem-v-stroku-chast-1-celye-chisla.html
struct _fdiv10 {
    _fdiv10(uint32_t num) {
        quot = num >> 1;
        quot += quot >> 1;
        quot += quot >> 4;
        quot += quot >> 8;
        quot += quot >> 16;
        uint32_t qq = quot;
        quot >>= 3;
        rem = uint8_t(num - ((quot << 1) + (qq & ~7ul)));
        if (rem > 9) rem -= 10, quot++;
    }
    uint32_t quot;
    uint8_t rem;
};

/**
 * @brief Быстрая конвертация числа в char* массив (в 3-8 раз быстрее ltoa)
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t uintToStr(uint32_t n, char* buf, uint8_t base) {
    char* p = buf;
    if (base == DEC) {
        do {
            _fdiv10 res(n);
            n = res.quot;
            *p++ = res.rem + '0';
        } while (n);
    } else {
        do {
            uint8_t c = n & (base - 1);
            n >>= (base == HEX) ? 4 : 1;
            *p++ = (c < 10) ? (c + '0') : (c + 'a' - 10);
        } while (n);
    }
    uint8_t len = p - buf;
    p--;
    char b;
    while (p > buf) {
        b = *buf;
        *buf = *p;
        *p = b;
        buf++;
        p--;
    }
    return len;
}

/**
 * @brief Быстрая конвертация числа в char* массив (в 3-8 раз быстрее ltoa)
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t intToStr(int32_t n, char* buf, uint8_t base) {
    char* p = buf;
    if (n < 0 && base == DEC) *p++ = '-';
    p += uintToStr((n < 0 && base == DEC) ? -n : n, p, base);
    *p = 0;
    return p - buf;
}

/**
 * @brief Конвертация числа в char* массив
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t uint64ToStr(uint64_t n, char* buf, uint8_t base) {
    char* p = buf;
    if (base == DEC) {
        do {
            uint8_t mod = n % base;
            n /= base;
            *p++ = mod + '0';
        } while (n);
    } else {
        do {
            uint8_t c = n & (base - 1);
            n >>= (base == HEX) ? 4 : 1;
            *p++ = (c < 10) ? (c + '0') : (c + 'a' - 10);
        } while (n);
    }
    uint8_t len = p - buf;
    p--;
    char b;
    while (p > buf) {
        b = *buf;
        *buf = *p;
        *p = b;
        buf++;
        p--;
    }
    return len;
}

/**
 * @brief Конвертация числа в char* массив
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t int64ToStr(int64_t n, char* buf, uint8_t base) {
    char* p = buf;
    if (n < 0) *p++ = '-';
    p += uint64ToStr((n < 0) ? -n : n, p, base);
    *p = 0;
    return p - buf;
}

// конвертация из строки во float
float strToFloat(const char* s) {
    float f = strToInt<int32_t>(s);
    char* d = strchr(s, '.');
    if (d) {
        f += (float)strToInt<int32_t>(d + 1) / getPow10(strchr(d, 0) - d - 1) * (f < 0 ? -1 : 1);
    }
    return f;
}

// конвертация из PROGEMEM строки во float
float strToFloat_P(PGM_P s) {
    float f = strToInt_P<int32_t>(s);
    char* d = strchr(s, '.');
    if (d) {
        f += (float)strToInt_P<int32_t>(d + 1) / getPow10(strchr(d, 0) - d - 1);
    }
    return f;
}

}  // namespace su