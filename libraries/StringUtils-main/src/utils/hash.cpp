#include "hash.h"

namespace su {

// хэш строки, выполняется в рантайме. Размер зависит от платформы и соответствует size_t
size_t hash(const char* str, int16_t len) {
    return _hash<size_t>(str, len);
}

// хэш строки, выполняется в рантайме. Размер 32 бит
uint32_t hash32(const char* str, int16_t len) {
    return _hash<uint32_t>(str, len);
}

// хэш PROGMEM строки, выполняется в рантайме. Размер зависит от платформы и соответствует size_t
size_t hash_P(PGM_P str, int16_t len) {
    return _hash_P<size_t>(str, len);
}

// хэш PROGMEM строки, выполняется в рантайме. Размер 32 бит
uint32_t hash32_P(PGM_P str, int16_t len) {
    return _hash_P<uint32_t>(str, len);
}

/*
#define FNV1A_16_BASIS 0x811C
#define FNV1A_16_PRIME 0x011D
#define FNV1A_32_BASIS 0x811C9DC5
#define FNV1A_32_PRIME 0x01000193

// hash FNV-1a 16 бит, выполняется на этапе компиляции
constexpr uint16_t hash16_c(char const* str, uint16_t hash = FNV1A_16_BASIS) {
    return (*str ? hash16_c(str + 1, (hash ^ *str) * FNV1A_16_PRIME) : hash);
}

// hash FNV-1a 32 бит, выполняется на этапе компиляции
constexpr uint32_t hash32_c(char const* str, uint32_t hash = FNV1A_32_BASIS) {
    return (*str ? hash32_c(str + 1, (hash ^ *str) * FNV1A_32_PRIME) : hash);
}

// hash FNV-1a 16 бит, выполняется в рантайме. ~1.4 мкс на символ (AVR)
uint16_t hash16(const char* str, int16_t len = -1) {
    uint16_t hash = FNV1A_16_BASIS;
    while (*str && len) {
        hash = (hash ^ *str++) * FNV1A_16_PRIME;
        if (len) len--;
    }
    return hash;
}

// hash FNV-1a 16 бит для PROGMEM, выполняется в рантайме. ~1.4 мкс на символ (AVR)
uint16_t hash16_P(PGM_P str, int16_t len = -1) {
    uint16_t hash = FNV1A_16_BASIS;
    while (len) {
        uint8_t c = pgm_read_byte(str++);
        if (!c) break;
        hash = (hash ^ c) * FNV1A_16_PRIME;
        if (len) len--;
    }
    return hash;
}

// hash FNV-1a 32 бит, выполняется в рантайме. ~6 мкс на символ (AVR)
uint32_t hash32(const char* str, int16_t len = -1) {
    uint32_t hash = FNV1A_32_BASIS;
    while (*str && len) {
        hash = (hash ^ *str++) * FNV1A_32_PRIME;
        if (len) len--;
    }
    return hash;
}

// hash FNV-1a 32 бит для PROGMEM, выполняется в рантайме. ~6 мкс на символ (AVR)
uint32_t hash32_P(PGM_P str, int16_t len = -1) {
    uint32_t hash = FNV1A_32_BASIS;
    while (len) {
        uint8_t c = pgm_read_byte(str++);
        if (!c) break;
        hash = (hash ^ c) * FNV1A_32_PRIME;
        if (len) len--;
    }
    return hash;
}

struct Hash16 {
    const uint16_t hash;
};

// hash FNV-1a 16 бит, выполняется на этапе компиляции, возвращает Hash16
constexpr Hash16 H16(const char* str) {
    return (Hash16){hash16_c(str)};
}

struct Hash32 {
    const uint32_t hash;
};

// hash FNV-1a 16 бит, выполняется на этапе компиляции, возвращает Hash32
constexpr Hash32 H32(const char* str) {
    return (Hash32){hash32_c(str)};
}

struct Hash_t {
    const size_t hash;
};

// хэш строки, выполняется на этапе компиляции. Возвращает структуру Hash_t
constexpr Hash_t Hash(const char* str) {
    return (Hash_t){_hash_c<size_t>(str)};
}
*/

}  // namespace su