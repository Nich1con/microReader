#pragma once
#include <Arduino.h>

#include "Text.h"

namespace su {
namespace list {

/**
 * @brief Получить количество подстрок в списке
 *
 * @param list строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return uint16_t
 */
uint16_t length(const Text& list, char div = ';');

/**
 * @brief Получить индекс подстроки в списке
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return int16_t индекс в строке. -1 если не найдена
 */
int16_t indexOf(const Text& list, const Text& str, char div = ';');

/**
 * @brief Проверка содержит ли список подстроку
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return true содержит
 * @return false не содержит
 */
bool includes(const Text& list, const Text& str, char div = ';');

/**
 * @brief Получить подстроку из списка по индексу
 *
 * @param list строка любого типа
 * @param idx строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return Text подстрока
 */
Text get(const Text& list, uint16_t idx, char div = ';');

// распарсить в массив указанного типа и размера. Вернёт количество записанных подстрок
template <typename T>
uint16_t parse(const Text& list, T* buf, uint16_t len, char div = ';') {
    if (!list.valid() || !list.length()) return 0;
    uint16_t idx = 0;
    int16_t st = 0, end = -1;
    bool stop = 0;
    while (1) {
        st = ++end;
        end = list.indexOf(div, end);
        if (end < 0) {
            end = list.length();
            stop = 1;
        }
        buf[idx] = Text(list.str() + st, end - st, list.pgm());
        idx++;
        if (stop) return idx;
        if (idx == len) return len;
    }
}

}  // namespace list

class List : public Text {
   public:
    using Text::Text;

    // установить разделитель
    void setDiv(char div) {
        _div = div;
    }

    // получить размер списка
    uint16_t length() const {
        return list::length(*this, _div);
    }

    // получить индекс подстроки в списке или -1 если её нет
    int16_t indexOf(const Text& str) const {
        return list::indexOf(*this, str, _div);
    }

    // проверить наличие подстроки в списке
    bool includes(const Text& str) const {
        return list::includes(*this, str, _div);
    }

    // распарсить в массив указанного типа и размера. Вернёт количество записанных подстрок
    template <typename T>
    uint16_t parse(T* buf, uint16_t len) const {
        return list::parse<T>(*this, buf, len, _div);
    }

    // получить подстроку под индексом
    Text get(uint16_t idx) const {
        return list::get(*this, idx, _div);
    }

    // получить подстроку под индексом
    Text operator[](int idx) const {
        return get(idx);
    }

   private:
    char _div = ';';
};

}  // namespace su