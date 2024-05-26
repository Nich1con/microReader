#pragma once
#include <Arduino.h>

namespace su {

// хэш строки, выполняется на этапе компиляции
template <typename T>
static constexpr T _hash_c(char const* str, T hash = 0) {
    return (*str ? _hash_c<T>(str + 1, hash + (hash << 5) + *str) : hash);
}

// хэш строки, выполняется в рантайме
template <typename T>
static T _hash(const char* str, int16_t len = -1) {
    if (!str) return 0;
    T hash = 0;
    while (*str && len--) hash = hash + (hash << 5) + *str++;
    return hash;
}

// хэш PROGMEM строки, выполняется в рантайме
template <typename T>
static T _hash_P(PGM_P str, int16_t len = -1) {
    if (!str) return 0;
    T hash = 0;
    while (len--) {
        uint8_t c = pgm_read_byte(str++);
        if (!c) break;
        hash = hash + (hash << 5) + c;
    }
    return hash;
}

// =====================================================

// StringLength длина строки, выполняется на этапе компиляции
constexpr size_t SL(char const* str, size_t len = 0) {
    return *str ? SL(str + 1, len + 1) : len;
}

// StringHash хэш строки, выполняется на этапе компиляции. Размер зависит от платформы и соответствует size_t
constexpr size_t SH(const char* str) {
    return _hash_c<size_t>(str);
}

// StringHash хэш строки, выполняется на этапе компиляции. Размер 32 бит
constexpr uint32_t SH32(const char* str) {
    return _hash_c<uint32_t>(str);
}

// =====================================================

// хэш строки, выполняется в рантайме. Размер зависит от платформы и соответствует size_t
size_t hash(const char* str, int16_t len = -1);

// хэш строки, выполняется в рантайме. Размер 32 бит
uint32_t hash32(const char* str, int16_t len = -1);

// хэш PROGMEM строки, выполняется в рантайме. Размер зависит от платформы и соответствует size_t
size_t hash_P(PGM_P str, int16_t len = -1);

// хэш PROGMEM строки, выполняется в рантайме. Размер 32 бит
uint32_t hash32_P(PGM_P str, int16_t len = -1);

}  // namespace su