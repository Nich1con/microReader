// Калькулятор
/*
  Управление:
  Удержание ок - следующее число
  Двойной тык туда же - посчитать
  Тройной - выход
*/

CALCUL_TYPE a, b, result;
uint8_t thisval, sign;  // 0-+ 1-- 2-* 3-/ 4-^
bool isdraw;
void calcul() {
  a = 0;
  b = 0;
  sign = 4;
  result = 0;
  thisval = 0;
  redraw();
  while (true) {
    up.tick();
    ok.tick();
    down.tick();
    data.tick();
    if (ok.hasClicks(3)) {
      oled.setScale(0);
      break;
    }
    if (ok.hasClicks(2)) {
      switch (sign) {
        case 0: result = (CALCUL_TYPE)a + b;         break;
        case 1: result = (CALCUL_TYPE)a - b;         break;
        case 2: result = (CALCUL_TYPE)a * b;         break;
        case 3: result = (CALCUL_TYPE)a / (b != 0 ? b : 1); break;
        case 4: result = _pow(a, b);                    break;
      }
      redraw();
    }
    if (ok.hold()) {
      if (++thisval >= 2) {
        thisval = 0;
      }
    }
    if (ok.hasClicks(1)) {
      if (++sign >= 5) sign = 0;
      redraw();
    }

    if (up.click()) {
      if (thisval) b++;
      else a++;
      redraw();
    }
    if (down.click()) {
      if (thisval) b--;
      else a--;
      redraw();
    }
    if (up.step()) {
      if (thisval) b += 10;
      else a += 10;
      redraw();
    }
    if (down.step()) {
      if (thisval) b -= 5;
      else a -= 5;
      redraw();
    }
    static uint32_t tmr;
    if (millis() - tmr >= 100) {
      tmr = millis();
      isdraw = !isdraw;
      redraw();
    }
    yield();
  }
  drawGameMenu();
}

void redraw() {
  oled.clear(); // чити
  oled.home();  // домой
  oled.setScale(1); // размер 1
  oled.print("КАЛЬКУЛЯТОР");  // надпись
  oled.setScale(0); // вернуть размер назад (костыль)
  checkBatteryCharge();   // Проверка напряжение аккума
  drawBatteryCharge();    // Рисуем индикатор
  oled.line(0, 10, 127, 10);   // Линия
  oled.setCursor(0, 3); // сдвинуть
  oled.setScale(2); // размер 2
  if (thisval || isdraw) oled.print(a);
  else for (int i = 0; i < getdgts(a); i++) oled.print(' ');
  switch (sign) {
    case 0: oled.print('+');  break;
    case 1: oled.print('-');  break;
    case 2: oled.print('*');  break;
    case 3: oled.print('/');  break;
    case 4: oled.print('^');  break;
  }
  if (!thisval || isdraw) oled.print(b);
  else for (int i = 0; i < getdgts(b); i++) oled.print(' ');
  oled.print('=');
  oled.print(result);
  oled.update();
}
int getdgts(int num) {
  int res = 1;
  int pow10 = 10;
  while (num / pow10) {
    pow10 *= 10;
    res++;
  }
  if (num < 0) res++;
  return res;
}

int64_t _pow(int64_t a, int64_t b) {
  int64_t res = 1;
  for (int i = 0; i < b; i++) {
    res*=(int64_t)a;
  }
  return res;
}
