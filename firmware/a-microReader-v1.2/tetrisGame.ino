// Написал Гайвер (https://github.com/AlexGyver/GyverMatrixBT/blob/master/firmware/GyverMatrixOS_v1.12/g_tetris.ino). Комментарии автора созранены
// игра  тетрис! (выход по удержанию ок)
// **************** НАСТРОЙКИ ТЕТРИС ****************
#define STEER_SPEED 40    // скорость перемещения в бок при удержании кнопки (меньше - быстрее)

// --------------------- ДЛЯ РАЗРАБОТЧИКОВ ----------------------

int8_t fig = 0, ang = 0, pos = WIDTH / 2, height = HEIGHT - 1;
int8_t prev_ang, prev_pos, prev_height;

//uint32_t colors[6] {0x0000EE, 0xEE0000, 0x00EE00, 0x00EEEE, 0xEE00EE, 0xEEEE00};
uint32_t color = 2;
byte color_index;
byte linesToClear;
boolean down_flag = true;
byte lineCleanCounter;

// самая важная часть программы! Координаты пикселей фигур
//  0 - палка
//  1 - кубик
//  2 - Г
//  3 - Г обратная
//  4 - Z
//  5 - Z обратная
//  6 - Т


const int8_t figures[7][12][2] PROGMEM = {
  {
    { -1, 0}, {1, 0}, {2, 0},
    {0, 1}, {0, 2}, {0, 3},
    { -1, 0}, {1, 0}, {2, 0},
    {0, 1}, {0, 2}, {0, 3},
  },
  {
    {0, 1}, {1, 0}, {1, 1},
    {0, 1}, {1, 0}, {1, 1},
    {0, 1}, {1, 0}, {1, 1},
    {0, 1}, {1, 0}, {1, 1},
  },
  {
    { -1, 0}, { -1, 1}, {1, 0},
    {0, 1}, {0, 2}, {1, 2},
    { -2, 1}, { -1, 1}, {0, 1},
    { -1, 0}, {0, 1}, {0, 2},
  },
  {
    { -1, 0}, {1, 0}, {1, 1},
    {0, 1}, {0, 2}, {1, 0},
    {0, 1}, {1, 1}, {2, 1},
    {0, 1}, {0, 2}, { -1, 2},
  },
  {
    { -1, 0}, {0, 1}, {1, 1},
    {0, 1}, { -1, 1}, { -1, 2},
    { -1, 0}, {0, 1}, {1, 1},
    {0, 1}, { -1, 1}, { -1, 2},
  },
  {
    { -1, 1}, {0, 1}, {1, 0},
    {0, 1}, {1, 1}, {1, 2},
    { -1, 1}, {0, 1}, {1, 0},
    {0, 1}, {1, 1}, {1, 2},
  },
  {
    { -1, 0}, {0, 1}, {1, 0},
    {0, 1}, {0, 2}, {1, 1},
    { -1, 1}, {0, 1}, {1, 1},
    { -1, 1}, {0, 1}, {0, 2},
  }
};

