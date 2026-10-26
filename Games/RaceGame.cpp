#include "GamePod_API.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int RacerPos;
int Walls = 8;
int WallPos[8] {};

int Score = 0;

void ShuffleNumbers() {
  RacerPos = 0;
  for (int i = 0; i < Walls - 1; i++) {
    WallPos[i] = random(0, Walls);
  }
}

void UpdateRace() {
  auto* lcd = GAMEPOD_API.GetDisplay();
  GAMEPOD_API.ClsScr();

  lcd->setCursor(20, RacerPos * 8);
  lcd->print(F(">"));
  
  lcd->setCursor(0, 0);
  lcd->print(Score);

  for (int i = 0; i < Walls; i++) {
    lcd->setCursor(50, WallPos[i] * 8);
    lcd->println(F("#"));
  }
  lcd->display();
}

void Race() {
  ShuffleNumbers();
  UpdateRace();
  while (true) {
    if (GAMEPOD_API.CheckButton(LEFT_BUTTON) && RacerPos > 0) {
      RacerPos--;
      UpdateRace();
	  GAMEPOD_API.Beep(SELECT_FREQ);
	}
    if (GAMEPOD_API.CheckButton(RIGHT_BUTTON) && RacerPos < Walls - 1) {
      RacerPos++;
      UpdateRace();
	  GAMEPOD_API.Beep(SELECT_FREQ);
    }
    if (GAMEPOD_API.CheckButton(OK_BUTTON)) {
      bool died = false;
      for (int i = 0; i < Walls; i++) {
        if (RacerPos == WallPos[i]) {
          died = true;
        }
      }
      if (died) {
        Score = 0;
		GAMEPOD_API.Beep(GAMEOVER_FREQ);
      }
	  else {
		Score++;
		GAMEPOD_API.Beep(STANDART_FREQ);
	  }
      ShuffleNumbers();
      UpdateRace();
    }
	if (GAMEPOD_API.CheckButton(HOME_BUTTON)) {
	  break;
	}
  }
  GAMEPOD_API.Beep(EXIT_FREQ);
}

void RegisterGame() {
  if (!GAMEPOD_API.AddGame("Race", "just raci'n", Race)) {
    return;
  }
}