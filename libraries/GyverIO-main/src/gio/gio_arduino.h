#pragma once
#include <Arduino.h>

#include "gio_defs.h"

namespace gio {

// mode
_GIO_INLINE void mode(int P, int V) {
    pinMode(P, V);
}

// read
_GIO_INLINE int read(int P) {
    return digitalRead(P);
}

// high
_GIO_INLINE void high(int P) {
    digitalWrite(P, 1);
}

// low
_GIO_INLINE void low(int P) {
    digitalWrite(P, 0);
}

// write
_GIO_INLINE void write(int P, int V) {
    digitalWrite(P, V);
}

// toggle
_GIO_INLINE void toggle(int P) {
    digitalWrite(P, !digitalRead(P));
}

// init
_GIO_INLINE void init(int P, int V = INPUT) {
    mode(P, V);
}

}