void tetrisGame() {                                                                          // Главное меню игры
  lineCleanCounter = 0;
  while (true) {                                                                             // Бесконечный цикл
    data.tick();                                                                             // Тикаем память
    oled.clear();                                                                            // Очистка дисплея
    oled.roundRect(0, 9, 127, 46, OLED_STROKE);                                              // Отрисовка интерфейса
    oled.setCursor(3, 0); oled.print(F("TETRIS GAME"));                                      // Отрисовка интерфейса
    oled.setCursor(18, 6); oled.print(F("Лучший счет:")); oled.print(sets.tetrBestScore);    // Вывод рекорда
    oled.setCursor(0, 7); oled.print(F("<- Вверх"));                                         // Вывод доп. инфы
    oled.setCursor(96, 7); oled.print(F("Ок ->"));                                           // Вывод доп. инфы
    //drawFigureRaw(3, 0, 32, 24);                                                             // Вывод картинок
    //drawFigureRaw(5, 1, 16, 16);                                                             // Вывод картинок
    //drawFigureRaw(6, 2, 20, 40);                                                             // Вывод картинок
    const bool pxls[] = {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
                         1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0,
                         1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                         0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                        };
    const int W = 15;
    for (int i = 0; i < sizeof(pxls) / sizeof(bool); i++) {
      int X = 3 + (i % W);
      int Y = i / W;
      if (pxls[i]) oled.rect(X * 6, X0 + Y * 6, X * 6 + 5, X0 + Y * 6 + 5, OLED_STROKE);
    }
    oled.update();                                                                           // Вывод на дисплей
    while (true) {                                                                           // Вложенный бесконечный цикл
      up.tick();
      ok.tick();
      data.tick();                                                                           // Тикаем память

      if (up.click() || millis() - uiTimer >= 10000) {
        if (sets.leftmode) {
          oled.flipH(1);              // Отзеркалить
          oled.flipV(1);              // Отзеркалить
          up = Button(DWN_BTN_PIN);   // меняем кнопки
          down = Button(UP_BTN_PIN);
        }
        drawGameMenu();
        return;
      }

      if (ok.click()) {                                                                  // Нажатие на ок - начать играть
        if (sets.leftmode) {
          oled.flipH(0);              // Отзеркалить
          oled.flipV(0);              // Отзеркалить
          down = Button(DWN_BTN_PIN); // меняем кнопки
          up = Button(UP_BTN_PIN);
        }
        playTetrisGame();                                                                // Запускаем игру
        loadingFlag = true;
        break;                                                                           // При выходе из игры переходим к отрисовке
      }

      yield();
    }
  }
}
void playTetrisGame() {
  //loadingFlag = true;
  oled.clear();
  lineCleanCounter = 0;
  for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; i++) oledbuf[i] = 0;
  while (1) {
    ok.tick();
    up.tick();
    down.tick();
    data.tick();  // тикаем память

    checkBatteryCharge();                                                                                      // Проверка напряжение аккума
    drawBatteryCharge();                                                                                       // Рисуем индикатор
    oled.line(0, 10, 127, 10);   // Линия
    //oled.line(127, 63, 127, 16);  // Опять линия
    oled.rect(0, 16, 127, 63, OLED_STROKE);   // Рамка
    oled.setCursor(0, 0); oled.print("HI");                                                                    // Выводим рекорд
    oled.setCursor(13, 0); oled.print(sets.tetrBestScore); oled.print(":"); oled.print(lineCleanCounter);      // Рекорд:текущий счет
    if (ok.hold()) {
      uiTimer = millis();
      if (lineCleanCounter > sets.tetrBestScore) {
        sets.tetrBestScore = lineCleanCounter; // записать
        data.update();                         // обновить
      }
      lineCleanCounter = 0;
      break;
    }
    tetrisRoutine();
    yield();
  }
  drawGameMenu();
}

void tetrisRoutine() {
  if (loadingFlag) {
    oled.clear();
    loadingFlag = false;
    newGameTetris();
  }

  if (checkButtons()) {

    if (buttons == 3) {   // кнопка нажата
      buttons = 4;
      stepLeft();
    }

    if (buttons == 1) {
      buttons = 4;
      stepRight();
    }

    if (buttons == 0) {
      buttons = 4;
      if (checkArea(3)) {       // проверка возможности поворота
        prev_ang = ang;         // запоминаем старый угол
        ang = ++ang % 4;        // изменяем ang от 0 до 3 (да, хитро)
        redrawFigure(prev_ang, pos, height);    // перерисовать фигуру
      }
    }
    /*
        if (buttons == 2) {             // кнопка вниз удерживается
          buttons = 4;
          gameTimer.setInterval(FAST_SPEED);  // увеличить скорость
        }*/
  }


  if (up.step()) {    // кнопка нажата и удерживается
    stepLeft();
  }
  if (down.step()) {
    stepRight();
  }


  if (gameTimer.isReady()) {        // главный таймер игры
    prev_height = height;

    if (!checkArea(0)) {            // проверяем столкновение с другими фигурами
      if (height >= HEIGHT - 2) {   // проиграл по высоте
        gameOverTetris();                 // игра окончена, очистить всё
        newGameTetris();                 // новый раунд
      } else {                      // если не достигли верха
        fixFigure();                // зафиксировать
        checkAndClear();            // проверить ряды и очистить если надо
        newGameTetris();                 // новый раунд
      }
    } else if (height == 0) {       // если достигли дна
      fixFigure();                  // зафиксировать
      checkAndClear();              // проверить ряды и очистить если надо
      newGameTetris();                   // новый раунд
    } else {                        // если путь свободен
      height--;                             // идём вниз
      redrawFigure(ang, pos, prev_height);  // перерисовка
    }
  }
}

