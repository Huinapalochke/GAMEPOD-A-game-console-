#include "GamePod_API.h"
extern Adafruit_SSD1306 lcd;

GamePodAPI GAMEPOD_API;
Game Games[MAX_GAMES];

int GameCount = 0;

Adafruit_SSD1306* GamePodAPI::GetDisplay() {
    return &lcd;
}

bool GamePodAPI::AddGame(const char* name, const char* description, void (*run)()) {
  if (GameCount >= MAX_GAMES || run == nullptr) {
    return false;
  }
  Games[GameCount] = {name, description, run};
  GameCount++;
  return true;
}

void GamePodAPI::ToggleSounds(bool BoolStatement) {
        Sounds = BoolStatement;
    }

bool GamePodAPI::isSoundsOn() {return Sounds;}

bool GamePodAPI::CheckButton(int PINTOCHECK) {
  if (digitalRead(PINTOCHECK) == LOW) {
    while (digitalRead(PINTOCHECK) == LOW) {
      // ниху... ничего не делаем
      // тут мы просто ждём пока он отпустит кнопку
    }
    return true;
  }
  return false;
}

void GamePodAPI::nop() {
  while (true) {
    if (CheckButton(LEFT_BUTTON) || CheckButton(OK_BUTTON) || CheckButton(RIGHT_BUTTON)) {
      break;
    }
  }
}

void GamePodAPI::Beep(int Freq, int Interval) {
  if (Sounds) {
    tone(SPEAKER_PIN, Freq);
    delay(Interval);
    noTone(SPEAKER_PIN);
  }
}

void GamePodAPI::ClsScr() {
  GetDisplay()->setCursor(0, 0);
  GetDisplay()->clearDisplay();
}