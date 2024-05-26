#pragma once
#include <Arduino.h>

#include "Text.h"
#include "convert/convert.h"

// автоматическое преобразование любых данных к строке Text

#ifndef SU_ANYVALUE_BUF_LEN
#define SU_ANYVALUE_BUF_LEN 21
#endif

namespace su {

// с указанием размера буфера
template <uint8_t bufsize = SU_ANYVALUE_BUF_LEN>
class ValueT : public Text {
   public:
    using Text::Text;

    ValueT() : Text() {}

    ValueT(const Text& value) : Text(value) {}

    ValueT(const bool& value) {
        buf[0] = value + '0';
        buf[1] = 0;
        _len = 1;
        _init();
    }

    ValueT(const char& value) {
        buf[0] = value;
        buf[1] = 0;
        _len = 1;
        _init();
    }
    ValueT(const unsigned char& value, uint8_t base = DEC) {
        _len = intToStr(value, buf, base);
        _init();
    }

    ValueT(const short& value, uint8_t base = DEC) {
        _len = intToStr(value, buf, base);
        _init();
    }
    ValueT(const unsigned short& value, uint8_t base = DEC) {
        _len = uintToStr(value, buf, base);
        _init();
    }

    ValueT(const int& value, uint8_t base = DEC) {
        _len = intToStr(value, buf, base);
        _init();
    }
    ValueT(const unsigned int& value, uint8_t base = DEC) {
        _len = uintToStr(value, buf, base);
        _init();
    }

    ValueT(const long& value, uint8_t base = DEC) {
        _len = intToStr(value, buf, base);
        _init();
    }
    ValueT(const unsigned long& value, uint8_t base = DEC) {
        _len = uintToStr(value, buf, base);
        _init();
    }

    ValueT(const long long& value, uint8_t base = DEC) {
        if (value >= 0 && value < UINT32_MAX) _len = uintToStr(value, buf, base);
        else if (value < 0 && value > -INT32_MAX) _len = intToStr(value, buf, base);
        else _len = int64ToStr(value, buf, base);
        _init();
    }
    ValueT(const unsigned long long& value, uint8_t base = DEC) {
        _len = (value <= UINT32_MAX) ? uintToStr(value, buf, base) : uint64ToStr(value, buf, base);
        _init();
    }

    ValueT(const double& value, uint8_t dec = 2) {
        _len = floatToStr(value, buf, dec);
        _init();
    }

   protected:
    char buf[bufsize] = {0};

   private:
    void _init() {
        _str = buf;
        _type = Type::value;
    }
};

// размер буфера общий для всех экземпляров и задаётся дефайном SU_ANYVALUE_BUF_LEN
class Value : public ValueT<> {
    using ValueT<SU_ANYVALUE_BUF_LEN>::ValueT;
};

}  // namespace su