// поиск и очистка заполненных уровней
void checkAndClear() {
  linesToClear = 1;                 // счётчик заполненных строк по вертикали. Искусственно принимаем 1 для работы цикла
  boolean full_flag = true;         // флаг заполненности
  while (linesToClear != 0) {       // чисти чисти пока не будет чисто!
    linesToClear = 0;
    byte lineNum = 255;       // высота, с которой начинаются заполненные строки (искусственно увеличена)
    for (byte Y  = 0; Y < HEIGHT; Y++) {   // сканируем по высоте
      full_flag = true;                   // поднимаем флаг. Будет сброшен, если найдём чёрный пиксель
      for (byte X = 0; X < WIDTH; X++) {  // проходимся по строкам
        if (oledbuf[X + Y * WIDTH] == 0) { // если хоть один пиксель чёрный
          full_flag = false;                                 // считаем строку неполной
        }
      }
      if (full_flag) {        // если нашлась заполненная строка
        linesToClear++;       // увеличиваем счётчик заполненных строк
        if (lineNum == 255)   // если это первая найденная строка
          lineNum = Y;        // запоминаем высоту. Значение 255 было просто "заглушкой"
      } else {                // если строка не полная
        if (lineNum != 255)   // если lineNum уже не 255 (значит строки были найдены!!)
          break;              // покинуть цикл
      }
    }
    if (linesToClear > 0) {             // если найденных полных строк больше 1
      lineCleanCounter += linesToClear;   // суммируем количество очищенных линий (игровой "счёт")

      // заполняем весь блок найденных строк белым цветом слева направо
      for (byte X = 0; X < WIDTH; X++) {
        for (byte i = 0; i < linesToClear; i++) {
          oled.clear((SEGMENT * (HEIGHT - 1 - (lineNum + i))), X0 + (SEGMENT * X), (SEGMENT * (HEIGHT - 1 - (lineNum + i)) + SEGMENT) - 1, X0 + (SEGMENT * X + SEGMENT) - 1); // стираем его
          oledbuf[X + (lineNum + i)*WIDTH] = 0;
        }
        oled.update();
        delay(5);     // задержка между пикселями слева направо
      }
      delay(1000);

      // и теперь смещаем вниз все пиксели выше уровня с первой найденной строкой
      for (byte i = 0; i < linesToClear; i++) {
        for (byte Y = lineNum; Y < HEIGHT - 1; Y++) {
          for (byte X = 0; X < WIDTH; X++) {
            oled.rect((SEGMENT * (HEIGHT - 1 - Y)), X0 + (SEGMENT * X), (SEGMENT * (HEIGHT - 1 - Y) + SEGMENT) - 1, X0 + (SEGMENT * X + SEGMENT) - 1, oledbuf[X + (Y + 1)*WIDTH] == 0 ? OLED_CLEAR : OLED_STROKE); // сдвигаем вниз
            oledbuf[X + Y * WIDTH] = oledbuf[X + (Y + 1) * WIDTH]; // сдвигаем вниз
          }
          oled.update();
        }
        delay(100);       // задержка между "сдвигами" всех пикселей на один уровень
      }
    }
  }
  gameTimer.reset();
}


// функция фиксации фигуры
void fixFigure() {
  color = 1;                   // чутка перекрасить
  redrawFigure(ang, pos, prev_height);  // перерисовать
}

