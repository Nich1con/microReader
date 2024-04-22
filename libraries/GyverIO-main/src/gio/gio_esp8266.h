#pragma once
#if defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)

#include <Arduino.h>

#include "gio_defs.h"

namespace gio {

// mode
_GIO_INLINE void mode(uint8_t pin, uint8_t mode) {
    // dsnt work
    // if (mode == INPUT) GPE &= ~(1 << pin);
    // else if (mode == OUTPUT) GPE |= (1 << pin);

    switch (mode) {
        case INPUT:
        case INPUT_PULLUP:
            if (pin < 16) {
                GPF(pin) = GPFFS(GPFFS_GPIO(pin));
                GPEC = (1 << pin);
                GPC(pin) = (GPC(pin) & (0xF << GPCI)) | (1 << GPCD);
                if (mode == INPUT_PULLUP) GPF(pin) |= (1 << GPFPU);
            } else if (pin == 16) {
                GPF16 = GP16FFS(GPFFS_GPIO(pin));
                GPC16 = 0;
                GP16E &= ~1;
            }
            break;

        case OUTPUT:
            if (pin < 16) {
                GPF(pin) = GPFFS(GPFFS_GPIO(pin));
                GPC(pin) = (GPC(pin) & (0xF << GPCI));
                GPES = (1 << pin);
            } else if (pin == 16) {
                GPF16 = GP16FFS(GPFFS_GPIO(pin));
                GPC16 = 0;
                GP16E |= 1;
            }
            break;

        default:
            pinMode(pin, mode);
            break;
    }
}

// read
_GIO_INLINE int read(uint8_t pin) {
    return (pin < 16) ? GPIP(pin) : (GP16I & 0x01);
}

// low
_GIO_INLINE void low(uint8_t pin) {
    if (pin < 16) GPOC = (1 << pin);
    else if (pin == 16) GP16O &= ~1;
}

// high
_GIO_INLINE void high(uint8_t pin) {
    if (pin < 16) GPOS = (1 << pin);
    else if (pin == 16) GP16O |= 1;
}

// write
_GIO_INLINE void write(uint8_t pin, uint8_t val) {
    if (val) high(pin);
    else low(pin);
}

// toggle
_GIO_INLINE void toggle(uint8_t pin) {
    if (pin < 16) {
        if (GPIP(pin)) GPOC = (1 << pin);
        else GPOS = (1 << pin);
    } else if (pin == 16) {
        if (GP16I & 0x01) GP16O &= ~1;
        else GP16O |= 1;
    }
}

// init
_GIO_INLINE void init(int P, int V = INPUT) {
    mode(P, V);
}

}  // namespace gio

#endif