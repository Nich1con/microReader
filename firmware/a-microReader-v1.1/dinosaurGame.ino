/*
  Игра "Google динозавр"
  В этом файле находятся:
  1. Битмапы (картинки), используемые в игре
  2. Функция меню игры
  3. Функция реализации игры
*/

#define DINO_EE_ADDR 90

/* ---------------------------------------- 1. Битмапы ----------------------------------------*/
const uint8_t DinoStandL_bmp[] PROGMEM = {   // 16x16 - Динозавр стоит на левой ноге
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xFE, 0xFF, 0xFD, 0xBF, 0xAF, 0x2F, 0x2F, 0x0E,
  0x03, 0x07, 0x1E, 0x1E, 0xFF, 0xBF, 0x1F, 0x3F, 0x7F, 0x4F, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00,
};

const uint8_t DinoStandR_bmp[] PROGMEM = {   // 16x16 - Динозавр стоит на правой ноге
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xFE, 0xFF, 0xFD, 0xBF, 0xAF, 0x2F, 0x2F, 0x0E,
  0x03, 0x07, 0x1E, 0x1E, 0x7F, 0x5F, 0x1F, 0x3F, 0xFF, 0x8F, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00,
};

const uint8_t DinoStandDie_bmp[] PROGMEM = {  // 16x16 - Динозавр стоит, раскрыв глаза (поражение)
  0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xFE, 0xF1, 0xF5, 0xB1, 0xBF, 0x2F, 0x2F, 0x0E,
  0x03, 0x07, 0x1E, 0x1E, 0xFF, 0xBF, 0x1F, 0x3F, 0xFF, 0x8F, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00,
};

const uint8_t DinoCroachL_bmp[] PROGMEM = {   // 16x8 - Динозавр пригнувшись на левой ноге
  0x03, 0x06, 0x6C, 0x5C, 0x1C, 0xFE, 0xBE, 0x1E, 0x7E, 0x5E, 0x0E, 0x1C, 0x3E, 0x2A, 0x2E, 0x0E,
};

const uint8_t DinoCroachR_bmp[] PROGMEM = {   // 16x8 - Динозавр пригнувшись на правой ноге
  0x03, 0x06, 0xEC, 0x9C, 0x1C, 0x7E, 0x5E, 0x1E, 0x7E, 0x5E, 0x0E, 0x1C, 0x3E, 0x2A, 0x2E, 0x0E,
};

const uint8_t CactusSmall_bmp[] PROGMEM = {  // 16x16 - Маленький кактус
  0x00, 0x00, 0x00, 0xE0, 0xC0, 0x00, 0xF8, 0xFC, 0xFC, 0xF8, 0x80, 0xFC, 0xFE, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0x07, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
};

const uint8_t CactusBig_bmp[] PROGMEM = {  // 24x16 - Большой кактус
  0xF0, 0x00, 0xFC, 0xFE, 0xFE, 0xC0, 0x7C, 0x00, 0xF0, 0x00, 0xF8, 0xFC, 0x60, 0x3E, 0x00, 0x80, 0x00, 0xF8, 0x80, 0xF8, 0xFC, 0xF8, 0x30, 0x1F,
  0x03, 0x07, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x03, 0xFF, 0xFF, 0x00, 0x1F, 0x30, 0xFF, 0x60, 0x3C, 0x01, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
};

const uint8_t BirdL_bmp[] PROGMEM = {    // 24x16 - Птица с крыльями вверх
  0x30, 0x38, 0x3C, 0x3C, 0x3F, 0x3F, 0x7F, 0x7C, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x7F, 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x00,
};

const uint8_t BirdR_bmp[] PROGMEM = {   // 24x16 - Птица с крыльями вниз
  0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF0, 0xF0, 0xC0, 0x0F, 0xFE, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x0E, 0x1F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC, 0xF8, 0xF8, 0x78, 0x68, 0x68, 0x68,
};