// проигрыш
void gameOverTetris() {
  oled.clear();
  oled.update();
  for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; i++) oledbuf[i] = 0;

  // тут можно вывести счёт lineCleanCounter
  oled.home();
  oled.print(lineCleanCounter);
  oled.update();
  if (lineCleanCounter > sets.tetrBestScore) {
    sets.tetrBestScore = lineCleanCounter; // записать
    data.update();                         // обновить
  }
  delay(1000);
  lineCleanCounter = 0;   // сброс счёта
  delay(20);
  newGameTetris();
}

// новый раунд
void newGameTetris() {
  //Serial.println("lolkek");   // без этого работает некорректно! магия  // Работает и так
  delay(10);
  buttons = 4;
  height = HEIGHT;    // высота = высоте дисплея
  pos = WIDTH / 2;    // фигура появляется в середине
  fig = random(7);    // выбираем слулчайно фигуру
  ang = random(4);    // и угол поворота
  color = 2;

  // возвращаем обычную скорость падения
  gameTimer.setInterval(GAME_SPEED);
  down_flag = false;  // разрешаем ускорять кнопкой "вниз"
  delay(10);
}

// управление фигурами вправо и влево
void stepRight() {
  if (checkArea(1)) {
    prev_pos = pos;
    if (++pos > WIDTH) pos = WIDTH;
    redrawFigure(ang, prev_pos, height);
  }
}
void stepLeft() {
  if (checkArea(2)) {
    prev_pos = pos;
    if (--pos < 0) pos = 0;
    redrawFigure(ang, prev_pos, height);
  }
}

// проверка на столкновения
boolean checkArea(int8_t check_type) {
  // check type:
  // 0 - проверка лежащих фигур и пола
  // 1 - проверка стенки справа и фигур
  // 2 - проверка стенки слева и фигур
  // 3 - проверка обеих стенок и пола

  boolean flag = true;
  int8_t X, Y;
  boolean offset = 1;
  int8_t this_ang = ang;

  // этот режим для проверки поворота. Поэтому "поворачиваем"
  // фигуру на следующий угол чтобы посмотреть, не столкнётся ли она с чем
  if (check_type == 3) {
    this_ang = ++this_ang % 4;
    offset = 0;   // разрешаем оказаться вплотную к стенке
  }

  for (byte i = 0; i < 4; i++) {
    // проверяем точки фигуры
    // pos, height - координаты главной точки фигуры в ГЛОБАЛЬНОЙ системе координат
    // X, Y - координаты остальных трёх точек в ГЛОБАЛЬНОЙ системе координат
    if (i == 0) {   // стартовая точка фигуры (начало отсчёта)
      X = pos;
      Y = height;
    } else {        // остальные три точки
      // получаем глобальные координаты точек, прибавив их положение в
      // системе координат главной точки к координатам самой главной
      // точки в глобальной системе координат. Если ты до сюда дочитал,
      // то стукни мне [Гайверу] на почту alex@alexgyver.ru . Печенек не дам, но ты молодец!
      X = pos + (int8_t)pgm_read_byte(&figures[fig][this_ang * 3 + i - 1][0]);
      Y = height + (int8_t)pgm_read_byte(&figures[fig][this_ang * 3 + i - 1][1]);

      // дичь дикая! Но на самом деле просто восстанавливаем из прогмема данные
      // и берём нужное число в массиве. Откуда все эти * 3 -1 ... можно додуматься
    }

    // границы поля
    if (check_type == 1 || check_type == 3) {
      if (X + 1 > WIDTH - 1) flag = false;    // смотрим следующий справа
      uint32_t getColor;
      if (Y < HEIGHT)
        getColor = oledbuf[X + offset + Y * WIDTH];
      if (getColor != 2 && getColor != 0) {
        flag = false;         // если не СВОЙ цвет и не чёрный
      }
    }

    if (check_type == 2 || check_type == 3) {
      if (X - 1 < 0) flag = false;    // смотрим следующий слева
      uint32_t getColor;
      if (Y < HEIGHT)
        getColor = oledbuf[X - offset + Y * WIDTH];
      if (getColor != 2 && getColor != 0) {
        flag = false;         // если не СВОЙ цвет и не чёрный
      }
    }

    if (check_type == 0 || check_type == 3) {
      uint32_t getColor;
      if (Y < HEIGHT) {
        getColor = oledbuf[X + (Y - 1) * WIDTH];
        if (getColor != 2 && getColor != 0) {
          flag = false;         // если не СВОЙ цвет и не чёрный
        }
      }
    }
  }
  return flag;    // возвращаем глобальный флаг, который говорит о том, столкнёмся мы с чем то или нет
}

