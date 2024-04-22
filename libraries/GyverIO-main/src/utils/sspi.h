#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "shift.h"

namespace gio {

template <uint8_t _softT = 0, uint32_t _freqT = 4000000, int8_t _csT = -1, int8_t _datT = -1, int8_t _clkT = -1>
class SSPI {
   public:
    SSPI(int8_t cs = -1, int8_t dat = -1, int8_t clk = -1) {
        if (_softT) {
            if (_datT >= 0) {
                gio::init(_datT, OUTPUT);
                gio::init(_clkT, OUTPUT);
            } else {
                _pins[1] = dat;
                _pins[2] = clk;
                gio::init(_pins[1], OUTPUT);
                gio::init(_pins[2], OUTPUT);
            }
        }

        if (_csT >= 0) {
            gio::init(_csT, OUTPUT);
            gio::high(_csT);
        } else {
            _pins[0] = cs;
            if (cs >= 0) {
                gio::init(cs, OUTPUT);
                gio::high(cs);
            }
        }
    }
    void send(uint8_t data) {
        _begin();
        if (_softT) {
            if (_datT >= 0) gio::shift::send_byte(_datT, _clkT, MSBFIRST, data, 1000000ul / _freqT);
            else gio::shift::send_byte(_pins[1], _pins[2], MSBFIRST, data, 1000000ul / _freqT);
        } else {
            SPI.transfer(data);
        }
        _end();
    }
    void send(uint8_t* data, uint16_t len) {
        _begin();
        if (_softT) {
            if (_datT >= 0) gio::shift::send(_datT, _clkT, MSBFIRST, data, len, 1000000ul / _freqT);
            else gio::shift::send(_pins[1], _pins[2], MSBFIRST, data, len, 1000000ul / _freqT);
        } else {
            for (uint16_t i = 0; i < len; i++) SPI.transfer(data[i]);
        }
        _end();
    }

   private:
    void _begin() {
        if (_csT >= 0) gio::low(_csT);
        else if (_pins[0] >= 0) gio::low(_pins[0]);

        if (!_softT) SPI.beginTransaction(SPISettings(_freqT, MSBFIRST, SPI_MODE0));
    }
    void _end() {
        if (!_softT) SPI.endTransaction();

        if (_csT >= 0) gio::high(_csT);
        else if (_pins[0] >= 0) gio::high(_pins[0]);
    }

    int8_t _pins[(_csT < 0) + ((_softT && _datT < 0) ? 2 : 0)];  // CS, DAT, CLK

    /*
    SSPI<0, f, cs> spi;             0
    SSPI<0, f, -1> spi(?);          1
    SSPI<1, f, cs, dat, clk> spi;   0
    SSPI<1, f> spi(cs, dat, clk);   1+2
    */
};

}  // namespace gio