bool INVERT_IMG = 0;
/* ======================================================================= */
/* ========================== Работа с файлами =========================== */
void checkFileSystem(void) {           // Проверка и индексация файловой системы
  Dir root = LittleFS.openDir("/");    // Открываем директорию (корень)
  fileCount = badCount = 0;            // Обнуляем счетчики файлов
  fileNames = "";                      // Обнуляем список имен
  for (int i = 0; root.next(); i++) {  // Шагаем по директории
    yield();                           // Внутренний поллинг
    File file = root.openFile("r");    // Открываем файл для чтения
    if (file) {                        // Если файл существует
      su::Text filename(file.name());  // Создаем строку с именем
      if ((filename.lengthUnicode() < MAX_FILENAME_LEN + 5) && (filename.endsWith(".txt") || filename.endsWith(".itxt") || filename.endsWith(".h") || filename.endsWith(".jpg"))) {
        fileCount++;                   // Нормальный файл (Имя короткое, тип .txt / .itxt / .h / .jpg)
        fileNames += "/";              // + /
        fileNames += root.fileName();  // + Имя файла
      } else if (!filename.endsWith(".dat")) badCount++;     // Битый
    } else badCount++;                                       // Битый
    file.close();                                            // Закрываем файл
  }
}

/* ======================================================================= */
/* ============================ Чтение файла ============================= */
void drawPage(File file) {        // Отрисовка страницы на олед
  if (!file.available()) return;  // Если файл кончился - не выводим
  oled.clear();
  oled.home();                                 // Очистка и установка в начало дисплея
  while (!oled.isEnd() && file.available()) {  // Выводим символы пока не кончился дисплей или файл
    oled.write(file.read());                   // Транслируем символы в дисплей
  }
  oled.update();  // Выводим картинку
}

void enterToReadTxtFile(void) {        // Режим чтения файла
  String fn = ("/" + selectedFile);    // Собираем путь до файла
  File file = LittleFS.open(fn, "r");  // Открываем файл
  if (!file) {                         // Если сам файл не порядке
    fileReadError();
    checkFileSystem();  // Чекаем файловую систему
    drawMainMenu();     // Рисуем главное меню
    file.close();       // Закрываем файл
    return;             // Выходим
  }

  drawPage(file);          // Если с файлом все ок - рисуем первую страницу
  while (1) {              // Бесконечный цикл
    up.tick();
    ok.tick();
    down.tick();           // Опрос кнопок
    if (ok.click()) {      // Если ок нажат
      uiTimer = millis();  // Сбрасываем таймер дисплея
      drawMainMenu();      // Рисуем главное меню
      file.close();        // Закрываем файл
      return;              // Выходим
    }

    if (up.click() or up.step()) {             // Если нажата или удержана вверх
      uiTimer = millis();                      // Сбрасываем таймер дисплея
      long pos = file.position() - 500;        // Смещаем положение файла вверх
      if (pos < 0) pos = 0;                    // Если достигли нуля - не идем дальше
      file.seek(pos);                          // Устанавливаем указатель файла
      drawPage(file);                          // Рисуем страницу
    } else if (down.click() or down.step()) {  // Если нажата или удержана вниз
      uiTimer = millis();                      // Сбрасываем таймер дисплея
      drawPage(file);                          // Рисуем страницу
    }
    yield();  // Внутренний поллинг ESP
  }
}

