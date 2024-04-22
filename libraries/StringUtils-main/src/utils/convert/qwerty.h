#pragma once
#include <Arduino.h>

namespace su {

// Изменить раскладку (RU в QWERTY) - String
String toQwerty(const String& ru);

// Изменить раскладку (RU в QWERTY) - char* (qw длина как ru + 1, функция добавит '\0')
char* toQwerty(const char* ru, char* qw);

}