// функция, которая удаляет текущую фигуру (красит её чёрным)
// а затем перерисовывает её в новом положении
void redrawFigure(int8_t clr_ang, int8_t clr_pos, int8_t clr_height) {
  drawFigure(fig, clr_ang, clr_pos, clr_height, 0);            // стереть фигуру
  drawFigure(fig, ang, pos, height, color);                           // отрисовать
  oled.update();
}

// функция, отрисовывающая фигуру заданным цветом и под нужным углом
void drawFigure(byte figure, byte angle, byte x, byte y, uint32_t color) {
  oledbuf[x + y * WIDTH] = color; // записать в массив
  oled.rect((SEGMENT * (HEIGHT - 1 - y)), X0 + (SEGMENT * x), (SEGMENT * (HEIGHT - 1 - y) + SEGMENT) - 1, X0 + (SEGMENT * x + SEGMENT) - 1, color >= 1 ? OLED_STROKE : OLED_CLEAR); // рисуем точку начала координат фигуры
  int8_t X, Y;                      // вспомогательные
  for (byte i = 0; i < 3; i++) {    // рисуем 4 точки фигуры
    // что происходит: рисуем фигуру относительно текущей координаты падающей точки
    // просто прибавляем "смещение" из массива координат фигур
    // для этого идём в прогмем (функция pgm_read_byte)
    // обращаемся к массиву по адресу &figures
    // преобразовываем число в int8_t (так как progmem работает только с "unsigned"
    // angle * 3 + i - обращаемся к координатам согласно текущему углу поворота фигуры

    X = x + (int8_t)pgm_read_byte(&figures[figure][angle * 3 + i][0]);
    Y = y + (int8_t)pgm_read_byte(&figures[figure][angle * 3 + i][1]);
    if (Y > HEIGHT - 1) continue;   // если выходим за пределы поля, пропустить отрисовку
    oledbuf[X + Y * WIDTH] = color; // записать в массив
    oled.rect((SEGMENT * (HEIGHT - 1 - Y)), X0 + (SEGMENT * X), (SEGMENT * (HEIGHT - 1 - Y) + SEGMENT) - 1, X0 + (SEGMENT * X + SEGMENT) - 1, color >= 1 ? OLED_STROKE : OLED_CLEAR); // нарисовать
  }
}
/*
  // аналогично, но координаты 0..128/0..64 и размер сегмента == 4
  void drawFigureRaw(byte figure, byte angle, byte x, byte y) {
  oledbuf[x + y * WIDTH] = color; // записать в массив
  oled.rect(y, x, y + 3, x + 3, OLED_STROKE); // рисуем точку начала координат фигуры
  int8_t X, Y;                      // вспомогательные
  for (byte i = 0; i < 3; i++) {    // рисуем 4 точки фигуры
    X = x + (int8_t)pgm_read_byte(&figures[figure][angle * 3 + i][0]) * 4;
    Y = y + (int8_t)pgm_read_byte(&figures[figure][angle * 3 + i][1]) * 4;
    if (Y > HEIGHT - 1) continue;   // если выходим за пределы поля, пропустить отрисовку
    oled.rect(Y, X, Y + 3, X + 3, OLED_STROKE); // нарисовать
  }
  }
*/
boolean checkButtons() {
  if (up.click()) buttons = 3;
  if (down.click()) buttons = 1;
  if (ok.click()) buttons = 0;

  if (buttons != 4) return true;
  return false;
}
