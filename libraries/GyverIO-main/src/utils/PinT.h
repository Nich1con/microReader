#pragma once

#include <Arduino.h>

#include "gio/gio.h"
#include "gio/gio_defs.h"

namespace gio {

template <uint8_t pin>
class PinT {
   public:
    PinT(uint8_t mode = INPUT) {
        gio::init(pin, mode);
    }
    _GIO_INLINE void mode(uint8_t mode) {
        gio::mode(pin, mode);
    }
    _GIO_INLINE void write(uint8_t val) {
        gio::write(pin, val);
    }
    _GIO_INLINE void high() {
        gio::high(pin);
    }
    _GIO_INLINE void low() {
        gio::low(pin);
    }
    _GIO_INLINE void toggle() {
        gio::toggle(pin);
    }
    _GIO_INLINE int read() {
        return gio::read(pin);
    }
};

}  // namespace gio