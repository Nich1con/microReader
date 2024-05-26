#define GAME_MENU_LINES 5
uint8_t gCursor = 0;

void enterToGameMode(void) {
  gCursor = 0;
  drawGameMenu();

  while (1) {
    up.tick();
    ok.tick();
    down.tick();
    data.tick();

    if (up.click()) {
      gCursor = constrain(gCursor - 1, 0, GAME_MENU_LINES - 1);
      drawGameMenu();
    } else if (down.click()) {
      gCursor = constrain(gCursor + 1, 0, GAME_MENU_LINES - 1);
      drawGameMenu();
    }

    if (ok.click()) {
      uiTimer = millis();
      switch (gCursor) {
        case 0:           // Динозаврик
          dinosaurGame();
          break;

        case 1:            // Тетрис
          tetrisGame();
          break;

        case 2:            // Змейка
          snakeGame();
          break;

        case 3:            // Калькулятор
          calcul();
          break;

        case 4:            // Выход
          drawMainMenu();
          return;
      }
    }
    yield();
  }
}

void drawGameMenu(void) {
  oled.clear();               // Чистим дисплей
  oled.line(0, 10, 127, 10);  // Линия
  oled.print(F("GAME MODE"));   // Версия прошивки
  oled.setCursor(0, 2);       // Выводим с 2й строки
  
  oled.print(F(               // Выводим пункты
    "  ДИНОЗАВРИК\r\n"                                                    // ВНИМАНИЕ!   Тут какой-то глюк, с одним пробелом не работает
    "  ТЕТРИС\r\n"
    "  ЗМЕЙКА\r\n"
    "  КАЛЬКУЛЯТОР\r\n"
    "  ВЫХОД\r\n"));

  oled.setCursor(0, gCursor + 2);  // Указывем строку для курсора
  oled.print(">");                    // Выводим курсор
  checkBatteryCharge();               // Проверка напряжение аккума
  drawBatteryCharge();                // Рисуем индикатор
  oled.update();                      // Апдейтим дисплей
}
