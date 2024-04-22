#pragma once
#include <Arduino.h>

#include "convert/b64.h"
#include "convert/convert.h"
#include "convert/unicode.h"
#include "hash.h"

namespace su {

class Text : public Printable {
   public:
    enum class Type : uint8_t {
        constChar,  // const char*
        pgmChar,    // PROGMEM
        value,      // буфер Value
    };
    struct find_t {
        int16_t count = 0;
        int16_t start = 0;
        int16_t end = 0;
        bool last = 0;
    };

    // ========================== CONSTRUCTOR ==========================
    Text() {}
    Text(const __FlashStringHelper* str, int16_t len = -1) : _str((PGM_P)str), _len(len >= 0 ? len : strlen_P((PGM_P)str)), _type(Type::pgmChar) {}
    Text(const char* str, int16_t len = -1, bool pgm = 0) : _str(str), _len(len >= 0 ? len : (pgm ? strlen_P(str) : strlen(str ? str : ""))), _type(pgm ? Type::pgmChar : Type::constChar) {}
    Text(const uint8_t* str, uint16_t len) : _str((const char*)str), _len(len) {}
    Text(const String& str) : _str(str.c_str()), _len(str.length()) {}

    // ========================== SYSTEM ==========================
    // Строка из Flash памяти
    inline bool pgm() const {
        return _type == Type::pgmChar;
    }

    // Длина строки
    uint16_t length() const {
        return valid() ? _len : 0;
    }

    // Длина строки с учётом unicode символов
    uint16_t lengthUnicode() const {
        if (!length()) return 0;
        uint16_t count = 0;
        for (uint16_t i = 0; i < _len; i++) {
            if ((_charAt(i) & 0xc0) != 0x80) count++;
        }
        return count;
    }

    // посчитать и вернуть длину строки (const)
    uint16_t readLen() const {
        return valid() ? (pgm() ? strlen_P(_str) : strlen(_str)) : 0;
    }

    // пересчитать и запомнить длину строки (non-const)
    void calcLen() {
        _len = readLen();
    }

    // Тип строки
    Type type() const {
        return _type;
    }

    // Получить указатель на строку. Всегда вернёт указатель, отличный от nullptr!
    const char* str() const {
        return valid() ? _str : "";
    }

    // указатель на конец строки
    const char* end() const {
        return valid() ? (_str + _len) : "";
    }

    // Статус строки
    inline bool valid() const {
        return _str;
    }

    // строка валидна и оканчивается \0
    bool terminated() const {
        return (valid()) ? (_charAt(_len) == 0) : 0;
    }

    // Напечатать в Print
    size_t printTo(Print& p) const {
        if (!length()) return 0;
        size_t ret = 0;
        if (pgm()) {
            if (terminated()) {
                ret = p.print((const __FlashStringHelper*)_str);
            } else {
                for (uint16_t i = 0; i < _len; i++) ret += p.write(_charAt(i));
            }
        } else {
            ret = p.write(_str, _len);
        }
        return ret;
    }

    // ========================== COMPARE ==========================

    bool operator==(const Text& s) const {
        return compare(s);
    }
    bool operator!=(const Text& s) const {
        return !compare(s);
    }
    bool operator==(const char* s) const {
        return compare(s);
    }
    bool operator!=(const char* s) const {
        return !compare(s);
    }
    bool operator==(const __FlashStringHelper* s) const {
        return compare(s);
    }
    bool operator!=(const __FlashStringHelper* s) const {
        return !compare(s);
    }
    bool operator==(const String& s) const {
        return compare(s);
    }
    bool operator!=(const String& s) const {
        return !compare(s);
    }

    /**
       @brief Сравнить со строкой

       @param s
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compare(const char* s) const {
        return (length() && s) ? !_compare(_str, s, false, _len) : 0;
    }
    bool compare(const __FlashStringHelper* s) const {
        return (length() && s) ? !_compare(_str, (PGM_P)s, true, _len) : 0;
    }
    bool compare(const Text& txt) const {
        return (txt.length() == _len) ? !_compareN(_str, txt._str, txt.pgm(), _len) : 0;
    }

    /**
       @brief Сравнить со строкой с указанием количества символов

       @param s строка любого типа
       @param amount количество символов
       @param from индекс, откуда начинать сравнение
       @return true строки совпадают
       @return false строки не совпадают
    */
    bool compareN(const Text& txt, uint16_t amount, uint16_t from = 0) const {
        if (!valid() || !txt.valid() || !amount || amount > txt._len || from + amount > _len) return 0;
        return !_compareN(_str + from, txt._str, txt.pgm(), amount);
    }
    bool compareN(const char* s, uint16_t amount, uint16_t from = 0) const {
        return (valid() && from + amount <= _len) ? !_compareN(_str + from, s, false, amount) : 0;
    }
    bool compareN(const __FlashStringHelper* s, uint16_t amount, uint16_t from = 0) const {
        return (valid() && from + amount <= _len) ? !_compareN(_str + from, (PGM_P)s, true, amount) : 0;
    }

