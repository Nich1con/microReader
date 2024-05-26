/*
    Обновленный скетч к проекту "Электронная шпаргалка с Wi-Fi" v1.2
    Внимание! Эта версия хранит данные в FS, при перепрошивке всё сбросится!

    Ссылка на ядро для IDE: https://arduino.esp8266.com/stable/package_esp8266com_index.json
    Рекомендуемая версия ядра 3.1.2
    Рекомендуемые настройки:
      ESP8266
        Generic ESP8266 Module
          Upload speed: 921600
          Flash size: 4M (FS:2MB OTA:~1019KB)
          CPU Frequency: 80MHz
          Erase Flash: All flash
          Остальные параметры по умолчанию
*/

/* ================ Настройки ================ */
#define MAX_FILENAME_LEN  12            // Максимальная длина имени файла (не включая .txt/.itxt)
#define AP_DEFAULT_SSID   "Reader AP"   // Стандартное имя точки доступа ESP (До 20-ти символов)
#define AP_DEFAULT_PASS   "00000000"    // Стандартный пароль точки доступа ESP (До 20-ти символов)
#define STA_DEFAULT_SSID  ""            // Стандартное имя точки доступа роутера (До 20-ти символов)
#define STA_DEFAULT_PASS  ""            // Стандартный пароль точки доступа роутера (До 20-ти символов)
#define STA_CONNECT_EN    0             // 1/0 - вкл./выкл. подключение к роутеру
#define OLED_CONTRAST     100           // Яркость дисплея по умолчанию (%)
#define LEFT_MODE         0             // Режим левши
#define WIFI_TIMEOUT_S    300           // Таймаут на отключение Wi-Fi (С)
#define UP_BTN_PIN        14            // Номер GPIO для кнопки ВВЕРХ
#define OK_BTN_PIN        12            // Номер GPIO для кнопки ОК
#define DWN_BTN_PIN       13            // Номер GPIO для кнопки ВНИЗ
#define IIC_SDA_PIN       4             // Номер GPIO SDA дисплея
#define IIC_SCL_PIN       5             // Номер GPIO SCL дисплея
#define EE_KEY            'B'           // Ключ EEPROM (1 байт) - измени, чтобы сбросить настройки
#define VBAT_FULL_MV      3600          // Напряжение питания при заряженном аккуме в (мВ)
#define VBAT_EMPTY_MV     2600          // Напряжение питания при севшем аккуме в (мВ)
#define _EB_DEB           25            // Дебаунс кнопок (мс)
#define GAME_SPEED        350           // Скорость  (меньше - быстрее)
#define CALCUL_TYPE       int64_t       // Тип переменной зачений в калькуляторе

#define T_SEGMENT 4            // Сегмент тетриса
#define MAX_WIDTH 64
#define MAX_HEIGHT 128
/* =========================================== */
/* ============ Список библиотек ============= */
#include <Wire.h>           // Либа I2C
//#include <EEPROM.h>       // Либа EEPROM
#include <FileData.h>       // Замена епрома
#include <LittleFS.h>       // Либа файловой системы
#include <GyverPortal.h>    // Либа веб морды
#include <StringUtils.h>    // Либа утилит строк
#include <GyverOLED_fix.h>  // Либа олед-дисплея
#include <EncButton.h>      // Либа кнопок
#include <TJpg_Decoder.h>   // Либа jpg'а
#include <GyverTimer.h>     // Либа таймера

/* =========================================== */
/* ============ Список объектов ============== */
GyverPortal ui(&LittleFS);          // Портал
GyverOLED<SSD1306_128x64> oled;     // Олед
Button up(UP_BTN_PIN);              // Кнопка вверх
Button ok(OK_BTN_PIN);              // Кнопка ОК
Button down(DWN_BTN_PIN);           // Кнопка вниз
ADC_MODE(ADC_VCC);                  // Режим работы АЦП - измерение VCC
GTimer_ms gameTimer(GAME_SPEED); // Таймер игр

/* =========================================== */
/* ========= Глобальные переменные =========== */
struct {                                // Структура со всеми настройками
  char apSsid[21] = AP_DEFAULT_SSID;    // Имя сети для AP режима по умолчанию
  char apPass[21] = AP_DEFAULT_PASS;    // Пароль сети для AP режима по умолчанию
  char staSsid[21] = STA_DEFAULT_SSID;  // Имя сети для STA режима по умолчанию
  char staPass[21] = STA_DEFAULT_PASS;  // Пароль сети для STA режима по умолчанию
  bool staModeEn = STA_CONNECT_EN;      // Подключаться к роутеру по умолчанию?
  int dispContrast = OLED_CONTRAST;     // Яркость оледа
  bool leftmode = LEFT_MODE;            // Режим левши
  uint8_t tetrisSegment = T_SEGMENT;    // Сегмент тетриса
  uint16_t dinoBestScore = 0;           // Счёт динозавра
  uint16_t tetrBestScore = 0;           // Счёт тетриса
  uint16_t snakeBestScore = 0;          // Счёт змейки
} sets;
FileData data(&LittleFS, "/data.dat", EE_KEY, &sets, sizeof(sets));  // замена епрома
#define SEGMENT (sets.tetrisSegment)

