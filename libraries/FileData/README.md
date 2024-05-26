[![latest](https://img.shields.io/github/v/release/GyverLibs/FileData.svg?color=brightgreen)](https://github.com/GyverLibs/FileData/releases/latest/download/FileData.zip)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/FileData?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# FileData
Замена EEPROM для ESP8266/32 для хранения любых данных в файлах
- Механизм автоматического "флага" первой записи
- Поддержка всех файловых систем (LittleFS, SPIFFS, SDFS) 
- Поддержка любых типов статических данных
- Отложенная запись по таймауту
- "Обновление" данных - файл не перезапишется, если данные не изменились

### Совместимость
ESP8266, ESP32

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **FileData** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/FileData/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="init"></a>
## Инициализация

```cpp
FileData;
FileData(fs::FS* fs);
FileData(fs::FS* fs, const char* path);
FileData(fs::FS* fs, const char* path, uint8_t key);
FileData(fs::FS* fs, const char* path, uint8_t key, void* data, uint16_t size);
FileData(fs::FS* fs, const char* path, uint8_t key, void* data, uint16_t size, uint16_t tout);

// fs - файловая система, адрес (&LittleFS, &SDFS..)
// path - путь (имя) файла. Может быть любым, как и расширение ("/myData", "/data/settings.dat")
// key - ключ первой записи. Не рекомендуется задавать 0 и 255. Рекомендуется использовать символы ('A', 'F')
// data - ссылка на переменную (массив, структуру, класс)
// size - размер переменной, можно передать как sizeof(переменная)
// tout - таймаут обновления в миллисекундах (умолч 5000)
```

<a id="usage"></a>
## Использование

В ESP8266/32 EEPROM память эмулируется из Flash памяти, реализация EEPROM во встроенной библиотеке имеет следующие минусы:
- Размер ограничен 4 кБ
- Весь указанный в `EEPROM.begin()` объём памяти дублируется в оперативной памяти до вызова `EEPROM.end()`
- При любом изменении в "EEPROM" памяти (даже 1-го байта) и вызове `EEPROM.commit()` полностью стирается и перезаписывается весь сектор 4 кБ. То есть износ памяти происходит не по ячейкам, а полностью всей EEPROM области! Около 10-20 тысяч перезаписей данных и EEPROM памяти больше нет

Предлагается использовать для хранения данных файловую систему (например встроенную LittleFS), которая сама заботится о перезаписи памяти и занимается ротацией файлов по отведённой области, что многократно увеличивает ресурс памяти и надёжность хранения данных. Также это позволит "скачивать" сохранённые данные, делать бэкапы и так далее. Данная библиотека является аналогом [EEManager](https://github.com/GyverLibs/EEManager) и имеет схожие с ней механизмы и возможности:
- "Подключение" статических переменных любого типа, библиотека сама будет читать и писать их содержимое в файл
- Механизм "ключа первого запуска" - если файл не существует или не содержит указанный ключ - в файл будут записаны данные "по умолчанию"
- Механизм отложенной записи по таймауту - после изменения данных достаточно дать библиотеке команду на обновление, и она обновит данные по истечению таймаута

```cpp
// установить файловую систему и путь к файлу
void setFS(fs::FS* nfs, const char* path);

// установить ключ
void setKey(uint8_t key);

// подключить данные (переменную)
void setData(void* data, uint16_t size);

// установить таймаут записи
void setTimeout(uint16_t tout);

// прочитать файл в переменную
// возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE/FD_ADD/FD_READ
FDstat_t read();

// обновить сейчас
// возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE/FD_NO_DIF
FDstat_t updateNow();

// отложить обновление на заданный таймаут
void update();

// тикер, обновит данные по таймауту
// возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE/FD_NO_DIF/FD_WAIT/FD_IDLE
FDstat_t tick();

// записать данные в файл
// возврат: FD_FS_ERR/FD_FILE_ERR/FD_WRITE
FDstat_t write();

// сбросить ключ
// возврат: FD_FS_ERR/FD_FILE_ERR/FD_RESET
FDstat_t reset();

// включить режим увеличения данных с добавлением в файл без очистки
void addWithoutWipe(bool addw);

// ============== СТАТУСЫ ==============
FD_IDLE      // 0 - холостая работа
FD_WAIT      // 1 - ожидание таймаута
FD_FS_ERR    // 2 - ошибка файловой системы
FD_FILE_ERR  // 3 - ошибка открытия файла
FD_WRITE     // 4 - запись данных в файл
FD_READ      // 5 - чтение данных из файла
FD_ADD       // 6 - добавление данных в файл
FD_NO_DIF    // 7 - данные не отличаются (не записаны)
FD_RESET     // 8 - произведён сброс ключа
```

### Порядок действий (глобальные данные)
Для хранения "настроек" программы в глобальной области, чтобы всегда иметь к ним доступ:
1. Создать глобальную переменную и объект `FileData`
2. Передать переменную в объект
3. Прочитать данные `read()` при запуске
4. Вызывать `tick()` в `loop()`
5. После изменения данных вызвать `update()`
6. По истечению таймаута данные сами запишутся в файл

> Таким образом безопасно вызывать `update()` несколько раз подряд, например при изменении "настройки" кнопками - данные обновятся только после окончания ввода и таймаута

### Порядок действий (локальные данные)
Для хранения настроек в файле и чтении/изменении внутри функций в программе:
1. Создать переменную и объект `FileData`
2. Передать переменную в объект
3. Прочитать данные `read()`
4. Если данные изменились и их нужно сохранить - вызвать `updateNow()`

```cpp
void func() {
  Data mydata;
  FileData data(&LittleFS, "/data.dat", 'A', &mydata, sizeof(mydata));
  data.read();
  // ...
  data.updateNow();
}
```

### Изменение данных
- При изменении размера данных (при изменении количества полей в структуре) будет выполнен сброс при чтении - в файл запишется новая структура
- При установке флага `addWithoutWipe(true)` и при **увеличении** размера данных: данные старого размера прочитаются из файла, затем в файл будут дописаны новые данные (разница со старым размером). Это удобно при разработке проекта - добавление новых "настроек" не будет сбрасывать старые

<a id="example"></a>
## Пример

```cpp
#include <Arduino.h>
#include <FileData.h>
#include <LittleFS.h>

struct Data {
  uint8_t val8;
  uint16_t val16;
  uint32_t val32 = 12345;
  char str[20];
};
Data mydata;

FileData data(&LittleFS, "/data.dat", 'B', &mydata, sizeof(mydata));

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  LittleFS.begin();
  
  // прочитать данные из файла в переменную
  // при первом запуске в файл запишутся данные из структуры
  FDstat_t stat = data.read();

  switch (stat) {
    case FD_FS_ERR: Serial.println("FS Error");
      break;
    case FD_FILE_ERR: Serial.println("Error");
      break;
    case FD_WRITE: Serial.println("Data Write");
      break;
    case FD_ADD: Serial.println("Data Add");
      break;
    case FD_READ: Serial.println("Data Read");
      break;
    default:
      break;
  }

  Serial.println("Data read:");
  Serial.println(mydata.val8);
  Serial.println(mydata.val16);
  Serial.println(mydata.val32);
  Serial.println(mydata.str);
}

void loop() {
  // data.tick();  // вызывать тикер в loop
  
  // можно отловить момент записи
  if (data.tick() == FD_WRITE) Serial.println("Data updated!");

  // запишем в данные строку из монитора порта
  // а также присвоим остальным переменным случайные значения
  if (Serial.available()) {
    int len = Serial.readBytes(mydata.str, 20);
    mydata.str[len] = '\0';
    mydata.val8 = random(255);
    mydata.val16 = random(65000);
    Serial.println("Update");

    // отложить обновление
    data.update();
  }
}
```

<a id="versions"></a>
## Версии
- v1.0

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код