    // ========================== SEARCH ==========================

    // найти символ и получить указатель на первое вхождение
    const char* find(char sym, uint16_t from = 0) const {
        if (!length()) return nullptr;
        int16_t idx = indexOf(sym, from);
        return (idx < 0) ? nullptr : (_str + idx);
    }

    // начинается со строки
    bool startsWith(const char* s) const {
        return length() ? !_compareEnd(_str, s, false, _len) : 0;
    }
    bool startsWith(const __FlashStringHelper* s) const {
        return length() ? !_compareEnd(_str, (PGM_P)s, true, _len) : 0;
    }
    bool startsWith(const Text& txt) const {
        return (length() && txt.length() && txt._len <= _len) ? !_compareN(_str, txt._str, txt.pgm(), txt._len) : 0;
    }

    // заканчивается строкой
    bool endsWith(const Text& txt) const {
        return (length() && txt.length() && txt._len <= _len) ? !_compareN(_str + _len - txt._len, txt._str, txt.pgm(), txt._len) : 0;
    }

    // Найти позицию символа в строке
    int16_t indexOf(char sym, uint16_t from = 0) const {
        if (!length() || from > _len) return -1;
        for (uint16_t i = from; i < _len; i++) {
            if (_charAt(i) == sym) return i;
        }
        return -1;
    }

    // Найти позицию строки в строке
    int16_t indexOf(const Text& txt, uint16_t from = 0) const {
        if (!length() || !txt.length() || (from + txt._len) > _len) return -1;
        for (uint16_t i = from; i <= _len - txt._len; i++) {
            if (!_compareN(_str + i, txt._str, txt.pgm(), txt._len)) return i;
        }
        return -1;
    }
    int16_t indexOf(const char* s, uint16_t from = 0) const {
        if (!length()) return -1;
        for (uint16_t i = from; i < _len; i++) {
            if (!_compareEnd(_str + i, s, false, _len - i)) return i;
        }
        return -1;
    }
    int16_t indexOf(const __FlashStringHelper* s, uint16_t from = 0) const {
        if (!length()) return -1;
        for (uint16_t i = from; i < _len; i++) {
            if (!_compareEnd(_str + i, (PGM_P)s, true, _len - i)) return i;
        }
        return -1;
    }

    /**
     @brief Найти позицию символа в строке с конца

    @param sym символ
    @return int16_t позиция символа, -1 если не найден
    */
    int16_t lastIndexOf(char sym) const {
        if (!length()) return -1;
        for (int16_t i = _len - 1; i >= 0; i--) {
            if (_charAt(i) == sym) return i;
        }
        return -1;
    }

    /**
      @brief Найти позицию строки в строке с конца

      @param str строка
      @return int16_t позиция строки, -1 если не найдена
    */
    int16_t lastIndexOf(const Text& txt) const {
        if (!length() || !txt.length() || txt._len > _len) return -1;
        for (int16_t i = _len - txt._len; i >= 0; i--) {
            if (!_compareN(_str + i, txt._str, txt.pgm(), txt._len)) return i;
        }
        return -1;
    }

    // ========================== SUB ==========================

    // Посчитать количество подстрок, разделённых символом (количество разделителей +1)
    uint16_t count(char sym) const {
        if (!length()) return 0;
        uint16_t sum = 1;
        for (uint16_t i = 0; i < _len; i++) {
            if (_charAt(i) == sym) sum++;
        }
        return sum;
    }

    // Посчитать количество подстрок, разделённых строками (количество разделителей +1)
    uint16_t count(const Text& txt) const {
        if (!length() || !txt.length()) return 0;
        uint16_t sum = 1;
        int16_t pos = 0;
        while (1) {
            pos = indexOf(txt, pos);
            if (pos < 0) break;
            pos += txt._len;
            sum++;
        }
        return sum;
    }