String selectedFile = "";  // Имя выбранной строки
String fileNames = "";     // Имена всех читаемых файлов
int16_t fileCount = 0;     // Количество читаемых файлов
int16_t badCount = 0;      // Количество битых файлов
int16_t cursor = 0;        // Указатель (курсор) меню
int16_t batMv = 3000;      // Напряжение питания ESP
uint32_t uiTimer = 0;      // Таймер таймаута дисплея
uint32_t batTimer = 0;     // Таймер опроса АКБ
uint8_t oledbuf[MAX_WIDTH * MAX_HEIGHT]; // замена матрицы для игр
uint8_t buttons;

bool loadingFlag = 0;      // Флаг игр (остался от прошивки Гайвера)
#define X0 16  // сдвиг по ширине для тетриса
uint8_t WIDTH = (64/SEGMENT - 16/SEGMENT);          // -1 (для текста) // ширина для того же тетриса
uint8_t HEIGHT = (128/SEGMENT);                     // и высота
/* =========================================== */

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);         // Лед на модуле как выход
  pinMode(UP_BTN_PIN, INPUT_PULLUP);
  pinMode(OK_BTN_PIN, INPUT_PULLUP);
  pinMode(DWN_BTN_PIN, INPUT_PULLUP);   // Все пины кнопок в режиме входа с подтяжкой

  batMv = ESP.getVcc();

  if (!digitalRead(UP_BTN_PIN)) {       // Запуск с зажатой кнопкой вверх
    enterToServiceMode();               // Сервис мод со своей инициализацией
  }

  ok.setHoldTimeout(1500);              // Длинное удержание кнопки ОК - 1.5 секунды
  up.setHoldTimeout(1500);
  up.setStepTimeout(100);
  down.setStepTimeout(100);

  selectedFile.reserve(MAX_FILENAME_LEN + 6);
  fileNames.reserve(4096);              // Резервируем 2 глобальных строки

  //EEPROM.begin(100);                    // Инициализация EEPROM
  while (!LittleFS.begin()) {           // Инициализация файловой системы
    LittleFS.format();
  }

  /*if (EEPROM.read(0) != EE_KEY) {  // Если ключ еепром не совпадает
    EEPROM.write(0, EE_KEY);       // Пишем ключ
    EEPROM.put(1, sets);           // Пишем дефолтные настройки
    EEPROM.commit();               // Запись
  } else {                         // Если ключ совпадает
    EEPROM.get(1, sets);           // Читаем настройки
  }*/
  data.read();  // это заменяет то, что выше

  while (!oled.init(IIC_SDA_PIN, IIC_SCL_PIN)) {  // Инициализация оледа
    digitalWrite(LED_BUILTIN, LOW);               // Медленно мигаем пока не проинициализируем олед
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }

  for (uint8_t i = 0; i < 6; i++) {  // Индикатор УСПЕШНОГО запуска ESP
    digitalWrite(LED_BUILTIN, LOW);
    delay(30);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
  }

  digitalWrite(LED_BUILTIN, HIGH);

  Wire.setClock(600E3);
  oled.flipH(sets.leftmode); // Отзеркалить
  oled.flipV(sets.leftmode); // Отзеркалить
  if (sets.leftmode) {  // меняем кнопки
    up = Button(DWN_BTN_PIN);
    down = Button(UP_BTN_PIN);
  }
  oled.clear();              // Очистка оледа
  oled.update();             // Вывод пустой картинки
  oled.autoPrintln(true);    // Включаем автоперенос строки

  checkFileSystem();
  drawMainMenu();

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(oled_output); // Функция отправки на дисплей
  
  WIDTH = (64/SEGMENT - 16/SEGMENT);          // обновляем ширину 
  HEIGHT = (128/SEGMENT);                     // и высоту
}

void loop() {
  up.tick();
  ok.tick();
  down.tick();
  data.tick();  // тикаем память

  if (up.click()) {                                    // Если нажата или удержана кнопка вверх
    uiTimer = millis();                                // Сбрасываем таймер дисплея
    cursor = constrain(cursor - 1, 0, fileCount - 1);  // Двигаем курсор
    drawMainMenu();                                    // Обновляем главное меню
  } else if (down.click()) {                           // Если нажата или удержана кнопка вниз
    uiTimer = millis();                                // Сбрасываем таймер дисплея
    cursor = constrain(cursor + 1, 0, fileCount - 1);  // Двигаем курсор
    drawMainMenu();                                    // Обновляем главное меню
  }

  if (ok.click()) {                         // Если нажата ОК
    uiTimer = millis();                     // Сбрасываем таймер дисплея
    if (fileCount) {
      if (selectedFile.endsWith(".txt")) {  // Если файл - текстовый
        enterToReadTxtFile();               // Читаем как текст
      } else if (selectedFile.endsWith(".itxt") || selectedFile.endsWith(".h")) {  // Если битмап
        enterToReadBmpFile();               // Читаем как картинку
      } else if (selectedFile.endsWith(".jpg")) {
        enterToReadJpgFile();
      }
    }
  }

  if (ok.hold()) {
    uiTimer = millis();  // Сбрасываем таймер дисплея
    enterToWifiMenu();   // Переходим в меню wifi
  }

  if (up.hold()) {
    uiTimer = millis();  // Сбрасываем таймер дисплея
    //dinosaurGame();
    //tetrisGame();
    enterToGameMode();
  }

  if (millis() - uiTimer >= 5000) {  // Каждые 5 сек принудительно обновляем дисп. ради индикации заряда
    uiTimer = millis();
    drawMainMenu();
  }
}
