#include "GamePod_API.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <EEPROM.h> i am too lazy to do ts man
int Clicks = 0;

void UpdateClicker() {
  auto* lcd = GAMEPOD_API.GetDisplay();
  GAMEPOD_API.ClsScr();
  lcd->println(Clicks);
  lcd->display();
}

void Clicker() {
  auto* lcd = GAMEPOD_API.GetDisplay();
  lcd->setTextSize(2);
  UpdateClicker();
  while (true) {
    if (GAMEPOD_API.CheckButton(OK_BUTTON)) {
      Clicks++;
      GAMEPOD_API.Beep(STANDART_FREQ);
      UpdateClicker();
    }
    if (GAMEPOD_API.CheckButton(HOME_BUTTON)) {
      lcd->setTextSize(1);
      GAMEPOD_API.Beep(EXIT_FREQ);
      break;
    }
  }
}

void RegisterGame() {
  if (!GAMEPOD_API.AddGame("Clicker", "Just click lmao", Clicker)) {
    return;
  }
}