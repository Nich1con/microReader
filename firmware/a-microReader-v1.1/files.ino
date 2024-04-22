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
      if ((filename.lengthUnicode() < MAX_FILENAME_LEN + 5) && (filename.endsWith(".txt") || filename.endsWith(".itxt") || filename.endsWith(".h"))) {
        fileCount++;                   // Нормальный файл (Имя короткое, тип .txt / .itxt / .h)
        fileNames += "/";              // + /
        fileNames += root.fileName();  // + Имя файла
      } else badCount++;               // Битый
    } else badCount++;                 // Битый
    file.close();                      // Закрываем файл
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
  delete[] img;                             // Выгружаем буфер

  while (1) {              // Бесконечный цикл
    ok.tick();             // Опрос кнопки
    if (ok.click()) {      // Если ок нажат
      uiTimer = millis();  // Сбрасываем таймер дисплея
      drawMainMenu();      // Рисуем главное меню
      file.close();        // Закрываем файл
      return;              // Выходим
    }
    yield();  // Внутренний поллинг ESP
  }
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
      img[imgLen] = p.trim().toInt32HEX();      // Вытаскиваем байт
      if (++imgLen >= 1023) return 0;           // Как только насобирали 1кб - выходим праздновать
      yield();                                  // Внутренний поллинг ESP
    } yield();                                  // На всякий случай еще и для внешнего цикла
  }

  return 1;                                     // не насобирали - ошибка  
}
/* ======================================================================= */