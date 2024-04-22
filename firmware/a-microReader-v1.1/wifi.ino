void enterToWifiMenu(void) {     // Переход в режим WIFI
  oled.clear();                  // Очистка
  oled.home();                   // Возврат на 0,0
  oled.line(0, 10, 127, 10);     // Линия
  oled.print(F("WI-FI МЕНЮ"));   // Выводим режим
  checkBatteryCharge();          // Проверка напряжение аккума
  drawBatteryCharge();           // Рисуем индикатор
  oled.update();                 // Выводим картинку

  if (sets.staModeEn) {         // Если нужно подключиться к роутеру
    oled.clear();               // Очистка
    oled.home();                // Возврат на 0,0
    oled.line(0, 10, 127, 10);  // Линия
    oled.print(F("WI-FI МЕНЮ"));   // Выводим режим
    oled.setCursor(0, 2);
    oled.print(F("Подключение"));  // Выводим надпись
    checkBatteryCharge();          // Проверка напряжение аккума
    drawBatteryCharge();           // Рисуем индикатор
    oled.update();                 // Выводим картинку

    WiFi.mode(WIFI_STA);                     // Включаем wifi
    WiFi.begin(sets.staSsid, sets.staPass);  // Подключаемся к сети
    oled.setCursor(66, 2);                   // Ставим курсор
    for (uint8_t i = 0; i < 10; i++) {       // Цикл на 10 секунд
      if (WiFi.status() != WL_CONNECTED) {   // Если к сети не подключились
        oled.print(".");                     // Рисуем точку
        oled.update();                       // Выводим картинку
        delay(1000);                         // Ждем секунду
      } else {                               // Как только подключились
        drawStaMenu();
        ui.attachBuild(build);               // Подключаем билд веб морды
        ui.attach(action);                   // Подключаем обработчик действий
        ui.start();                          // Стартуем!
        ui.enableOTA();                      // Включаем ОТА для прошивки по воздуху
        while (1) {                          // Бесконечный цикл
          ok.tick();                         // Опрос кнопки ОК
          up.tick();                         // Опрос кнопки UP
          down.tick();                       // Опрос кнопки DOWN

          if (up.click() || up.hold()) {  // Поднять яркость
            uiTimer = millis();           // Сброс таймера дисплея
            sets.dispContrast = constrain(sets.dispContrast + 10, 10, 100);
            byte con = map(sets.dispContrast, 10, 100, 1, 255);
            oled.setContrast(con);        // Тут же задаем яркость оледа
          }

          if (down.click() || down.hold()) {  // Понизить яркость
            uiTimer = millis();               // Сброс таймера дисплея
            sets.dispContrast = constrain(sets.dispContrast - 10, 10, 100);
            byte con = map(sets.dispContrast, 10, 100, 1, 255);
            oled.setContrast(con);            // Тут же задаем яркость оледа
          }

          if (ok.click() || ((millis() - uiTimer) >= WIFI_TIMEOUT_S * 1000)) {  // Если нажата кнопка или таймаут WiFi
            uiTimer = millis();                                                 // Сброс таймера дисплея
            checkFileSystem();                                                  // Чекаем файловую систему
            drawMainMenu();                                                     // Рисуем главное меню
            EEPROM.put(1, sets);                                                // Сохраняем все настройки в EEPROM
            EEPROM.commit();                                                    // Записываем
            WiFi.mode(WIFI_OFF);                                                // Вырубаем wifi
            return;                                                             // Валим из функции
          }

          if (millis() - batTimer >= 5000) {  // Таймер дисплея
            batTimer = millis();              // Сбрасываем таймер
            drawStaMenu();                    // Рисуем меню (только ради индикации заряда)
          }
          ui.tick();  // Тикер портала
          yield();    // Внутренний поллинг ESP
        }
      }
    }
  }

  WiFi.mode(WIFI_AP);                     // Если STA режим пропущен врубаем AP
  WiFi.softAP(sets.apSsid, sets.apPass);  // Создаем сеть
  drawApMenu(); 
  ui.attachBuild(build);                  // Подключаем билд веб морды
  ui.attach(action);                      // Подключаем обработчик действий
  ui.start();                             // Стартуем!
  ui.enableOTA();                         // Включаем ОТА для прошивки по воздуху
  while (1) {                             // Бесконечный цикл
    ok.tick();                            // Опрос кнопки ОК
    up.tick();                            // Опрос кнопки UP
    down.tick();                          // Опрос кнопки DOWN

    if (up.click() || up.hold()) {        // Поднять яркость
      uiTimer = millis();                 // Сброс таймера дисплея
      sets.dispContrast = constrain(sets.dispContrast + 10, 10, 100);
      uint8_t con = map(sets.dispContrast, 10, 100, 1, 255);
      oled.setContrast(con);              // Тут же задаем яркость оледа
    }

    if (down.click() || down.hold()) {    // Понизить яркость
      uiTimer = millis();                 // Сброс таймера дисплея
      sets.dispContrast = constrain(sets.dispContrast - 10, 10, 100);
      uint8_t con = map(sets.dispContrast, 10, 100, 1, 255);
      oled.setContrast(con);              // Тут же задаем яркость оледа
    }

    if (ok.click() || ((millis() - uiTimer) / 1000 >= WIFI_TIMEOUT_S)) {  // Если нажата кнопка или таймаут WiFi
      uiTimer = millis();                                                 // Сброс таймера дисплея
      checkFileSystem();                                                  // Чекаем файловую систему
      drawMainMenu();                                                     // Рисуем главное меню
      EEPROM.put(1, sets);                                                // Сохраняем все настройки в EEPROM
      EEPROM.commit();                                                    // Записываем
      WiFi.softAPdisconnect();                                            // Отключаем точку доступа
      WiFi.mode(WIFI_OFF);                                                // Вырубаем wifi
      return;                                                             // Валим из функции
    }
    if (millis() - batTimer >= 5000) {  // Таймер дисплея
      batTimer = millis();              // Сбрасываем таймер
      drawApMenu();                     // Рисуем меню (только ради индикации заряда)
    }
    ui.tick();  // Тикер портала
    yield();    // Внутренний поллинг ESP
  }
}