    /**
       @brief Получить подстроку из списка по индексу

       @param idx индекс
       @param div символ-разделитель
       @return Text подстрока
    */
    Text getSub(uint16_t idx, char div) const {
        if (!length()) return Text();
        int16_t start = 0, end = 0;
        while (1) {
            end = indexOf(div, end);
            if (end < 0) end = _len;
            if (!idx--) return Text(_str + start, end - start, pgm());
            if ((uint16_t)end == _len) break;
            end++;
            start = end;
        }
        return Text();
    }

    /**
      @brief Получить подстроку из списка по индексу

      @param idx индекс
      @param div строка-разделитель
      @return Text подстрока
    */
    Text getSub(uint16_t idx, const Text& div) const {
        if (!length() || !div.length() || div._len > _len) return Text();
        int16_t start = 0, end = 0;
        while (1) {
            end = indexOf(div, end);
            if (end < 0) end = _len;
            if (!idx--) return Text(_str + start, end - start, pgm());
            if ((uint16_t)end == _len) break;
            end += div._len;
            start = end;
        }
        return Text();
    }

    // ========================== SPLIT ==========================

    /**
       @brief Разделить по символу-разделителю

       @param arr внешний массив любого типа (Text, числа)
       @param len размер массива
       @param div символ разделитель
       @return uint16_t количество найденных подстрок
    */
    template <typename T>
    uint16_t split(T* arr, uint16_t len, char div) const {
        if (!len || !length()) return 0;
        find_t f;
        while (!f.last) arr[f.count - 1] = _parse(div, 1, len, f);
        return f.count;
    }

    template <typename T>
    uint16_t split(T** arr, uint16_t len, char div) const {
        if (!len || !length()) return 0;
        find_t f;
        while (!f.last) *(arr[f.count - 1]) = _parse(div, 1, len, f);
        return f.count;
    }

    /**
      @brief Разделить по строке-разделителю

      @param arr внешний массив любого типа (Text, числа)
      @param len размер массива
      @param div строка разделитель
      @return uint16_t количество найденных подстрок
    */
    template <typename T>
    uint16_t split(T* arr, uint16_t len, const Text& div) const {
        if (!len || !length() || !div.length() || div._len > _len) return 0;
        find_t f;
        while (!f.last) arr[f.count - 1] = _parse(div, div._len, len, f);
        return f.count;
    }

    template <typename T>
    uint16_t split(T** arr, uint16_t len, const Text& div) const {
        if (!len || !length() || !div.length() || div._len > _len) return 0;
        find_t f;
        while (!f.last) *(arr[f.count - 1]) = _parse(div, div._len, len, f);
        return f.count;
    }

    // ========================== EXPORT ==========================

    // вернёт новую строку с убранными пробельными символами с начала и конца
    Text trim() const {
        if (!length()) return Text();
        Text txt(*this);
        while (txt._len) {
            uint8_t sym = txt._charAt(0);
            if (sym && (sym <= 0x0F || sym == ' ')) {
                txt._str++;
                txt._len--;
            } else break;
        }
        while (txt._len) {
            uint8_t sym = txt._charAt(txt._len - 1);
            if (sym <= 0x0F || sym == ' ') txt._len--;
            else break;
        }
        return txt;
    }

    // выделить подстроку (начало, конец не включая). Отрицательные индексы работают с конца строки
    Text substring(int16_t start, int16_t end = 0) const {
        if (!length()) return Text();
        if (start < 0) start += _len;
        if (!end) end = _len;
        else if (end < 0) end += _len;
        if (start > (int16_t)_len || end > (int16_t)_len) return Text();

        if (end && end < start) {
            int16_t b = end;
            end = start;
            start = b;
        }
        return Text(_str + start, end - start, pgm());
    }

    // Добавить к String строке. Вернёт false при неудаче
    bool addString(String& s) const {
        if (!length() || !_len) return 0;
        if (!s.reserve(s.length() + _len)) return 0;
        if (pgm()) {
            if (terminated()) {
                s += (const __FlashStringHelper*)_str;
            } else {
                for (uint16_t i = 0; i < _len; i++) s += _charAt(i);
            }
        } else {
#if defined(ESP8266) || defined(ESP32)
            s.concat(_str, _len);
#else
            if (terminated()) {
                s.concat(_str);
            } else {
                const char* p = _str;
                for (uint16_t i = 0; i < _len; i++) s += p[i];
            }
#endif
        }

        return 1;
    }

