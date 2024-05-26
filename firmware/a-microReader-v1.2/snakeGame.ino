// Написал Гайвер (https://github.com/AlexGyver/GyverMatrixBT/blob/master/firmware/GyverMatrixOS_v1.12/g_snake.ino). Комментарии автора созранены
// игра змейка! (выход по удержанию ок)
// **************** НАСТРОЙКИ ЗМЕЙКИ ****************
#define START_LENGTH 4    // начальная длина змейки
#define MAX_LENGTH 80     // максимальная длина змейки

// **************** ДЛЯ РАЗРАБОТЧИКОВ ****************
int8_t vectorX, vectorY;
int8_t headX, headY, buttX, buttY;
int8_t appleX, appleY;
boolean apple_flag, missDelete = false;

int8_t buttVector[MAX_LENGTH];
int snakeLength;
boolean butt_flag, gameover;

void snakeGame() {                                                                           // Главное меню игры
  while (true) {                                                                             // Бесконечный цикл
    data.tick();                                                                             // Тикаем память
    oled.clear();                                                                            // Очистка дисплея
    oled.roundRect(0, 9, 127, 46, OLED_STROKE);                                              // Отрисовка интерфейса
    oled.setCursor(3, 0); oled.print(F("SNAKE GAME"));                                       // Отрисовка интерфейса
    oled.setCursor(18, 6); oled.print(F("Лучший счет:")); oled.print(sets.snakeBestScore);   // Вывод рекорда
    oled.setCursor(0, 7); oled.print(F("<- Вверх"));                                         // Вывод доп. инфы
    oled.setCursor(96, 7); oled.print(F("Ок ->"));                                           // Вывод доп. инфы
    //drawFigureRaw(3, 0, 32, 24);                                                             // Вывод картинок
    //drawFigureRaw(5, 1, 16, 16);                                                             // Вывод картинок
    //drawFigureRaw(6, 2, 20, 40);                                                             // Вывод картинок
    const int d_pxlsX[] = {0, 0, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 6, 6, 5, 4, 2};
    const int d_pxlsY[] = {1, 2, 3, 3, 3, 2, 2, 2, 3, 3, 3, 2, 1, 0, 0, 0, 0};
    for (int i = 0; i < sizeof(d_pxlsX) / sizeof(int); i++) {
      int X = 7 + d_pxlsX[i];
      int Y = 0 + d_pxlsY[i];
      oled.rect(X * 6, X0 + Y * 6, X * 6 + 5, X0 + Y * 6 + 5, OLED_STROKE);
    }
    oled.update();                                                                           // Вывод на дисплей
    while (true) {                                                                           // Вложенный бесконечный цикл
      up.tick();
      ok.tick();
      data.tick();                                                                           // Тикаем память

      if (up.click() || millis() - uiTimer >= 10000) {
        drawGameMenu();
        return;
      }

      if (ok.click()) {                                                                  // Нажатие на правую - начать играть
        playSnakeGame();                                                                // Запускаем игру
        loadingFlag = true;
        break;                                                                           // При выходе из игры переходим к отрисовке
      }

      yield();
    }
  }
}
void playSnakeGame() {
  loadingFlag = true;
  oled.clear();
  for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; i++) oledbuf[i] = 0;
  while (1) {
    ok.tick();
    up.tick();
    down.tick();
    data.tick();                              // тикаем память

    checkBatteryCharge();                                                                                                // Проверка напряжение аккума
    drawBatteryCharge();                                                                                                 // Рисуем индикатор
    oled.line(0, 10, 127, 10);                // Линия
    oled.rect(0, 16, 127, 63, OLED_STROKE);   // Рамка
    oled.setCursor(0, 0); oled.print("HI");                                                                              // Выводим рекорд
    oled.setCursor(13, 0); oled.print(sets.snakeBestScore); oled.print(":"); oled.print(snakeLength - START_LENGTH);     // Рекорд:текущий счет
    if (ok.hold()) {
      uiTimer = millis();
      if (snakeLength - START_LENGTH > sets.snakeBestScore) {
        sets.snakeBestScore = snakeLength - START_LENGTH;  // записать
        data.update();                                     // обновить
      }
      break;
    }
    snakeRoutine();
    yield();
  }
  drawGameMenu();
}

