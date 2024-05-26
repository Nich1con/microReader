#pragma once
#include <Arduino.h>

namespace su {

// быстрое возведение 10 в степень
uint32_t getPow10(uint8_t value);

// быстрый целочисленный логарифм 10 (длина числа в кол-ве символов)
uint8_t getLog10(const uint32_t& value);

// быстрый целочисленный логарифм 10 (длина числа в кол-ве символов)
uint8_t getLog10(const int32_t& value);

/**
 * @brief Длина строки с русскими символами
 *
 * @param str строка
 * @return uint16_t длина
 */
uint16_t strlenRu(const char* str);

/**
 * @brief Получить длину целого числа (с учётом знака -)
 *
 * @param val
 * @return uint8_t
 */
uint8_t intLen(const int32_t& val);

/**
 * @brief Получить длину float числа (с учётом знака -)
 *
 * @param val
 * @param dec
 * @return uint8_t
 */
uint8_t floatLen(const double& val, uint8_t dec);

/**
 * @brief Преобразовать строку в целое число
 *
 * @tparam T тип целого
 * @param str строка
 * @param len опционально длина
 * @return T
 */
template <typename T>
T strToInt(const char* str, uint8_t len = 0) {
    T v = 0;
    bool n = 0;
    const char* p = str;
    if (*p == '-') n = 1, p++;
    while (1) {
        if (!*p || *p < '0' || *p > '9' || (len && p - str >= len)) break;
        v = v * 10 + (*p & 0xF);
        p++;
    }
    return n ? -v : v;
}

/**
 * @brief Преобразовать PROGMEM строку в целое число
 *
 * @tparam T тип целого
 * @param str строка
 * @param len опционально длина
 * @return T
 */
template <typename T>
T strToInt_P(const char* str, uint8_t len = 0) {
    T v = 0;
    bool n = 0;
    const char* p = str;
    if ((char)pgm_read_byte(p) == '-') n = 1, p++;
    while (1) {
        char c = (char)pgm_read_byte(p++);
        if (!c || c < '0' || c > '9' || (len && p - str > len)) break;
        v = v * 10 + (c & 0xF);
    }
    return n ? -v : v;
}

/**
 * @brief Преобразовать float в строку с указанием кол-ва знаков после точки
 *
 * @param val
 * @param buf
 * @param dec кол-во знаков после точки
 * @return uint8_t длина полученной строки
 */
uint8_t floatToStr(double val, char* buf, uint8_t dec);

/**
 * @brief Преобразовать HEX строку в целое число
 *
 * @param str строка
 * @param len длина числа в строке (не указывать, если строка заканчивается '\0')
 * @return uint32_t
 */
uint32_t strToIntHex(const char* str, int8_t len = -1);

/**
 * @brief Длина символа в количестве байт
 *
 * @param sym символ utf-8
 * @return uint8_t 0 если некорректный символ или продолжение предыдущего
 */
uint8_t charSize(char sym);

/**
 * @brief Быстрая конвертация числа в char* массив (в 3-8 раз быстрее ltoa)
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t uintToStr(uint32_t n, char* buf, uint8_t base = DEC);

/**
 * @brief Быстрая конвертация числа в char* массив (в 3-8 раз быстрее ltoa)
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t intToStr(int32_t n, char* buf, uint8_t base = DEC);

/**
 * @brief Конвертация числа в char* массив
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t uint64ToStr(uint64_t n, char* buf, uint8_t base = DEC);

/**
 * @brief Конвертация числа в char* массив
 *
 * @param num число
 * @param buf char массив
 * @param base основание (DEC, HEX, OCT, BIN)
 * @return uint8_t длина числа
 */
uint8_t int64ToStr(int64_t n, char* buf, uint8_t base = DEC);

// конвертация из строки во float
float strToFloat(const char* s);

// конвертация из PROGEMEM строки во float
float strToFloat_P(PGM_P s);

}  // namespace su