void enterToReadBmpFile(void) {
  String fn = ("/" + selectedFile);    // Собираем путь до файла
  File file = LittleFS.open(fn, "r");  // Открываем файл
  if (!file) {                         // Если сам файл не порядке
    fileReadError();
    checkFileSystem();  // Чекаем файловую систему
    drawMainMenu();     // Рисуем главное меню
    file.close();       // Закрываем файл
    return;             // Выходим
  }

  uint8_t *img = new uint8_t[128 * 64];
  if (parseItxt(img, file)) {
    fileReadError();     // Выводим ошибку чтения
    delete[] img;        // Выгружаем буфер
    uiTimer = millis();  // Сбрасываем таймер дисплея
    drawMainMenu();      // Рисуем главное меню
    file.close();        // Закрываем файл
    return;              // Выходим
  }

  oled.clear();                             // Чистим олед
  oled.drawBmpFromRam(0, 0, img, 128, 64);  // Выводим картинку
  oled.update();                            // Обновляем олед
  file.close();                             // Закрываем файл

  while (1) {              // Бесконечный цикл
    ok.tick();             // Опрос кнопки
    down.tick();           // Опрос кнопки
    if (ok.click()) {      // Если ок нажат
      uiTimer = millis();  // Сбрасываем таймер дисплея
      drawMainMenu();      // Рисуем главное меню
      delete[] img;        // Выгружаем буфер
      return;              // Выходим
    }
    if (down.click()) {    // Если нажали вниз
      File file = LittleFS.open(fn, "r");  // Открываем файл
      if (!file) {                         // Если сам файл не порядке
        fileReadError();
        checkFileSystem();   // Чекаем файловую систему
        file.close();        // Закрываем файл
        delete[] img;        // Выгружаем буфер
        uiTimer = millis();  // Сбрасываем таймер дисплея
        drawMainMenu();      // Рисуем главное меню
        return;              // Выходим
      }
      oled.clear();             // Залить чёрным
      INVERT_IMG = !INVERT_IMG; // Инвертировать
      if (parseItxt(img, file)) {
        fileReadError();     // Выводим ошибку чтения
        delete[] img;        // Выгружаем буфер
        uiTimer = millis();  // Сбрасываем таймер дисплея
        drawMainMenu();      // Рисуем главное меню
        file.close();        // Закрываем файл
        return;              // Выходим
      }
      oled.drawBmpFromRam(0, 0, img, 128, 64);  // Выводим картинку
      oled.update();       // Обновить
      file.close();        // Закрываем файл
    }
    yield();  // Внутренний поллинг ESP
  }
}
uint16_t jpgW = 0, jpgH = 0, kW, kH;
void enterToReadJpgFile(void) {
  // Есть ли файл
  String fn = ("/" + selectedFile);    // Собираем путь до файла
  File file = LittleFS.open(fn, "r");  // Открываем файл
  if (!file) {                         // Если сам файл не порядке
    fileReadError();
    checkFileSystem();  // Чекаем файловую систему
    drawMainMenu();     // Рисуем главное меню
    file.close();       // Закрываем файл
    return;             // Выходим
  }
  file.close();        // Закрываем файл

  oled.clear();        // Чистим олед
  TJpgDec.getFsJpgSize(&jpgW, &jpgH, fn, LittleFS);  // получаем размер
  kW = ceil(jpgW / 128.0) > 1 ? ceil(jpgW / 128.0) : 1;
  kH = ceil(jpgH / 64.0) > 1 ? ceil(jpgH / 64.0) : 1;
  TJpgDec.drawFsJpg(0, 0, fn, LittleFS); // Выводим картинку
  oled.update();       // Обновляем олед

  while (1) {              // Бесконечный цикл
    down.tick();           // Опрос кнопки
    ok.tick();             // Опрос кнопки
    if (ok.click()) {      // Если ок нажат
      uiTimer = millis();  // Сбрасываем таймер дисплея
      drawMainMenu();      // Рисуем главное меню
      return;              // Выходим
    }
    if (down.click()) {    // Если нажали вниз
      oled.clear();        // Залить чёрным
      INVERT_IMG = !INVERT_IMG; // Инвертировать
      TJpgDec.drawFsJpg(0, 0, fn, LittleFS); // Выводим картинку
      oled.update();       // Обновить
    }
    yield();  // Внутренний поллинг ESP
  }
}
// Внимание! Картинка выводится блоками 16 на 16!
bool oled_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
  //if ((x + w)/kW > 128 || (y + h)/kH > 64) return 0;  // Проверка размера // глючит
  for (int _y = y; _y < y + h; _y++) {  // Сверху вниз
    for (int _x = x; _x < x + w; _x++) {  // Слева направо
      // Пропускаем лишние пиксели
      if (!(_x % kW) && !(_y % kH)) oled.dot(_x / kW, _y / kH, getBright(bitmap[(_x - x) + (_y - y) * w]) > 127 ? !INVERT_IMG : INVERT_IMG);  // для цветного
      //if (!(_x % kW) && !(_y % kH)) oled.dot(_x / kW, _y / kH, bitmap[(_x - x) + (_y - y) * w] > 32768 ? !INVERT_IMG : INVERT_IMG); // для ч/б
    }
  }
  return 1;
}
uint8_t getBright(uint16_t clr) {                                     // максимальная яркость. Просто выбирает наибольшее значение цвета
  byte r = (clr & 0b1111100000000000) >> 8;
  byte g = (clr & 0b0000011111100000) >> 3;
  byte b = (clr & 0b0000000000011111) << 3;
  return max(max(r, g), b);
}

uint8_t parseItxt(uint8_t *img, File file) {
  int imgLen = 0;                               // Длина+индекс для массива
  memset(img, 0, 1024);                         // Чистим буфер

  while (file.read() != '{') {                  // Ищем начало массива '{'
    if (!file.available()) return 1;            // Если не нашли - ошибка
    yield();                                    // Внутренний поллинг ESP
  }

  while (file.available()) {                    // Пока файл не кончился
    String line = file.readStringUntil('\n');   // Читаем по строке
    su::TextParser p(line, ',');                // Готовим парсер
    while (p.parse()) {                         // Парсим по ','
      uint8_t val = p.trim().toInt32HEX();      // Вытаскиваем байт
      if (INVERT_IMG) val = ~val;               // Если надо - инвертировать
      img[imgLen] = val;                        // Записать
      if (++imgLen >= 1023) return 0;           // Как только насобирали 1кб - выходим праздновать
      yield();                                  // Внутренний поллинг ESP
    } yield();                                  // На всякий случай еще и для внешнего цикла
  }

  return 1;                                     // не насобирали - ошибка
}
/* ======================================================================= */
