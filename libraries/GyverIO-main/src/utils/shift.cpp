#include "shift.h"

namespace gio {
namespace shift {

bool read(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay) {
    bool dif = 0;
    if (order & 0b10) data += len - 1;

#if defined(__AVR__) && !defined(GIO_NO_MASK)
    if (!__builtin_constant_p(dat_pin) || !__builtin_constant_p(clk_pin)) {
        volatile uint8_t* c_reg = portOutputRegister(digitalPinToPort(clk_pin));
        volatile uint8_t* d_reg = portInputRegister(digitalPinToPort(dat_pin));
        uint8_t c_mask = digitalPinToBitMask(clk_pin);
        uint8_t d_mask = digitalPinToBitMask(dat_pin);
        uint8_t val = 0;
        while (len--) {
            val = 0;
            for (uint8_t i = 0; i < 8; i++) {
                if (order & 0b01) {  // MSBFIRST
                    val <<= 1;
                    if (greg_read(d_reg, d_mask)) val |= 1;
                } else {
                    val >>= 1;
                    if (greg_read(d_reg, d_mask)) val |= (1 << 7);
                }
                greg_set(c_reg, c_mask);
                if (delay) delayMicroseconds(delay);
                greg_clr(c_reg, c_mask);
            }
            if (!dif && *data != val) dif = 1;
            *data = val;
            data += (order & 0b10) ? -1 : 1;
        }
        greg_clr(d_reg, d_mask);
    } else
#endif
    {
        uint8_t val = 0;
        while (len--) {
            val = 0;
            for (uint8_t i = 0; i < 8; i++) {
                if (order & 0b01) {  // MSBFIRST
                    val <<= 1;
                    if (gio::read(dat_pin)) val |= 1;
                } else {
                    val >>= 1;
                    if (gio::read(dat_pin)) val |= (1 << 7);
                }
                gio::high(clk_pin);
                if (delay) delayMicroseconds(delay);
                gio::low(clk_pin);
            }
            if (!dif && *data != val) dif = 1;
            *data = val;
            data += (order & 0b10) ? -1 : 1;
        }
    }
    return dif;
}

uint8_t read_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t delay) {
    uint8_t value;
    read(dat_pin, clk_pin, order, &value, 1, delay);
    return value;
}

bool read_cs(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay) {
    gio::low(cs_pin);
    bool res = read(dat_pin, clk_pin, order, data, len, delay);
    gio::high(cs_pin);
    return res;
}

uint8_t read_cs_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t delay) {
    uint8_t value;
    read_cs(dat_pin, clk_pin, cs_pin, order, &value, 1, delay);
    return value;
}

void send(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay) {
#if defined(__AVR__) && !defined(GIO_NO_MASK)
    if (!__builtin_constant_p(dat_pin) || !__builtin_constant_p(clk_pin)) {
        volatile uint8_t* c_reg = portOutputRegister(digitalPinToPort(clk_pin));
        volatile uint8_t* d_reg = portOutputRegister(digitalPinToPort(dat_pin));
        uint8_t c_mask = digitalPinToBitMask(clk_pin);
        uint8_t d_mask = digitalPinToBitMask(dat_pin);
        uint8_t val;
        for (uint16_t b = 0; b < len; b++) {
            val = data[(order & 0b10) ? (len - b - 1) : b];
            for (uint8_t i = 0; i < 8; i++) {
                if (order & 0b01) {  // MSBFIRST
                    greg_write(d_reg, d_mask, val & (1 << 7));
                    val <<= 1;
                } else {
                    greg_write(d_reg, d_mask, val & 1);
                    val >>= 1;
                }
                greg_set(c_reg, c_mask);
                if (delay) delayMicroseconds(delay);
                greg_clr(c_reg, c_mask);
            }
        }
        greg_clr(d_reg, d_mask);
    } else
#endif
    {
        for (uint16_t b = 0; b < len; b++) {
            uint8_t val = data[(order & 0b10) ? (len - b - 1) : b];
            for (uint8_t i = 0; i < 8; i++) {
                if (order & 0b01) {  // MSBFIRST
                    gio::write(dat_pin, val & (1 << 7));
                    val <<= 1;
                } else {
                    gio::write(dat_pin, val & 1);
                    val >>= 1;
                }
                gio::high(clk_pin);
                if (delay) delayMicroseconds(delay);
                gio::low(clk_pin);
            }
            gio::low(dat_pin);
        }
    }
}

void send_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t order, uint8_t data, uint8_t delay) {
    send(dat_pin, clk_pin, order, &data, 1, delay);
}

void send_cs(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t* data, uint16_t len, uint8_t delay) {
    gio::low(cs_pin);
    send(dat_pin, clk_pin, order, data, len, delay);
    gio::high(cs_pin);
}

void send_cs_byte(uint8_t dat_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t data, uint8_t delay) {
    send_cs(dat_pin, clk_pin, cs_pin, order, &data, 1, delay);
}

}  // namespace shift
}  // namespace gio