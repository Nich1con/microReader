This is an automatic translation, may be incorrect in some places. See sources and examples!

# Gyverio
Fast functions for working with AVR Pin (a full list look at Gio_avr.h), ESP8266, ESP32
- acceleration on average by 20-30 times, the final time for all architectures is almost the same
- Classes for quick PIN control
- Separate processing of cases of constant and uncontrolled pins for AVR
- Fast implementation of Shiftin/Shiftout
- Universal class Hard Spi / Soft SPI for use in libraries

## Speed
### measurement conditions
||Version |Frequency |
| --------- | -------- | -------- |
|AVR |1.8.19 |16 |
|ESP8266 |3.1.2 |80 |
|ESP32 |2.0.11 |240 |
|ESP32C3 |2.0.11 |80 |

### GPIO (US)
||Write NC |Write |Read NC |Read |Mode NC |Mode |
| ------------- | ---------- | ----------------- | ------------------------------| ------------ | --------- | ------------
|AVR Ardu |** 5.3 ** |5.3 |** 4.8 ** |4.8 |** 3.3 ** |3.3 |
|AVR Gio |1.6 |*** 0.125 *** |1.75 |*** 0.075 *** |1.8 |*** 0.125 *** |
||||||||
|ESP8266 Ardu |** 1.5 ** |1.5 |** 0.54 ** |0.54 |** 1.4 ** |1.4 |
|ESP8266 Gio |0.29 |*** 0.08 *** |0.5 |*** 0.17 *** |1.29 |*** 0.58 *** |
||||||||
|ESP32 Ardu |** 0.33 ** |0.33 |** 0.124 ** |0.124 |** 16 ** |16 |
|ESP32 Gio |0.04 |*** 0.04 *** |0.085 |*** 0.085 *** |0.126 |*** 0.08 *** |
||||||||
|ESP32C3 Ardu |** 0.91 ** |0.91 |** 0.25 ** |0.25 |** 21 ** |21 |
|ESP32C3 Gio |0.05 |*** 0.05 *** |0.4 |*** 0.08 *** |0.49 |*** 0.08 *** |

> * Nc * - Pins are not constants

> ** Fat ** highlighted the worst time (Arduino not constants), *** with a fat italics *** - the best (gio constants)

### Shift (MHZ)
||Shiftout |Gio :: Shift |
| -------- | ---------- | -----------------------------------
|AVR NC |0.06 |0.66 |
|AVR |0.06 |1.3 |
|ESP8266 |0.2 |1.1 |
|ESP32 |0.96 |6 |
|ESP32C3 |0.35 |2.6 |

> * Nc * - Pins are not constants

## compatibility
Compatible with all arduino platforms (used arduino functions)
- For ESP8266 and ESP32, fast `pinmode ()` (`mode ()`) works only on `Input`/` output`!In other modes, the standard `pinmode ()` is called

## Content
- [documentation] (#docs)
- [use] (#usage)
- [versions] (#varsions)
- [installation] (# Install)
- [bugs and feedback] (#fedback)

<a id="docs"> </a>
## Documentation
## Gio
Squeezing pins

`` `CPP
int Gio :: read (int p);
Void Gio :: High (int p);
Void Gio :: Low (int p)
Void Gio :: Write (int p, int v);

//
Void Gio :: Toggle (int p);

// Pin mode.For ESP8266/ESP32 only Input/Output!
Void Gio :: Mode (int p, int v);

// you need to call for ESP8266/ESP32 when initializing the PIN
// Otherwise, mod () will not work!
Void Gio :: Init (int p);
`` `

### Gio :: Shift
Fast analogue Shiftin/Shiftout (sending data with a clock)

`` `CPP
// Read the package
vOID GIO :: Shift :: Read (uint8_t data_pin, uint8_t clk_pin, uint8_t Order, uint8_t* Data, Uint16_t Len, Uint8_t Delay = 0);

// Read the byte
uint8_t Gio :: Shift :: read_byte (uint8_t dat_pin, uint8_t clak_pin, uint8_t Order, uint8_t demely = 0);

// Read the package + cs pin
VOID GIO :: Shift :: Read_cs (uint8_t data_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t Order, uint8_t* data, uint16_t Len, uint8_t Delay = 0);

// read byte + cs pin
Uint8_t Gio :: Shift :: Read_cs_byte (uint8_t data_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t order, uint8_t dlavay = 0);

// Send a package
VOID Gio :: Shift :: Send (uint8_t data_pin, uint8_t clk_pin, uint8_t Order, uint8_t* data, uint16_t len, uint8_t dlavay = 0);

// Send byte
VOID Gio :: Shift :: Send_byte (uint8_t data_pin, uint8_t clk_pin, uint8_t order, uint8_t data, uint8_t demelavy = 0);

// Send package + cs pin
VOID Gio :: Shift :: Send_cs (uint8_t data_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t Order, uint8_t* data, uint16_t Len, uint8_t demelavy = 0);

// Send byte + cs pin
VOID GIO :: Shift :: Send_cs_byte (uint8_t data_pin, uint8_t clk_pin, uint8_t cs_pin, uint8_t ORDER, UINT8_T DATA, UINT8_T DELAY = 0);
`` `

The `order` parameter can be:
- `lsbfirst`/` lsb_normal` - lsb, direct bytes directly
- `MSBFIRST`/` MSB_Normal` - MSB, direct bytes direct
- `lsb_reverse` - lsb, reverse byte order
- `MSB_Reverse` - MSB, reverse bytes

#### Note
- `Delay` in microseconds, allows you to reduce the speed of transmission.For example, `1` μs will limit speed up to ~ 1 MHz, 2 μs up to ~ 500 kHz
- Pins should be configured as `output` yourself before sending (when starting a program for example)

### Gio :: SSPI
Universal class of software and hardware SPI with optimization of the number of variables for Pin

`` `CPP
SSPI <0, Freq> Spi;// hardware without PIN CS
SSPI <0, Freq, CS> SPI;// Hardware with PIN CS in the template
SSPI <0, Freq> Spi (CS);// Hardware with PIN CS in the classroom
SSPI <1, Freq, CS, DT, CLK> Spi;// Program with pines in the template
SSPI <1, FreQ> Spi (CS, DT, CLK);// Program with pins in class
`` `

### compilation settings
`` `CPP
#define gio_use_arduino // Disable quick functions (replace with standard)
#define gio_no_mask // Disable a quick IO based on AVR mask (in the Pinio class and all Shift)
`` `

<a id="usage"> </a>
## Usage

`` `CPP
Gio :: Write (3, 1);// Turn on PIN 3

// Send data on pins 3 and 4
uint8_t data [] = {34, 63, 231, 9};
Gio :: Shift :: Send (3, 4, MSBFIRST, DATA, 4);
`` `

<a id="versions"> </a>
## versions
- V1.0
- V1.1 - AVR Non -Const is 3 times accelerated, the tables are updated
- V1.2 - Fixed a mistake!
- V1.2.1 - Small optimization
- v1.2.2 - added inversion to Shift

<a id="install"> </a>
## Installation
- The library can be found by the name ** gyverio ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/gyverio/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes files are deleted in new versions,Cranberries that remain when replacing and can lead to errors!

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code