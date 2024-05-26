#pragma once
#include <Arduino.h>

#include "Text.h"

// парсинг по разделителям

namespace su {

class Parser {
   public:
    Parser(String& list, char div = ';') : Parser((char*)list.c_str(), div) {}
    
    Parser(char* list, char div = ';') : _str((char*)list), _div(div) {
        if (!*list) _stop = 1;
    }

    // установить разделитель
    void setDiv(char div) {
        _div = div;
    }

    // парсить следующую подстроку. Вернёт false, если парсинг закончен
    bool next() {
        if (!_stop && _end >= 0) _str[_end] = _div;
        if (_stop) return 0;
        _st = ++_end;
        char* n = strchr(_str + _end, _div);
        if (!n) {
            n = strchr(_str + _end, 0);
            _stop = 1;
        }
        _end = n - _str;
        _str[_end] = 0;
        if (_st) _idx++;
        return 1;
    }

    // индекс текущей подстроки
    uint8_t index() {
        return _idx;
    }

    // получить подстроку
    const char* str() {
        return _str ? (_str + _st) : "";
    }

    // получить подстроку как Text
    Text get() {
        return Text(str(), _end - _st);
    }

   private:
    char* _str = nullptr;
    int16_t _st = 0, _end = -1;
    char _div;
    uint8_t _idx = 0;
    bool _stop = 0;
};

}  // namespace su