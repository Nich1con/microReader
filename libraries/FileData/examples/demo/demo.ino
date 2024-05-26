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