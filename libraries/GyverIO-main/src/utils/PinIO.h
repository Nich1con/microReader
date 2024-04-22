#pragma once

#include <Arduino.h>

#include "gio/gio.h"

// #define GIO_NO_MASK

namespace gio {

// single mode pin (output/input)
class PinIO {
   public:
    PinIO() {}
    PinIO(uint8_t npin, uint8_t mode = INPUT) {
        init(npin, mode);
    }

    void init(uint8_t npin, uint8_t mode = INPUT) {
        gio::init(npin, mode);
#if defined(__AVR__) && !defined(GIO_NO_MASK)
        if (mode == OUTPUT) reg = portOutputRegister(digitalPinToPort(npin));
        else reg = portInputRegister(digitalPinToPort(npin));
        mask = digitalPinToBitMask(npin);
#else
        pin = npin;
#endif
    }

    void write(uint8_t val) {
#if defined(__AVR__) && !defined(GIO_NO_MASK)
        greg_write(reg, mask, val);
#else
        gio::write(pin, val);
#endif
    }

    void high() {
#if defined(__AVR__) && !defined(GIO_NO_MASK)
        greg_set(reg, mask);
#else
        gio::high(pin);
#endif
    }

    void low() {
#if defined(__AVR__) && !defined(GIO_NO_MASK)
        greg_clr(reg, mask);
#else
        gio::low(pin);
#endif
    }

    void toggle() {
#if defined(__AVR__) && !defined(GIO_NO_MASK)
        *reg ^= mask;
#else
        gio::toggle(pin);
#endif
    }

    int read() {
#if defined(__AVR__) && !defined(GIO_NO_MASK)
        return greg_read(reg, mask);
#else
        return gio::read(pin);
#endif
    }

   private:
#if defined(__AVR__) && !defined(GIO_NO_MASK)
    volatile uint8_t *reg;
    uint8_t mask = 0xff;
#else
    uint8_t pin;
#endif
};

}  // namespace gio