/* Написано Матвеем и Андреем Бородиными, потому что мы можем в Си. */
double display = 0;      // Число на экране
double accumulator = 0;  // Аккумулятор для +, -, *, /
int op = 0;              // Операции 1 +, 2 -, 3 *, 4 /
int button_x = 0;        // Выбранная кнопка по горизонтали
int button_y = 0;        // Выбранная кнопка по вертикали

void drawDisplay() {
  oled.clear();
  // Число из display (может нарисовать и аккумулятор с операций? места на экране ещё полно)
  oled.setCursor(1, 1);
  oled.print(display);

  // Рисуем клавиатуру
  oled.setCursor(5, 3);
  oled.print(F("C = + -"));
  oled.setCursor(5, 4);
  oled.print(F("1 2 3 *"));
  oled.setCursor(5, 5);
  oled.print(F("4 5 6 /"));
  oled.setCursor(5, 6);
  oled.print(F("7 8 9 0"));

  // Выбранная кнопка
  oled.roundRect(3 + button_y * 12, 22 + button_x * 8, 13 + button_y * 12, 32 + button_x * 8, OLED_STROKE);
  oled.update();
}

void okButtonClick() {
  // Ифы, ифчики, ифята
  if (button_x == 0) {
    if (button_y == 0) {
      // C
      display = 0;
    } else if (button_y == 1) {
      // =
      if (op == 1) {
        display = display + accumulator;
      }
      if (op == 2) {
        display = accumulator - display;
      }
      if (op == 3) {
        display = accumulator * display;
      }
      if (op == 4) {
        display = accumulator / display;
      }
      op = 0;
    } else if (button_y == 2) {
      // +
      accumulator = display;
      display = 0;
      op = 1;
    } else if (button_y == 3) {
      // -
      accumulator = display;
      display = 0;
      op = 2;
    }
  } else if (button_x == 1) {
    if (button_y == 0) {
      //1
      display = display * 10 + 1;
    } else if (button_y == 1) {
      //2
      display = display * 10 + 2;
    } else if (button_y == 2) {
      //3
      display = display * 10 + 3;
    } else if (button_y == 3) {
      //*
      accumulator = display;
      display = 0;
      op = 3;
    }

  } else if (button_x == 2) {
    if (button_y == 0) {
      //4
      display = display * 10 + 4;
    } else if (button_y == 1) {
      //5
      display = display * 10 + 5;
    } else if (button_y == 2) {
      //6
      display = display * 10 + 6;
    } else if (button_y == 3) {
      // /
      accumulator = display;
      display = 0;
      op = 4;
    }

  } else if (button_x == 3) {
    if (button_y == 0) {
      //7
      display = display * 10 + 7;
    } else if (button_y == 1) {
      //8
      display = display * 10 + 8;
    } else if (button_y == 2) {
      //9
      display = display * 10 + 9;
    } else if (button_y == 3) {
      // 0
      display = display * 10;
    }
  }
  // Элсы, элсики, элсята
}

void calc(void) {
  while (true) {
    oled.clear();
    oled.update();
    drawDisplay();
    while (true) {
      up.tick();
      ok.tick();
      down.tick();
      if (ok.click()) {
        okButtonClick();
      }

      // Передвижения кнопок
      if (down.click()) {
        button_x++;
        if (button_x >= 4)
          button_x = 0;
      }
      if (up.click()) {
        button_y++;
        if (button_y >= 4)
          button_y = 0;
      }

      if (down.click() || ok.click() || up.click())
        drawDisplay();

      if (up.hold()) {
        Wire.setClock(600E3);
        system_update_cpu_freq(80);
        drawMainMenu();
        return;
      }

      yield();
    }
  }
}
