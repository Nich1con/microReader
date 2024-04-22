#include <Arduino.h>

#include "GyverIO.h"

// gio::SSPI<false, 4000000> spi0;
// gio::SSPI<false, 4000000, 9> spi1;
// gio::SSPI<false, 4000000> spi2(9);
// gio::SSPI<true, 4000000, 9, 11, 13> spi3;
// gio::SSPI<true, 4000000> spi4(9, 11, 13);

#define PIN_DAT 7
#define PIN_CLK 6
byte datNC, clkNC;

void setup() {
    pinMode(PIN_DAT, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    // gio::shift::send_byte(PIN_DAT, PIN_CLK, MSBFIRST, 0b10100101);
}

void loop() {
    datNC = digitalRead(5);
    clkNC = digitalRead(5);
    datNC = PIN_DAT;
    clkNC = PIN_CLK;
    digitalWrite(PIN_DAT, 1);
    delayMicroseconds(50);
    digitalWrite(PIN_DAT, 0);
    delayMicroseconds(50);
    noInterrupts();

    shiftOut(datNC, clkNC, MSBFIRST, 0b10100101);
    shiftOut(PIN_DAT, PIN_CLK, MSBFIRST, 0b10100101);
    gio::shift::send_byte(datNC, clkNC, MSBFIRST, 0b10100101);
    gio::shift::send_byte(PIN_DAT, PIN_CLK, MSBFIRST, 0b10100101);
    delayMicroseconds(50);

    // ardu write nc
    digitalWrite(datNC, 1);
    digitalWrite(datNC, 0);
    digitalWrite(datNC, 1);
    digitalWrite(datNC, 0);
    digitalWrite(datNC, 1);
    digitalWrite(datNC, 0);
    digitalWrite(datNC, 1);
    digitalWrite(datNC, 0);
    digitalWrite(datNC, 1);
    digitalWrite(datNC, 0);
    delayMicroseconds(10);

    // ardu write
    digitalWrite(PIN_DAT, 1);
    digitalWrite(PIN_DAT, 0);
    digitalWrite(PIN_DAT, 1);
    digitalWrite(PIN_DAT, 0);
    digitalWrite(PIN_DAT, 1);
    digitalWrite(PIN_DAT, 0);
    digitalWrite(PIN_DAT, 1);
    digitalWrite(PIN_DAT, 0);
    digitalWrite(PIN_DAT, 1);
    digitalWrite(PIN_DAT, 0);
    delayMicroseconds(10);

    // ardu read nc
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    digitalRead(datNC);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    // ardu read
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    digitalRead(PIN_DAT);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    // ardu mode nc
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    pinMode(datNC, OUTPUT);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    // ardu mode
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    pinMode(PIN_DAT, OUTPUT);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(50);

    // gio write nc
    gio::write(datNC, 1);
    gio::write(datNC, 0);
    gio::write(datNC, 1);
    gio::write(datNC, 0);
    gio::write(datNC, 1);
    gio::write(datNC, 0);
    gio::write(datNC, 1);
    gio::write(datNC, 0);
    gio::write(datNC, 1);
    gio::write(datNC, 0);
    delayMicroseconds(10);

    // gio write
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    // gio read nc
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    gio::read(datNC);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    // gio read
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    gio::read(PIN_DAT);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    // gio mode nc
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    gio::mode(datNC, OUTPUT);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    // gio mode
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);

    gio::mode(PIN_DAT, OUTPUT);

    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    gio::write(PIN_DAT, 1);
    gio::write(PIN_DAT, 0);
    delayMicroseconds(10);

    interrupts();
}