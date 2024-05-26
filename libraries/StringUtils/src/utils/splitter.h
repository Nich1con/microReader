#pragma once
#include <Arduino.h>

#include "Text.h"
#include "list.h"

// парсинг по разделителям

namespace su {

// передать размер буфера. -1 - динамический буфер
template <int16_t cap>
class SplitterT {
   public:
    SplitterT(String& str, char div = ';') : SplitterT((char*)str.c_str(), div) {}

    SplitterT(char* str, char div = ';') : _div(div) {
        if (!str || !*str) return;
        uint16_t rsize = cap;
        if (cap >= 0) {
            _str = _arr;
        } else {
            rsize = list::length(str, _div);
            _str = (char**)malloc(rsize * sizeof(char*));
            if (!_str) return;
        }
        char* divp;
        while (_len < rsize) {
            divp = strchr(str, _div);
            _str[_len] = (char*)str;
            _len++;
            if (divp && _len < rsize) {
                str = divp + 1;
                *divp = 0;
            } else {
                break;
            }
        }
    }

    ~SplitterT() {
        restore();
        if (cap < 0 && _str) free(_str);
    }

    // восстановить строку (вернуть разделители)
    void restore() {
        if (_str) {
            for (uint8_t i = 1; i < _len; i++) _str[i][-1] = _div;
        }
    }

    // количество подстрок
    uint8_t length() {
        return _len;
    }

    // получить подстроку по индексу
    const char* str(uint16_t idx) {
        return (_str && idx < _len) ? _str[idx] : "";
    }

    // получить подстроку по индексу как Text
    Text get(uint16_t idx) {
        return str(idx);
    }

    // получить подстроку по индексу как Text
    Text operator[](int idx) {
        return str(idx);
    }

   private:
    char** _str = nullptr;
    char* _arr[cap >= 0 ? cap : 0];
    uint8_t _len = 0;
    char _div;
};

// Splitter с динамическим буфером
class Splitter : public SplitterT<-1> {
   public:
    using SplitterT<-1>::SplitterT;
};

}  // namespace su