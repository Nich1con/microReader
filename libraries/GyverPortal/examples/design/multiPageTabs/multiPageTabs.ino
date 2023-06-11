// виртуальная мультистраничность (на одной странице) со вкладками

#define AP_SSID ""
#define AP_PASS ""

#include <GyverPortal.h>
GyverPortal ui;

// конструктор страницы
void build() {
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);

  // первый блок навигации
  GP.NAV_TABS("Home,Settings,Keks,End");

  GP.NAV_BLOCK_BEGIN();
  GP.LABEL("Tab1");
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.LABEL("Tab2");
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.LABEL("Tab3");
  GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP.LABEL("Tab4");
  GP.NAV_BLOCK_END();

  // второй блок навигации
  GP.NAV_TABS("KEK,PUK,4EBUREK", GP_RED);

  // эти блоки сделаем макросами
  M_NAV_BLOCK(
    GP.LABEL("block1");
    GP.BUTTON("", "Btn");
  );

  M_NAV_BLOCK(
    GP.LABEL("block2");
    GP.LED("");
  );

  M_NAV_BLOCK(
    GP.LABEL("block3");
    GP.BREAK();
    GP.TEXT("");
  );

  GP.BUILD_END();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  // подключаем конструктор и запускаем
  ui.attachBuild(build);
  ui.attach(action);
  ui.start();
}

void action() {
}

void loop() {
  ui.tick();
}
