#include "list.h"

namespace su {
namespace list {

/**
 * @brief Получить количество подстрок в списке
 *
 * @param list строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return uint16_t
 */
uint16_t length(const Text& list, char div) {
    if (!list.valid() || !list.length()) return 0;
    uint16_t am = 0;
    int16_t p = -1;
    while (1) {
        am++;
        p = list.indexOf(div, p + 1);
        if (p < 0) return am;
    }
}

/**
 * @brief Получить индекс подстроки в списке
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return int16_t индекс в строке. -1 если не найдена
 */
int16_t indexOf(const Text& list, const Text& str, char div) {
    if (!list.valid() || !str.valid() || !list.length() || !str.length()) return -1;
    int16_t idx = 0;
    int16_t st = 0, end = -1;
    bool stop = 0;
    while (1) {
        if (stop) return -1;
        st = ++end;
        end = list.indexOf(div, end);
        if (end < 0) {
            end = list.length();
            stop = 1;
        }
        if (str.length() == (uint16_t)(end - st) && list.compareN(str, str.length(), st)) return idx;
        idx++;
    }
}

/**
 * @brief Проверка содержит ли список подстроку
 *
 * @param list строка любого типа
 * @param str строка любого типа
 * @param div символ-разделитель (умолч. ';')
 * @return true содержит
 * @return false не содержит
 */
bool includes(const Text& list, Text str, char div) {
    return indexOf(list, str, div) >= 0;
}

/**
 * @brief Получить подстроку из списка по индексу
 *
 * @param list строка любого типа
 * @param idx индекс
 * @param div символ-разделитель (умолч. ';')
 * @return Text подстрока
 */
Text get(const Text& list, uint16_t idx, char div) {
    if (!list.valid() || !list.length()) return Text();
    int16_t spos = 0, epos = -1;
    bool stop = 0;
    while (1) {
        epos = list.indexOf(div, epos + 1);
        if (epos < 0) {
            epos = list.length();
            stop = 1;
        }
        if (!idx--) return Text(list.str() + spos, epos - spos, list.pgm());
        if (stop) break;
        spos = epos + 1;
    }
    return Text();
}

}  // namespace list
}  // namespace su