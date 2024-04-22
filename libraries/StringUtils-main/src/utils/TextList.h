#pragma once
#include <Arduino.h>

#include "Text.h"

namespace su {

template <int16_t cap>
class TextListT {
   public:
    TextListT(const Text& list, char div) {
        len = list.split(arr, cap, div);
    }
    TextListT(const Text& list, const Text& div) {
        len = list.split(arr, cap, div);
    }

    // количество построк
    uint16_t length() const {
        return len;
    }

    // размер буфера
    uint16_t capacity() const {
        return cap;
    }

    // получить подстроку под индексом
    const Text& get(uint16_t idx) const {
        return arr[idx < len ? idx : cap];
    }

    // получить подстроку под индексом
    const Text& operator[](int idx) const {
        return get(idx);
    }

   private:
    Text arr[cap + 1];  // + dummy
    uint16_t len;
};

class TextList {
   public:
    TextList(const Text& list, char div) {
        len = list.count(div);
        arr = new Text[len];
        list.split(arr, len, div);
    }
    TextList(const Text& list, const Text& div) {
        len = list.count(div);
        arr = new Text[len];
        list.split(arr, len, div);
    }

    ~TextList() {
        if (arr) delete[] arr;
    }

    // количество построк
    uint16_t length() const {
        return len;
    }

    // получить подстроку под индексом
    const Text& get(uint16_t idx) const {
        return (arr && idx < len) ? arr[idx] : dummy;
    }

    // получить подстроку под индексом
    const Text& operator[](int idx) const {
        return get(idx);
    }

   private:
    Text* arr = nullptr;
    Text dummy;
    uint16_t len = 0;
};

}  // namespace su