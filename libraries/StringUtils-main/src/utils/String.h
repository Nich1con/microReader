#pragma once
#include <Arduino.h>

#include "Text.h"
#include "Value.h"

namespace su {

class StringExt : public Text {
   public:
    StringExt(char* buf, uint16_t capacity) : capacity(capacity) {
        _str = buf;
    }

    // очистить
    void clear() {
        _len = 0;
    }

    // завершить нулём
    void terminate() {
        if (_len < capacity && !terminated()) {
            concatChar(0);
            _len--;
        }
    }

    // автоматически завершать нулём (умолч. false)
    void terminateAuto(bool ter) {
        autoter = ter;
    }

    // обрезать с from до конца
    void trunc(uint16_t from) {
        if (from < _len) _len = from;
    }

    // прибавить
    bool concat(const Value& txt) {
        if (!_str) return 0;
        _len += txt.toStr((char*)_str + _len, capacity - _len, autoter);
        return 1;
    }

    // прибавить
    bool concat(const char* str, uint16_t len) {
        return concat(Text(str, len));
    }

    // прибавить
    bool concat(double val, uint8_t dec) {
        return concat(Value(val, dec));
    }

    // присвоить
    bool assign(const Value& txt) {
        if (!_str) return 0;
        _len = 0;
        return concat(txt);
    }

    // присвоить
    bool assign(const char* str, uint16_t len) {
        return assign(Text(str, len));
    }

    // присвоить
    bool assign(double val, uint8_t dec) {
        return assign(Value(val, dec));
    }

    // operator
    StringExt& operator=(const Text& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const Text& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const Text& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const char* value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const char* value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const char* value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const __FlashStringHelper* value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const __FlashStringHelper* value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const __FlashStringHelper* value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const String& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const String& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const String& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(String& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(String& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(String& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const char& value) {
        assignChar(value);
        return *this;
    }
    StringExt& operator+=(const char& value) {
        concatChar(value);
        return *this;
    }
    StringExt& operator+(const char& value) {
        concatChar(value);
        return *this;
    }

    StringExt& operator=(const bool& value) {
        assignChar(value + '0');
        return *this;
    }
    StringExt& operator+=(const bool& value) {
        concatChar(value + '0');
        return *this;
    }
    StringExt& operator+(const bool& value) {
        concatChar(value + '0');
        return *this;
    }

    StringExt& operator=(const double& value) {
        assign(value, 2);
        return *this;
    }
    StringExt& operator+=(const double& value) {
        concat(value, 2);
        return *this;
    }
    StringExt& operator+(const double& value) {
        concat(value, 2);
        return *this;
    }

    StringExt& operator=(const unsigned char& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const unsigned char& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const unsigned char& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const short& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const short& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const short& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const unsigned short& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const unsigned short& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const unsigned short& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const int& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const int& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const int& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const unsigned int& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const unsigned int& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const unsigned int& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const long& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const long& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const long& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const unsigned long& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const unsigned long& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const unsigned long& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const long long& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const long long& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const long long& value) {
        concat(value);
        return *this;
    }

    StringExt& operator=(const unsigned long long& value) {
        assign(value);
        return *this;
    }
    StringExt& operator+=(const unsigned long long& value) {
        concat(value);
        return *this;
    }
    StringExt& operator+(const unsigned long long& value) {
        concat(value);
        return *this;
    }

   private:
    uint16_t capacity = 0;
    bool autoter = false;

    void concatChar(const char& sym) {
        if (!_str || _len + 1 > capacity) return;
        ((char*)_str)[_len] = sym;
        _len++;
    }
    void assignChar(const char& sym) {
        if (!_str) return;
        _len = 0;
        concat(sym);
    }
};

template <uint16_t cap>
class StringStatic : public StringExt {
   public:
    StringStatic() : StringExt(buf, cap) {}

    using StringExt::operator=;
    using StringExt::operator+=;

   private:
    char buf[cap];
};

}  // namespace su