    // Добавить к String строке. Вернёт false при неудаче
    bool addString(String& s, bool decodeUnicode) const {
        if (!valid() || !_len) return 0;
        if (decodeUnicode) {
            if (pgm()) {
                char str[_len + 1];
                strncpy_P(str, _str, _len);
                str[_len] = 0;
                s += unicode::decode(str, _len);
            } else {
                s += unicode::decode(_str, _len);
            }
        } else {
            addString(s);
        }
        return 1;
    }

    // Получить символ по индексу
    char charAt(uint16_t idx) const {
        return (valid() && idx < _len) ? _charAt(idx) : 0;
    }

    // Получить символ по индексу
    char operator[](int idx) const {
        return charAt(idx);
    }

    // ========================== B64 ==========================

    // размер данных (байт), если они b64
    size_t sizeB64() const {
        if (!length()) return 0;
        return b64::decodedLen(_str, _len);
    }

    // вывести в переменную из b64
    bool decodeB64(void* var, size_t size) const {
        if (sizeB64() == size) {
            b64::decode((uint8_t*)var, _str, _len);
            return 1;
        }
        return 0;
    }

    // ========================== HASH ==========================

    // хэш строки, размер зависит от платформы (size_t)
    size_t hash() const {
        return valid() ? (pgm() ? su::hash_P(_str, _len) : su::hash(_str, _len)) : 0;
    }

    // хэш строки 32 бит
    uint32_t hash32() const {
        return valid() ? (pgm() ? su::hash32_P(_str, _len) : su::hash32(_str, _len)) : 0;
    }

    // ========================== CONVERT ==========================

    // Вывести в String строку. Вернёт false при неудаче
    bool toString(String& s, bool decodeUnicode = false) const {
        s = "";
        return addString(s, decodeUnicode);
    }

    // Получить как String строку
    String toString(bool decodeUnicode = false) const {
        if (!valid() || !_len) return String();
        String s;
        toString(s, decodeUnicode);
        return s;
    }

    // Вывести в char массив. Вернёт длину строки. terminate - завершить строку нулём
    uint16_t toStr(char* buf, int16_t bufsize = -1, bool terminate = true) const {
        if (!bufsize) return 0;
        if (!length()) {
            if (terminate) buf[0] = 0;
            return 0;
        }
        if (bufsize > 0 && (int16_t)(_len + terminate) > bufsize) return 0;
        pgm() ? strncpy_P(buf, _str, _len) : strncpy(buf, _str, _len);
        if (terminate) buf[_len] = 0;
        return _len;
    }

    // получить значение как bool
    bool toBool() const {
        return valid() && (charAt(0) == 't' || charAt(0) == '1');
    }

    // получить значение как int 16
    int16_t toInt16() const {
        if (!valid()) return 0;
        return pgm() ? strToInt_P<int16_t>(_str, _len) : strToInt<int16_t>(_str, _len);
    }

    // получить значение как int 32
    int32_t toInt32() const {
        if (!valid()) return 0;
        if (_len < 5) return toInt16();
        return pgm() ? strToInt_P<int32_t>(_str, _len) : strToInt<int32_t>(_str, _len);
    }

    // получить значение как uint 32 из HEX строки
    uint32_t toInt32HEX() const {
        if (!length()) return 0;
        uint8_t i = 0;
        uint32_t v = 0;
        if (_len > 2 && _charAt(0) == '0' && _charAt(1) == 'x') i += 2;
        for (; i < _len; i++) {
            char sym = _charAt(i);
            v <<= 4;
            v += (sym & 0xf) + (sym > '9' ? 9 : 0);
        }
        return v;
    }

    // получить значение как int64
    int64_t toInt64() const {
        if (!valid()) return 0;
        if (_len < 10) return toInt32();
        return pgm() ? strToInt_P<int64_t>(_str, _len) : strToInt<int64_t>(_str, _len);
    }

    // получить значение как float
    float toFloat() const {
        if (!valid()) return 0;
        if (pgm()) {
            char buf[_len + 1];
            strncpy_P(buf, _str, _len);
            buf[_len] = 0;
            return atof(buf);
        } else {
            return atof(_str);
        }
    }

    // ================= CAST =================
    // bool
    operator bool() const {
        return toBool();
    }
    bool operator==(const bool& v) const {
        return toBool() == v;
    }
    bool operator!=(const bool& v) const {
        return toBool() != v;
    }

    // signed char
    operator signed char() const {
        return (char)toInt16();
    }
    bool operator==(const signed char& v) const {
        return toInt16() == v;
    }
    bool operator!=(const signed char& v) const {
        return toInt16() != v;
    }

