// based on https://github.com/PaulStoffregen/OneWire/blob/master/util/OneWire_direct_gpio.h

#pragma once
#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <driver/rtc_io.h>

#include "gio_defs.h"

namespace gio {

// low idf
#if ESP_IDF_VERSION_MAJOR < 4
#define __esp32_rtc_idf3(pin)                                                                             \
    uint32_t rtc_reg(rtc_gpio_desc[pin].reg);                                                             \
    if (rtc_reg) {                                                                                        \
        ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].mux);                                  \
        ESP_REG(rtc_reg) = ESP_REG(rtc_reg) & ~(rtc_gpio_desc[pin].pullup | rtc_gpio_desc[pin].pulldown); \
    }
#else
#define __esp32_rtc_idf3(pin)
#endif

// mode
_GIO_INLINE void mode(uint8_t pin, uint8_t mode) {
    if (!digitalPinIsValid(pin)) return;
    switch (mode) {
        case INPUT:
#if CONFIG_IDF_TARGET_ESP32C3
            GPIO.enable_w1tc.val = (1ul << pin);
#else
            __esp32_rtc_idf3(pin);
            if (pin < 32) GPIO.enable_w1tc = (1ul << pin);
            else GPIO.enable1_w1tc.val = (1ul << (pin - 32));
#endif
            break;

        case OUTPUT:
#if CONFIG_IDF_TARGET_ESP32C3
            GPIO.enable_w1ts.val = (1ul << (pin));
#else
            if (pin <= 33) {
                __esp32_rtc_idf3(pin);
                if (pin < 32) GPIO.enable_w1ts = (1ul << pin);
                else GPIO.enable1_w1ts.val = (1ul << (pin - 32));
            }
#endif
            break;

        default:
            pinMode(pin, mode);
            break;
    }
}

// read
_GIO_INLINE int read(uint8_t pin) {
    if (!digitalPinIsValid(pin)) return 0;
#if CONFIG_IDF_TARGET_ESP32C3
    return (GPIO.in.val >> pin) & 0x1;
#else
    if (pin < 32) return (GPIO.in >> pin) & 0x1;
    else return (GPIO.in1.val >> (pin - 32)) & 0x1;
#endif
}

// low
_GIO_INLINE void low(uint8_t pin) {
    if (!digitalPinIsValid(pin)) return;
#if CONFIG_IDF_TARGET_ESP32C3
    GPIO.out_w1tc.val = (1ul << pin);
#else
    if (pin < 32) GPIO.out_w1tc = (1ul << pin);
    else GPIO.out1_w1tc.val = (1ul << (pin - 32));
#endif
}

// high
_GIO_INLINE void high(uint8_t pin) {
    if (!digitalPinIsValid(pin)) return;
#if CONFIG_IDF_TARGET_ESP32C3
    GPIO.out_w1ts.val = (1ul << pin);
#else
    if (pin < 32) GPIO.out_w1ts = (1ul << pin);
    else GPIO.out1_w1ts.val = (1ul << (pin - 32));
#endif
}

// write
_GIO_INLINE void write(uint8_t pin, uint8_t val) {
    if (val) high(pin);
    else low(pin);
}

// toggle
_GIO_INLINE void toggle(uint8_t pin) {
    write(pin, !read(pin));
}

// init
_GIO_INLINE void init(int P, int V = INPUT) {
    pinMode(P, V);
}

}  // namespace gio

#endif