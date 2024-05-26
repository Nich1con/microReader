#define SERV_MENU_LINES 3
uint8_t servCursor = 0;

void enterToServiceMode(void) {
  while (!oled.init(IIC_SDA_PIN, IIC_SCL_PIN)) {  // Инициализация оледа
    digitalWrite(LED_BUILTIN, LOW);               // Медленно мигаем пока не проинициализируем олед
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }

  oled.clear();       // Очистка оледа
  oled.update();      // Вывод пустой картинки

  digitalWrite(LED_BUILTIN, LOW); // Зажигаем лед

  //EEPROM.begin(100);              // Инициализация EEPROM
  while (!LittleFS.begin()) {     // Инициализация файловой системы
    LittleFS.format();
  }

  drawServiceMenu();

  while (1) {
    up.tick();
    ok.tick();
    down.tick();
    data.tick();

    if (up.click()) {
      servCursor = constrain(servCursor - 1, 0, SERV_MENU_LINES - 1);
      drawServiceMenu();
    } else if (down.click()) {
      servCursor = constrain(servCursor + 1, 0, SERV_MENU_LINES - 1);
      drawServiceMenu();
    }

    if (ok.click()) {
      switch (servCursor) {
        case 0:           // Сброс настроек
          //EEPROM.put(1, sets);
          //EEPROM.commit();
          data.write();
          drawResetNotify();
          drawServiceMenu();
          break;

        case 1:            // Форматирование
          LittleFS.format();
          drawFormatNotify();
          drawServiceMenu();
          break;

        case 2:            // Выход
          LittleFS.end();  // Деинит FS (на всякий случай)
          //EEPROM.end();  // Деинит EE (на всякий случай)
          return;
      }
    }

    yield();
  }
}

void drawResetNotify(void) {
  oled.clear();
  oled.setScale(2);
  oled.setCursorXY(10, 16);
  oled.print(F("настройки"));
  oled.setCursorXY(16, 32);
  oled.print(F("сброшены"));
  oled.setScale(1);
  oled.update();
  delay(1500);
}

void drawFormatNotify(void) {
  oled.clear();
  oled.setScale(2);
  oled.setCursorXY(34, 16);
  oled.print(F("файлы"));
  oled.setCursorXY(22, 32);
  oled.print(F("удалены"));
  oled.setScale(1);
  oled.update();
  delay(1500);
}

void drawServiceMenu(void) {
  oled.clear();               // Чистим дисплей
  oled.line(0, 10, 127, 10);  // Линия
  oled.print(F("FW.V1.2"));   // Версия прошивки
  oled.setCursor(0, 2);       // Выводим с 2й строки

  oled.print(F(               // Выводим пункты
    " СБРОС НАСТРОЕК\r\n"
    " ФОРМАТИРОВАНИЕ\r\n"
    " ВЫХОД\r\n"));

  oled.setCursor(0, servCursor + 2);  // Указывем строку для курсора
  oled.print(">");                    // Выводим курсор
  checkBatteryCharge();               // Проверка напряжение аккума
  drawBatteryCharge();                // Рисуем индикатор
  oled.update();                      // Апдейтим дисплей
}