void dinosaurGame(void) {                                                           // Главное меню игры
  while (true) {                                                                    // Бесконечный цикл
    uint16_t bestScore = 0;                                                         // Лучший счет
    EEPROM.get(DINO_EE_ADDR, bestScore);                                            // Берем его из EEPROM
    oled.clear();                                                                   // Очистка дисплея
    oled.roundRect(0, 9, 127, 46, OLED_STROKE);                                     // Отрисовка интерфейса
    oled.setCursor(3, 0); oled.print(F("GOOGLE DINOSAUR GAME"));                    // Отрисовка интерфейса
    oled.setCursor(18, 6); oled.print(F("Лучший счет:")); oled.print(bestScore);    // Вывод рекорда
    oled.setCursor(0, 7); oled.print(F("<- Вверх"));                                // Вывод доп. инфы
    oled.setCursor(96, 7); oled.print(F("Ок ->"));                                  // Вывод доп. инфы
    oled.drawBitmap(10, 30, DinoStandL_bmp, 16, 16);                                // Вывод картинок
    oled.drawBitmap(46, 30, CactusBig_bmp, 24, 16);                                 // Вывод картинок
    oled.drawBitmap(94, 20, BirdL_bmp, 24, 16);                                     // Вывод картинок
    oled.update();                                                                  // Вывод на дисплей
    Wire.setClock(1E6);
    system_update_cpu_freq(160);
    while (true) {                                                                  // Вложенный бесконечный цикл
      up.tick();
      ok.tick();

      if(up.click() || millis() - uiTimer >= 10000){
        Wire.setClock(600E3);
        system_update_cpu_freq(80);
        drawMainMenu(); 
        return;
      }

      if (ok.click()) {                                                         // Нажатие на правую - начать играть
        playDinosaurGame();                                                     // Запускаем игру
        break;                                                                  // При выходе из игры переходим к отрисовке
      }

      yield();
    }
  }
}

/* ---------------------------------------- 2. Реализация игры ----------------------------------------*/
#define EFV           -5        // Упрощающий сдвиг при просчетах столкновений по вертикали (меньше 0 -> легче)
#define EFH           -5        // Упрощающий сдвиг по горизонтали (аналогичен EFV)
#define DINO_GROUND_Y 47        // Позиция динозавра по вертикали
#define DINO_GRAVITY  0.170f    // Значение гравитации динозавра
#define DINO_GAME_FPS 30        // Скорость обновления дисплея