void snakeRoutine() {
  if (loadingFlag) {
    oled.clear();
    loadingFlag = false;
    newGameSnake();
  }
  buttonsTickSnake();
  if (gameTimer.isReady()) {
    // БЛОК ГЕНЕРАЦИИ ЯБЛОКА
    while (!apple_flag) {                         // пока яблоко не создано
      appleY = random(0, WIDTH);                  // взять случайные координаты
      appleX = random(0, HEIGHT);

      // проверить, не совпадает ли координата с телом змеи
      while (oledbuf[appleY + appleX * WIDTH] != 0) {
        appleY = random(0, WIDTH);                // взять случайные координаты
        appleX = random(0, HEIGHT);
      }
      apple_flag = true;                          // если не совпадает, считаем что яблоко создано
      oled.rect(appleX * SEGMENT, X0 + appleY * SEGMENT, appleX * SEGMENT + SEGMENT - 1, X0 + appleY * SEGMENT + SEGMENT - 1, OLED_STROKE); // и рисуем
      oledbuf[appleY + appleX * WIDTH] = 1;
      oled.update();
    }

    // запоминаем, куда повернули голову
    // 0 - право, 1 - лево, 2 - вверх, 3 - вниз
    if (vectorX > 0) buttVector[snakeLength] = 0;
    else if (vectorX < 0) buttVector[snakeLength] = 1;
    if (vectorY > 0) buttVector[snakeLength] = 2;
    else if (vectorY < 0) buttVector[snakeLength] = 3;

    // смещение головы змеи
    headX += vectorX;
    headY += vectorY;

    if (headX < 0 || headX > HEIGHT - 1 || headY < 0 || headY > WIDTH - 1) { // если вышла за границы поля
      gameover = true;
    }

    if (!gameover) {
      // проверка на gameover
      if (oledbuf[headY + headX * WIDTH] == 2) { // если змея врезалась в себя
        gameover = true;                           // флаг на отработку
      }

      // БЛОК ОТРАБОТКИ ПОЕДАНИЯ ЯБЛОКА
      if (!gameover && oledbuf[headY + headX * WIDTH] == 1) { // если попали головой в яблоко
        apple_flag = false;                       // флаг что яблока больше нет
        snakeLength++;                            // увеличить длину змеи
        buttVector[snakeLength] = 4;              // запоминаем, что надо будет не стирать хвост
      }

      // вычисляем координату хвоста (чтобы стереть) по массиву вектора
      switch (buttVector[0]) {
        case 0: buttX += 1;
          break;
        case 1: buttX -= 1;
          break;
        case 2: buttY += 1;
          break;
        case 3: buttY -= 1;
          break;
        case 4: missDelete = true;  // 4 значит не стирать!
          break;
      }

      // смещаем весь массив векторов хвоста ВЛЕВО
      for (byte i = 0; i < snakeLength; i++) {
        buttVector[i] = buttVector[i + 1];
      }

      // если змея не в процессе роста, закрасить бывший хвост чёрным
      if (!missDelete) {
        oled.clear(buttX * SEGMENT, X0 + buttY * SEGMENT, buttX * SEGMENT + SEGMENT - 1, X0 + buttY * SEGMENT + SEGMENT - 1);
        oledbuf[buttY + buttX * WIDTH] = 0;
      } else missDelete = false;

      // рисуем голову змеи в новом положении
      oled.rect(headX * SEGMENT, X0 + headY * SEGMENT, headX * SEGMENT + SEGMENT - 1, X0 + headY * SEGMENT + SEGMENT - 1, OLED_STROKE);
      oledbuf[headY + headX * WIDTH] = 2;
      oled.update();
    }
  }

  // если он настал
  if (gameover) {
    gameover = false;

    oled.clear();
    oled.update();
    int score = snakeLength - START_LENGTH;
    if (score > sets.snakeBestScore) {
      sets.snakeBestScore = score;
      data.update();
    }
    delay(1000);
    newGameSnake(); // Тыгдык опять
  }
}

void buttonsTickSnake() {
  if (checkButtons()) {
    if (buttons == 3) {   // кнопка нажата
      _left();
      buttons = 4;
    }
    if (buttons == 1) {   // кнопка нажата
      _right();
      buttons = 4;
    }
  }
}
void _left() {
  if (vectorX == 0 && vectorY == 1) {
    vectorX = 1;
    vectorY = 0;
  } else if (vectorX == 1 && vectorY == 0) {
    vectorX = 0;
    vectorY = -1;
  } else if (vectorX == 0 && vectorY == -1) {
    vectorX = -1;
    vectorY = 0;
  } else if (vectorX == -1 && vectorY == 0) {
    vectorX = 0;
    vectorY = 1;
  }
}
void _right() {
  if (vectorX == 0 && vectorY == 1) {
    vectorX = -1;
    vectorY = 0;
  } else if (vectorX == 1 && vectorY == 0) {
    vectorX = 0;
    vectorY = 1;
  } else if (vectorX == 0 && vectorY == -1) {
    vectorX = 1;
    vectorY = 0;
  } else if (vectorX == -1 && vectorY == 0) {
    vectorX = 0;
    vectorY = -1;
  }
}

void newGameSnake() {
  oled.clear();
  // свежее зерно для генератора случайных чисел
  randomSeed(millis());
  for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; i++) oledbuf[i] = 0;

  // длина из настроек, начинаем в середине экрана, бла-бла-бла
  snakeLength = START_LENGTH;
  headY = WIDTH / 2;
  headX = HEIGHT / 2;
  buttY = headY;

  vectorX = 1;  // начальный вектор движения задаётся вот здесь
  vectorY = 0;
  buttons = 4;

  // первоначальная отрисовка змейки и забивка массива векторов для хвоста
  for (byte i = 0; i < snakeLength; i++) {
    oled.rect((headX - i)*SEGMENT, X0 + headY * SEGMENT, (headX - i)*SEGMENT + SEGMENT - 1, X0 + headY * SEGMENT + SEGMENT - 1, OLED_STROKE);
    oledbuf[headY + (headX - i)*WIDTH] = 2;
    buttVector[i] = 0;
  }
  oled.update();
  buttX = headX - snakeLength;   // координата хвоста как голова - длина
  missDelete = false;
  apple_flag = false;
}
