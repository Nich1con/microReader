#pragma once

#include <Arduino.h>

#include "gio/gio.h"

#define LSB_NORMAL 0b00
#define MSB_NORMAL 0b01
#define LSB_REVERSE 0b10
#define MSB_REVERSE 0b11

namespace gio {
namespace shift {

// read
// прочитать пакет. Вернёт true, если хотя бы один бит отличается
bool read(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay = 0);

// прочитать байт
uint8_t read_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t delay = 0);

// прочитать пакет + cs пин. Вернёт true, если хотя бы один бит отличается
bool read_cs(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay = 0);

// прочитать байт + cs пин
uint8_t read_cs_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t delay = 0);

// send
// отправить пакет
void send(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay = 0);

// отправить байт
void send_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t data, uint8_t delay = 0);

// отправить пакет + cs пин
void send_cs(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay = 0);

// отправить байт + cs пин
void send_cs_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t data, uint8_t delay = 0);

}  // namespace shift
}  // namespace gio

/*
us byte (MHz)
|         | shiftOut     | gio_send | gio_send (non-const) |
|---------|--------------|----------|----------------------|
| AVR     | 100 (0.075)  | 6 (1.3)  | 11 (0.7)             |
*/