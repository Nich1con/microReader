/* ======================================================================= */
/* ========================= Индикатор заряда ============================ */
void checkBatteryCharge(void) {       // Проверка заряда аккумулятора
  if (millis() - batTimer >= 5000) {  // Таймер батарейки на 5 сек
    batTimer = millis();              // Сброс таймера
    batMv = ESP.getVcc();             // Измерение напряжения питания
  }
}

void drawBatteryCharge(void) {                       // Рисуем батарейку
  byte charge =                                      // Заряд в виде числа
    constrain(                                       // Ограничиваем диапазон
      map(                                           // Преобразуем диапазон
        batMv, VBAT_EMPTY_MV, VBAT_FULL_MV,          // Напряжение в заряд (костыль)
        0, 12                                        // Преобразуем в [0...12]
        ),                                           // Конец map
      0, 12                                          // Ограничиваем от 0 до 12
    );                                               // Конец constrain
  oled.setCursorXY(110, 0);                          // Положение на экране
  oled.drawByte(0b00111100);                         // Пипка
  oled.drawByte(0b00111100);                         // 2 штуки
  oled.drawByte(0b11111111);                         // Передняя стенка
  for (uint8_t i = 0; i < 12; i++) {                 // 12 градаций
    if (i < 12 - charge) oled.drawByte(0b10000001);  // Рисуем пустые
    else oled.drawByte(0b11111111);                  // Рисуем полные
  }
  oled.drawByte(0b11111111);  // Задняя стенка
}
/* ======================================================================= */
/* ============================ Главное меню ============================= */
void drawMainMenu(void) {     // Отрисовка главного меню
  oled.clear();               // Очистка
  oled.home();                // Возврат на 0,0
  oled.line(0, 10, 127, 10);  // Линия
  oled.print(F("ФАЙЛОВ:"));
  oled.print(fileCount);      // Выводим кол-во файлов + битые при наличии
  if (badCount) oled.printf("[%i]", fileCount + badCount);

  int sidx = (cursor < 6 ? 0 : cursor - 5);  // Начальный индекс
  int i = 2;                                 // Строка на дисплее
  su::TextParser p(fileNames, '/');          // Парсер
  while (p.parse()) {                        // Циклически парсим строку имен
    if (p.index() > sidx) {                  // Пока не дошли до начальной позиции
      oled.setCursor(6, i++);                // Ставим курсор
      oled.print(p);                         // Выводим имя
    }
    if (p.index() == cursor + 1) {       // Если курсор указывает на имя
      selectedFile = (String)p;          // Запоминаем имя
    }
    if (p.index() > sidx + 5) break;     // Как только распарсили 6 строк - выходим
    yield();                             // Внутренний поллинг ESP
  }

  int cp = constrain(cursor, 0, 5) + 2;  // Ограничиваем позицию курсора
  oled.setCursor(0, cp);                 // Указывем строку для курсора
  oled.print(">");                       // Выводим курсор
  checkBatteryCharge();                  // Проверка напряжение аккума
  drawBatteryCharge();                   // Рисуем индикатор
  oled.update();                         // Выводим картинку
}
/* ======================================================================= */
/* ============================= Меню Wi-FI ============================== */
void drawStaMenu(void) {      // Рисуем STA меню
  oled.clear();               // Очистка
  oled.home();                // Возврат на 0,0
  oled.line(0, 10, 127, 10);  // Линия
  oled.print(F("РЕЖИМ STA")); // Выводим режим
  oled.setCursor(0, 2);
  oled.print(F("Сеть: "));
  oled.print(sets.staSsid);   // Выводим имя сети
  oled.setCursor(0, 4);
  oled.print(F("Локал.IP:"));
  oled.print(WiFi.localIP());  // Выводим IP
  checkBatteryCharge();        // Проверка напряжение аккума
  drawBatteryCharge();         // Рисуем индикатор
  oled.update();               // Выводим картинку
}

void drawApMenu(void) {       // Рисуем AP меню
  oled.clear();               // Очистка
  oled.home();                // Возврат на 0,0
  oled.line(0, 10, 127, 10);  // Линия
  oled.print(F("РЕЖИМ AP"));  // Выводим режим
  oled.setCursor(0, 2);
  oled.print(F("Сеть: "));
  oled.print(sets.apSsid);    // Выводим имя сети
  oled.setCursor(0, 4);
  oled.print(F("Ключ: "));
  oled.print(sets.apPass);    // Выводим пароль
  oled.setCursor(0, 6);
  oled.print(F("Локал.IP:"));
  oled.print(F("192.168.4.1"));   // Выводим IP
  checkBatteryCharge();           // Проверка напряжение аккума
  drawBatteryCharge();            // Рисуем индикатор
  oled.update();                  // Выводим картинку
}

void fileReadError(void) {
  oled.clear();
  oled.setScale(2);
  oled.setCursorXY(28, 8);
  oled.print(F("ошибка"));
  oled.setCursorXY(28, 24);
  oled.print(F("чтения"));
  oled.setCursorXY(34, 40);
  oled.print(F("файла"));
  oled.setScale(1);
  oled.update();
  delay(1500);
}