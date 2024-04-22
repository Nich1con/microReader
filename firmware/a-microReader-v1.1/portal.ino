/* ======================================================================= */
/* ================= Билд веб-страницы + подсос значений ================= */
void build() {                     // Билд страницы
  GP.BUILD_BEGIN(400);             // Ширина колонок
  GP.THEME(GP_DARK);               // Темная тема
  GP.PAGE_TITLE("Wi-Fi Reader");   // Обзываем титл
  GP.FORM_BEGIN("/cfg");           // Начало формы
  GP.GRID_RESPONSIVE(600);         // Отключение респонза при узком экране
  M_BLOCK(                         // Общий блок-колонка для WiFi
    GP.SUBMIT("Сохранить");        // Кнопка отправки формы
    M_BLOCK_TAB(                   // Конфиг для AP режима -> текстбоксы (логин + пароль)
      "Точка доступа",             // Имя + тип DIV
      GP.TEXT("apSsid", "Имя сети", sets.apSsid, "", 20);
      GP.BREAK();
      GP.TEXT("apPass", "Пароль", sets.apPass, "", 20); GP.BREAK(););
    M_BLOCK_TAB(                   // Конфиг для STA режима -> текстбоксы (логин + пароль)
      "Подключение к сети",        // Имя + тип DIV
      GP.TEXT("staSsid", "Имя сети", sets.staSsid, "", 20);
      GP.BREAK();
      GP.TEXT("staPass", "Пароль", sets.staPass, "", 20); GP.BREAK();
      M_BOX(GP_CENTER, GP.LABEL("Автоподключение"); GP.SWITCH("staEn", sets.staModeEn);););
    GP.FORM_END();                 // <- Конец формы (костыль)
    M_BLOCK_TAB(                   // Блок с OTA-апдейтом
      "Обновление прошивки",       // Имя + тип DIV
      GP.OTA_FIRMWARE();           // Кнопка с OTA начинкой
    );
    M_BLOCK_TAB(                   // Блок с файловым менеджером
      "Файловая система",          // Имя + тип DIV
      GP.FILE_UPLOAD("file_upl");  // Кнопка для загрузки файла
      GP.FILE_MANAGER(&LittleFS);  // Файловый менеджер
    ););
  GP.BUILD_END();                  // Конец билда страницы
}

void action(GyverPortal& p) {      // Подсос значений со страницы
  if (p.form("/cfg")) {            // Если есть сабмит формы - копируем все в переменные
    p.copyStr("apSsid", sets.apSsid);
    p.copyStr("apPass", sets.apPass);
    p.copyStr("staSsid", sets.staSsid);
    p.copyStr("staPass", sets.staPass);
    p.copyBool("staEn", sets.staModeEn);

    if(strlen(sets.apSsid) < 1)  strcpy(sets.apSsid, AP_DEFAULT_SSID);    // Проверка на пустое имя сети AP
    if(strlen(sets.apPass) < 8)  strcpy(sets.apPass, AP_DEFAULT_PASS);    // Проверка на пустой пасс AP

    if(sets.staModeEn){                                                   // Если включен коннект
      if(strlen(sets.staSsid) < 1 || strlen(sets.staPass) < 8) {          // Проверка имени и пасса
        sets.staModeEn = false;                                           // Битое имя/пасс - выключаем коннект
      }
    }

    uint8_t contrast = map(sets.dispContrast, 10, 100, 1, 255);
    oled.setContrast(contrast);   // Тут же задаем яркость оледа
    EEPROM.put(1, sets);          // Сохраняем все настройки в EEPROM
    EEPROM.commit();              // Записываем
  }
}