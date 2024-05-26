#include "qwerty.h"

namespace su {

static const char _qwerty_ru[] PROGMEM = "F<DULT:PBQRKVYJGHCNEA{WXIO}SM\">Zf,dult;pbqrkvyjghcnea[wxio]sm'.z~`";

String toQwerty(const String& ru) {
    String qw;
    uint8_t prev = 0;
    for (uint16_t i = 0; i < ru.length(); i++) {
        uint8_t cur = ru[i];
        if (cur > 127) {
            uint8_t thiscur = cur;
            if (cur > 191) cur = 0;
            else if (prev == 209 && cur == 145) cur = 193;  // ё
            else if (prev == 208 && cur == 129) cur = 192;  // Ё
            prev = thiscur;
        }
        if (!cur) continue;

        if (cur <= 127) {
            qw += (char)cur;
            continue;
        } else if (cur <= 143) cur -= 80;
        else if (cur <= 191) cur -= 144;
        else cur -= 128;
        qw += (char)pgm_read_byte(&_qwerty_ru[cur]);
    }
    return qw;
}

char* toQwerty(const char* ru, char* qw) {
    uint16_t len = strlen(ru);
    uint8_t prev = 0;
    for (uint16_t i = 0; i < len; i++) {
        uint8_t cur = ru[i];
        if (cur > 127) {
            uint8_t thiscur = cur;
            if (cur > 191) cur = 0;
            else if (prev == 209 && cur == 145) cur = 193;  // ё
            else if (prev == 208 && cur == 129) cur = 192;  // Ё
            prev = thiscur;
        }
        if (!cur) continue;

        if (cur <= 127) {
            *qw++ = (char)cur;
            continue;
        } else if (cur <= 143) cur -= 80;
        else if (cur <= 191) cur -= 144;
        else cur -= 128;
        *qw++ = (char)pgm_read_byte(&_qwerty_ru[cur]);
    }
    *qw = 0;
    return qw;
}

}  // namespace su