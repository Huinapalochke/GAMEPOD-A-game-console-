#include "GamePod_API.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int WallWidth = 5;
int Iterations = 0;
int DotSpeed = 2;
int DotPos = 0;
int Combo = 0;

void UpdateTiming() {
  auto* lcd = GAMEPOD_API.GetDisplay();
  GAMEPOD_API.ClsScr();
  if (Combo > 0) {
    lcd->print(Combo);
  }
  lcd->drawPixel(DotPos, 32, SSD1306_WHITE);
  lcd->drawRect(64-WallWidth, 0, WallWidth * 2, 64, SSD1306_WHITE);
  lcd->display();
}

void CatchTiming() {
  // DotPosOld = 0;
  DotPos = 0;
  Iterations = 0;
  UpdateTiming();
  while (true) {

    if (Iterations >= 30) {
      Iterations = 0;
      DotPos += DotSpeed;
      UpdateTiming();
    }

    if (DotPos > 128) {
      DotPos = 1;
    }

    Iterations++;
    if (GAMEPOD_API.CheckButton(HOME_BUTTON)) {
      GAMEPOD_API.Beep(EXIT_FREQ);
      break;
    } else if (GAMEPOD_API.CheckButton(OK_BUTTON)) {
      if (DotPos >= 64-WallWidth+1 && DotPos <= (64-WallWidth)+(WallWidth*2)-1) {
        GAMEPOD_API.Beep(700);
        DotPos = 0;
        Combo++;
        DotSpeed = random(1, 5);
        WallWidth = random(round(DotSpeed), round(DotSpeed+3));
	  } else {
        DotPos = 0;
        Combo = 0;
      }
    }
  }
}

void RegisterGame() {
  GAMEPOD_API.AddGame("Catch Timing!", "Try to press a key\nin the right timing!", CatchTiming);
}