    // unsigned char
    operator unsigned char() const {
        return toInt16();
    }
    bool operator==(const unsigned char& v) const {
        return (unsigned char)toInt16() == v;
    }
    bool operator!=(const unsigned char& v) const {
        return (unsigned char)toInt16() != v;
    }

    // short
    operator short() const {
        return toInt16();
    }
    bool operator==(const short& v) const {
        return toInt16() == v;
    }
    bool operator!=(const short& v) const {
        return toInt16() != v;
    }

    // unsigned short
    operator unsigned short() const {
        return toInt16();
    }
    bool operator==(const unsigned short& v) const {
        return (unsigned short)toInt16() == v;
    }
    bool operator!=(const unsigned short& v) const {
        return (unsigned short)toInt16() != v;
    }

    // int
    operator int() const {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }
    bool operator==(const int& v) const {
        return toInt16() == v;
    }
    bool operator!=(const int& v) const {
        return toInt16() != v;
    }

    // unsigned int
    operator unsigned int() const {
        return (sizeof(int) == 2) ? toInt16() : toInt32();
    }
    bool operator==(const unsigned int& v) const {
        return (unsigned int)toInt16() == v;
    }
    bool operator!=(const unsigned int& v) const {
        return (unsigned int)toInt16() != v;
    }

    // long
    operator long() const {
        return toInt32();
    }
    bool operator==(const long& v) const {
        return toInt32() == v;
    }
    bool operator!=(const long& v) const {
        return toInt32() != v;
    }

    // unsigned long
    operator unsigned long() const {
        return toInt32();
    }
    bool operator==(const unsigned long& v) const {
        return (unsigned long)toInt32() == v;
    }
    bool operator!=(const unsigned long& v) const {
        return (unsigned long)toInt32() != v;
    }

    // long long
    operator long long() const {
        return toInt64();
    }
    bool operator==(const long long& v) const {
        return toInt64() == v;
    }
    bool operator!=(const long long& v) const {
        return toInt64() != v;
    }

    // unsigned long long
    operator unsigned long long() const {
        return toInt64();
    }
    bool operator==(const unsigned long long& v) const {
        return (unsigned long long)toInt64() == v;
    }
    bool operator!=(const unsigned long long& v) const {
        return (unsigned long long)toInt64() != v;
    }

    // float
    operator float() const {
        return toFloat();
    }
    bool operator==(const float& v) const {
        return toFloat() == v;
    }
    bool operator!=(const float& v) const {
        return toFloat() != v;
    }

    // double
    operator double() const {
        return toFloat();
    }
    bool operator==(const double& v) const {
        return toFloat() == v;
    }
    bool operator!=(const double& v) const {
        return toFloat() != v;
    }

    operator String() const {
        return toString();
    }

    const char* _str = nullptr;
    uint16_t _len = 0;
    Type _type = Type::constChar;

    // получить символ по индексу (без проверок валидности)
    char _charAt(uint16_t idx) const {
        return pgm() ? (char)pgm_read_byte(_str + idx) : *(_str + idx);
    }

    template <typename T>
    Text _parse(const T& div, uint16_t divlen, int16_t len, find_t& f) const {
        if (f.count) f.start = f.end = f.end + divlen;
        f.end = indexOf(div, f.end);
        if (f.end < 0 || f.count + 1 == len) f.end = _len;
        f.count++;
        if (f.count == len || f.end == (int16_t)_len) f.last = 1;
        return Text(_str + f.start, f.end - f.start, pgm());
    }

   protected:
    uint16_t _compareN(const char* s1, const char* s2, bool pgm2, uint16_t len) const {
        while (len) {
            if ((pgm() ? pgm_read_byte(s1++) : *s1++) != (pgm2 ? pgm_read_byte(s2++) : *s2++)) return len;
            len--;
        }
        return 0;
    }
    uint16_t _compare(const char* s1, const char* s2, bool pgm2, uint16_t len) const {
        uint16_t left = _compareN(s1, s2, pgm2, len);
        return left ? left : (pgm2 ? pgm_read_byte(s2 + len) : *(s2 + len));
    }
    bool _compareEnd(const char* s1, const char* s2, bool pgm2, uint16_t len) const {
        uint16_t left = _compareN(s1, s2, pgm2, len);
        return pgm2 ? pgm_read_byte(s2 + len - left) : *(s2 + len - left);
    }
};

}  // namespace su