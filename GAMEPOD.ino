#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "GamePod_API.h"
#include <EEPROM.h>

bool DeveloperMode = false; // не ставь на true, лучше просто подключи пин 12 к GND! (Либо к минусу)
bool BetaFeatures = false; // не ставь на true, лучше просто включи в настройках!
// P.S. если я такой далбаёб забуду добавить BetaFeatures - Можете называться меня далбаёбом, ебланом, кончито великий, и т.д..... гы)

//                   RS, E, D4, D5, D6, D7
// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Если чесно я не хочю это удалять т.к. у меня этот пример (как инициализировать екранчик) есть только в этом файле поэтому похуй, и так сойдёт
// А ты можешь удалить))))

Adafruit_SSD1306 lcd(128, 64, &Wire, -1, 100000, 100000);

int Selected_Choose = 0;

bool Sounds = true;

void setup() {
  // Инициализация экрана и кнопок

  RegisterAllGames();
  Serial.begin(9600);
  Serial.print("games found: ");
  Serial.println(GameCount);
  
  if (!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {}
  }
  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(HOME_BUTTON, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  Prepare_Device();
}



int SelectedSetting = 0;
int MaxSettingIndex = 2;
int SettingValueOffset = 110;

void PrintSetting(const __FlashStringHelper* Setting, int SettingIndex, bool SettingValue) {
  lcd.setCursor(12, SettingIndex * 8);
  if (SelectedSetting == SettingIndex) {
    lcd.print(F("| "));
  } else {
    lcd.print(F("  "));
  }
  lcd.print(Setting);
  lcd.setCursor(SettingValueOffset + 2, SettingIndex * 8);
  lcd.print(SettingValue);
  lcd.display();
}

void UpdateTestSettings() {
  GAMEPOD_API.ClsScr();
  lcd.drawLine(SettingValueOffset, 0, SettingValueOffset, 64, SSD1306_WHITE);
  PrintSetting(F("Back"), 0, false);
  PrintSetting(F("Test Buttons"), 1, false);
  PrintSetting(F("Developer Mode"), 2, DeveloperMode);
}

void TestModeSettings() {
  SelectedSetting = 0;
  GAMEPOD_API.ClsScr();
  UpdateTestSettings();
  while (true) {
    if (GAMEPOD_API.CheckButton(RIGHT_BUTTON)) {
      SelectedSetting += 1;
      UpdateTestSettings();
      GAMEPOD_API.Beep(400);
    } else if (GAMEPOD_API.CheckButton(LEFT_BUTTON)) {
      SelectedSetting -= 1;
      UpdateTestSettings();
      GAMEPOD_API.Beep(400);
    } else if (GAMEPOD_API.CheckButton(OK_BUTTON)) {
      if (SelectedSetting == 0) {
        break;
      }
      if (SelectedSetting == 1) {
        GAMEPOD_API.ClsScr();
        lcd.setTextSize(1);
        lcd.println(F("Press"));
        lcd.setTextSize(3);
        lcd.println(F("LEFT"));
        lcd.display();
        while (!GAMEPOD_API.CheckButton(LEFT_BUTTON)) {}
        GAMEPOD_API.ClsScr();
        lcd.setTextSize(1);
        lcd.println(F("Press"));
        lcd.setTextSize(3);
        lcd.println(F("OK"));
        lcd.display();
        while (!GAMEPOD_API.CheckButton(OK_BUTTON)) {}
        GAMEPOD_API.ClsScr();
        lcd.setTextSize(1);
        lcd.println(F("Press"));
        lcd.setTextSize(3);
        lcd.println(F("RIGHT"));
        lcd.display();
        while (!GAMEPOD_API.CheckButton(RIGHT_BUTTON)) {}
        GAMEPOD_API.ClsScr();
        lcd.setTextSize(1);
        lcd.println(F("Press"));
        lcd.setTextSize(3);
        lcd.println(F("HOME"));
        lcd.display();
        while (!GAMEPOD_API.CheckButton(HOME_BUTTON)) {}
        lcd.setTextSize(1);
        UpdateTestSettings();
      }
      if (SelectedSetting == 2) {
        GAMEPOD_API.ClsScr();
        lcd.print(F("To activate this you\nneed to connect a GND\nto pin 12 on startup!"));
        lcd.display();

        GAMEPOD_API.Beep(1200, 200);
        delay(50);
        GAMEPOD_API.Beep(1200, 200);

        GAMEPOD_API.nop();
        UpdateTestSettings();
      }
    }
  }
}

void UpdateSettings() {
  if (SelectedSetting > MaxSettingIndex) SelectedSetting = MaxSettingIndex;
  if (SelectedSetting < 0) SelectedSetting = 0;

  GAMEPOD_API.ClsScr();
  lcd.drawLine(SettingValueOffset, 0, SettingValueOffset, 64, SSD1306_WHITE);

  PrintSetting(F("Sounds"), 0, GAMEPOD_API.isSoundsOn());
  PrintSetting(F("Game Save"), 1, false);
  PrintSetting(F("Test Mode"), 2, false);

  lcd.display();
}

void Settings() {
  UpdateSettings();
  GAMEPOD_API.Beep(700);
  while (true) {
    if (GAMEPOD_API.CheckButton(RIGHT_BUTTON)) {
      SelectedSetting += 1;
      UpdateSettings();
      GAMEPOD_API.Beep(400);
    } else if (GAMEPOD_API.CheckButton(LEFT_BUTTON)) {
      SelectedSetting -= 1;
      UpdateSettings();
      GAMEPOD_API.Beep(400);
    } else if (GAMEPOD_API.CheckButton(OK_BUTTON)) {
      if (SelectedSetting == 0) GAMEPOD_API.ToggleSounds(!GAMEPOD_API.isSoundsOn());
      GAMEPOD_API.Beep(700); // я его не помещяю в начало потому что если пользователь выключит звуки - этот проиграеться всё ровно
      if (SelectedSetting == 1) {
        GAMEPOD_API.Beep(ERROR_FREQ);
      }
      if (SelectedSetting == 2) TestModeSettings();
      UpdateSettings();
    } else if (GAMEPOD_API.CheckButton(HOME_BUTTON)) {
      GAMEPOD_API.Beep(700);
      UpdChoose();
      break;
    }
  }
}

void Prepare_Device() {

  GAMEPOD_API.Beep(700);

  lcd.setTextSize(1);
  lcd.setTextColor(SSD1306_WHITE);
  lcd.setCursor(0, 0);

  if (digitalRead(12) == LOW) {
    DeveloperMode = true;
    GAMEPOD_API.ClsScr();
    lcd.setTextSize(2);
    lcd.println(F("ATTENTION!"));
    lcd.drawLine(0, 16, 128, 16, SSD1306_WHITE);
    lcd.setTextSize(1);
    lcd.setCursor(0, 18);
    lcd.print(F("Pin 12 is connected\nto GND!\n\n\n'Test mode' Active!"));
    lcd.display();
    GAMEPOD_API.Beep(1200, 200);
    delay(50);
    GAMEPOD_API.Beep(1200, 200);
    GAMEPOD_API.nop();
  }

  GAMEPOD_API.ClsScr();
  
  lcd.setTextSize(2);
  lcd.println(F("License"));
  lcd.drawLine(0, 16, 128, 16, SSD1306_WHITE);
  lcd.setTextSize(1);
  lcd.setCursor(0, 18);
  lcd.print(F("This software is\nunder M.I.T License!"));
  // Details here
  // https://opensource.org/license/mit
  lcd.display();
  
  GAMEPOD_API.Beep(1200, 200);
  delay(50);
  GAMEPOD_API.Beep(1200, 200);

  GAMEPOD_API.nop();

  GAMEPOD_API.ClsScr();
  
  lcd.setTextSize(2);
  lcd.println(F("Developers"));
  lcd.drawLine(0, 16, 128, 16, SSD1306_WHITE);
  lcd.setTextSize(1);
  lcd.setCursor(0, 18);
  lcd.print(F("This software is\ncreated by Goreboxer_\n\nDONT CLAIM IT\nTO YOURSELF!"));
  lcd.display();
  
  GAMEPOD_API.Beep(1200, 200);
  delay(50);
  GAMEPOD_API.Beep(1200, 200);

  GAMEPOD_API.nop();

  GAMEPOD_API.Beep(400);

  GAMEPOD_API.ClsScr();

  lcd.println(F(" Wellcome to GAMEPOD!"));
  lcd.fillRect(20, 20, 80, 14, SSD1306_WHITE);
  lcd.setTextColor(SSD1306_BLACK);
  lcd.setCursor(24, 24);
  lcd.print(F("Start"));

  lcd.display();

  lcd.setTextColor(SSD1306_WHITE);
  lcd.setTextSize(1);
  
  while (true) {
    if (GAMEPOD_API.CheckButton(OK_BUTTON)) break;
  }

  Game_Menu();
}

void UpdChoose() {
  if (Selected_Choose > GameCount - 1) {
    Selected_Choose = GameCount - 1;
  } else if (Selected_Choose < 0) {
    Selected_Choose = 0;
  }

  GAMEPOD_API.ClsScr();
  lcd.setTextColor(SSD1306_WHITE);

  lcd.setCursor(0, 56);
  lcd.println(Games[Selected_Choose].name);

  lcd.setCursor(0, 48);
  lcd.print(F("#"));
  lcd.println(Selected_Choose + 1);

  lcd.drawLine(0, 45, 128, 45, SSD1306_WHITE);

  lcd.setCursor(0, 0);
  lcd.print(Games[Selected_Choose].description);

  lcd.display();
}

void Game_Menu() {
  UpdChoose();
  while (true) {
    if (GAMEPOD_API.CheckButton(LEFT_BUTTON)) {
      Selected_Choose -= 1;
      GAMEPOD_API.Beep(500);
      UpdChoose();
    } else if (GAMEPOD_API.CheckButton(RIGHT_BUTTON)) {
      Selected_Choose += 1;
      GAMEPOD_API.Beep(500);
      UpdChoose();
    }

    if (GAMEPOD_API.CheckButton(OK_BUTTON)) {
      GAMEPOD_API.Beep(2000);
      Games[Selected_Choose].run();
      UpdChoose();
    } else if (GAMEPOD_API.CheckButton(HOME_BUTTON)) {
      Settings();
    }
  }
}

void loop() {}
