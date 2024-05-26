#pragma once
#include <Arduino.h>

#include "Text.h"

namespace su {

class TextParser : public Text {
   public:
    TextParser(const Text& txt, char div) : txt(txt), divc(div) {}
    TextParser(const Text& txt, const Text& div) : txt(txt), divt(div) {}

    // парсить. Вернёт true, если найдена подстрока
    bool parse() {
        if (f.last) return 0;
        if (divc) *((Text*)this) = txt._parse(divc, 1, -1, f);
        else *((Text*)this) = txt._parse(divt, divt._len, -1, f);
        idx++;
        return 1;
    }

    // индекс подстроки (всегда > 0)
    int index() {
        return idx < 0 ? 0 : idx;
    }

    // получить текущую подстроку
    const Text& get() {
        return *this;
    }

   private:
    const Text txt;
    const char divc = 0;
    const Text divt;
    Text::find_t f;
    int16_t idx = -1;
};

}  // namespace su