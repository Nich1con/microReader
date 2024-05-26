#pragma once
#include <Arduino.h>
#include <Print.h>

// строка, в которую можно печатать через print/println

namespace su {

class PrintString : public String, public Print {
   public:
    size_t write(uint8_t data) {
        concat((char)data);
        return 1;
    }
};

}  // namespace su