void playDinosaurGame(void) {
  down.setHoldTimeout(160);         // Настраиваем удобные таймауты удержания
  ok.setHoldTimeout(160);
  ok.setStepTimeout(160);

startDinoGame:                         // Начало игры
  uint8_t gameSpeed = 10;              // Скорость игры
  uint16_t score = 0;                  // Текущий счет
  uint16_t bestScore = 0;              // Рекорд
  int8_t oldEnemyPos = 128;            // Позиция старого противника (тот, что уже заходит за горизонт)
  int8_t oldEnemyType = 0;             // Тип старого противника (тот, что уже заходит за горизонт)
  int8_t newEnemyPos = 128;            // Позиция нового противника (тот, что только выходит изза горизонта)
  int8_t newEnemyType = random(0, 3);  // Тип нового противника - определяем случайно
  bool dinoStand = true;               // Динозавр стоит на земле
  bool legFlag = true;                 // Флаг переключения ног динозавра
  bool birdFlag = true;                // Флаг взмахов птицы
  int8_t dinoY = DINO_GROUND_Y;        // Позиция динозавра по вертикали (изначально на земле)
  float dinoU = 0.0;                   // Скорость динозавра (вектор направлен вниз)

  EEPROM.get(DINO_EE_ADDR, bestScore); // Читаем рекорд из EEPROM

  while (1) {                                                   // Бесконечный цикл игры
    yield();
    up.tick();
    ok.tick();
    down.tick();

    if (up.click()) return;                                                  // Клик кнопки влево мгновенно возвращает нас в игровое меню

    /* ------------------ User input ------------------ */
    if (ok.click() and dinoY == DINO_GROUND_Y) {                             // Клик по ОК и динозавр стоит на земле (слабый прыжок)
      dinoU = -2.8;                                                          // Прибавляем скорости по направлению вверх
      dinoY -= 4;                                                            // Подкидываем немного вверх
    } else if ((ok.holding() or ok.step()) and dinoY == DINO_GROUND_Y) {     // Удержание ОК и динозавр стоит на земле (сильный прыжок)
      dinoU = -3.4;                                                          // Прибавляем скорости по направлению вверх
      dinoY -= 4;                                                            // Подкидываем немного вверх
    } else if (down.press()) {                                               // Нажатие ВНИЗ
      dinoU = 3.2;                                                           // Прибавляем скорости по направлению к земле
      if (dinoY >= DINO_GROUND_Y) {                                          // Если динозавр коснулся земли
        dinoY = DINO_GROUND_Y;                                               // Ставим его на землю
        dinoU = 0.0;                                                         // Обнуляем скорость
      }
    }

    if (down.holding() and dinoY >= DINO_GROUND_Y) {                         // Удержание ВНИЗ и дино стоит на земле
      dinoStand = false;                                                     // Переходим в присяд
    } else {
      dinoStand = true;                                                      // Иначе встаем обратно
    }

    /* ------------------ Game processing ------------------ */
    static uint32_t scoreTimer = millis();                                   // Таймер подсчета очков
    if (millis() - scoreTimer >= 100) {
      scoreTimer = millis();
      score++;                                                               // Увеличиваем счет
      gameSpeed = constrain(map(score, 1000, 0, 4, 10), 4, 10);              // Увеличиваем скорость игры! (10 - медленно, 4 - очень быстро)
    }

    static uint32_t enemyTimer = millis();                                   // Таймер кинематики противников
    if (millis() - enemyTimer >= gameSpeed) {                                // Его период уменьшается с ростом счета
      enemyTimer = millis();
      if (--newEnemyPos < 16) {                                              // Как только НОВЫЙ противник приближается к динозавру
        oldEnemyPos = newEnemyPos;                                           // Новый противник становится старым
        oldEnemyType = newEnemyType;                                         // И копирует тип нового к себе
        newEnemyPos = 128;                                                   // Между тем новый противник выходит изза горизонта
        do newEnemyType = random(0, 3);                                      // Получаем нового случайного противника     
        while(newEnemyType == oldEnemyType);                                 // Но не позволяем спаунить одинаковых подряд         
      }
      if (oldEnemyPos >= -24) {                                              // Двигаем старый пока он полностью не скроется за горизонтом
        oldEnemyPos--;                                                       // Двигаем старый
      }
    }

    static uint32_t legTimer = millis();                                     // Таймер анимации ног динозавра
    if (millis() - legTimer >= 130) {
      legTimer = millis();
      legFlag = !legFlag;                                                    // Он просто переключает флаг
    }

    static uint32_t birdTimer = millis();                                    // Таймер анимации взмахов птицы
    if (millis() - birdTimer >= 200) {
      birdTimer = millis();
      birdFlag = !birdFlag;                                                  // Он тоже просто переключает флаг!
    }

    static uint32_t dinoTimer = millis();                                    // Таймер кинематики динозавра
    if (millis() - dinoTimer >= 15) {                                        // С периодом DT
      dinoTimer = millis();
      dinoU += (float)DINO_GRAVITY;                                          // Увеличиваем скорость
      dinoY += (float)dinoU;                                                 // И соответственно координату (динозавр падает)
      if (dinoY >= DINO_GROUND_Y) {                                          // При касании с землей
        dinoY = DINO_GROUND_Y;                                               // Ставим динозвра на землю
        dinoU = 0.0;                                                         // Тормозим его до нуля
      }
    }

    /* ------------------ Drawing ------------------ */
    static uint32_t oledTimer = millis();                                    // Таймер отрисовки игры!
    if (millis() - oledTimer >= (1000 / DINO_GAME_FPS)) {                    // Привязан к FPS игры
      oledTimer = millis();

      oled.clear();                                                                                     // Чистим дисплей
      checkBatteryCharge();                                                                             // Проверка напряжение аккума
      drawBatteryCharge();                                                                              // Рисуем индикатор
      oled.setCursor(0, 0); oled.print("HI");                                                           // Выводим рекорд
      oled.setCursor(13, 0); oled.print(bestScore); oled.print(":"); oled.print(score);                 // Рекорд:текущий счет
      oled.line(0, 63, 127, 63);                                                                        // Рисуем поверхность земли (линия)

      switch (oldEnemyType) {                                                                           // Выбираем старого противника
        case 0: oled.drawBitmap(oldEnemyPos, 48, CactusSmall_bmp, 16, 16);                   break;     // Рисуем маленький кактус
        case 1: oled.drawBitmap(oldEnemyPos, 48, CactusBig_bmp, 24, 16);                     break;     // Рисуем большой кактус
        case 2: oled.drawBitmap(oldEnemyPos, 35, birdFlag ? BirdL_bmp : BirdR_bmp, 24, 16);  break;     // Рисуем птицу (выбираем одну из двух картинок для анимации)
      }

      switch (newEnemyType) {                                                                           // Выбираем нового противника
        case 0: oled.drawBitmap(newEnemyPos, 48, CactusSmall_bmp, 16, 16);                     break;   // Рисуем маленький кактус
        case 1: oled.drawBitmap(newEnemyPos, 48, CactusBig_bmp, 24, 16);                       break;   // Рисуем большой кактус
        case 2: oled.drawBitmap(newEnemyPos, 35, birdFlag ? BirdL_bmp : BirdR_bmp, 24, 16);    break;   // Рисуем птицу (выбираем одну из двух картинок для анимации)
      }

      if (oldEnemyPos <= (16 + EFH) and oldEnemyPos >= (oldEnemyType > 0 ? -24 - EFH : -16 - EFH)) {    // Если противник в опасной зоне (Отслеживаем столкновения)
        if (oldEnemyType != 2 ? dinoY > 32 - EFH : dinoStand and dinoY > 19 - EFH) {                    // Выбираем условие столкновения в зависимости от типа противника 
          uiTimer = millis();
          oled.drawBitmap(0, dinoY, DinoStandDie_bmp, 16, 16);                                          // Столкнулись - рисуем погибшего динозавра :(  
          oled.roundRect(0, 10, 127, 40, OLED_CLEAR); oled.roundRect(0, 10, 127, 40, OLED_STROKE);      // Очищаем и обводим область
          oled.setScale(2); oled.setCursor(7, 2); oled.print(F("GAME OVER"));                           // Выводим надпись   
          oled.setScale(1); oled.setCursor(3, 4); oled.print(F("<- Вверх"));                            // Выводим подсказку
          oled.setCursor(96, 4); oled.print(F("Ок ->"));                                                // Выводим подсказку
          oled.update();                                                                                // Отрисовка картинки на дисплей
          if (score > bestScore) {                                                                      // Если новый рекорд
            EEPROM.put(DINO_EE_ADDR, score);                                                            // Обновляем его
            EEPROM.commit();                                                                            // Запись
          }                                       
          while (1) {                                                                                   // Бесконечный цикл
            ok.tick();
            up.tick(); 
            if (ok.click()) goto startDinoGame;                                                         // Начинаем сначала
            if (up.click() || millis() - uiTimer > 30000) return;                                       // Вернулись в меню
            yield();
          }
        }
      }

      if (dinoStand) {                                                                                  // Если все окей, столкновения нет и дино стоит в полный рост
        oled.drawBitmap(0, dinoY, legFlag ? DinoStandL_bmp : DinoStandR_bmp, 16, 16);                   // Выводим в полный рост с анимацией переступания  
      } else {                                                                                          // Дино пригнулся
        oled.drawBitmap(0, 56, legFlag ? DinoCroachL_bmp : DinoCroachR_bmp, 16, 8);                     // Выводим пригнувшимся, тоже с анимацией ног
      }

      oled.update();                                                                                    // Финальная отрисовка на дисплей
    }
    yield();
  }
}