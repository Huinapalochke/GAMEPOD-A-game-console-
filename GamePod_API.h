#pragma once

// Use auto* lcd = GAMEPOD_API.GetDisplay();
// to get the screen

// to do something with it just use it like that

// IMPORTANT: You need to call lcd->display() to show changes on the OLED.
// Drawing commands modify the display buffer, not the physical screen
// if you dont gonna use this command nothing will happen, use it like this :
// lcd->println("This text gonna be shown");
// lcd->println("This too");
// lcd->drawLine(30, 30, 40, 40, SSD1306_WHITE);
// lcd->display();
// lcd->println("This one not gonna be shown but gonna be stored in buffer (Memory)");
// lcd->println("Becuse it does not have a lcd->display() function after")

// lcd->print("text")
// Prints but not transfers you on to new line

// lcd->println("text")
// Just..... Prints line....

// lcd->draw...
// use drawLine, drawRect, drawPixel etc to draw a shape

// lcd->drawBitmap (i am REALLY dont remember how to write this command)
// draws bitmap.... reserch in the internet man... thats >2027 and you dont know what google is....

// thats all the basis.... i think

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
extern bool Sounds;

#define MAX_GAMES 10

// all the buttons correspond to pins they in

#define OK_BUTTON 3
#define LEFT_BUTTON 2
#define RIGHT_BUTTON 4
#define HOME_BUTTON 5
#define SPEAKER_PIN 6

// Freq for the GAMEPOD_API.Beep() command
// Use like this : GAMEPOD_API.Beep(STANDART_FREQ)
// Or other freq like EXIT : GAMEPOD_API.Beep(EXIT_FREQ)
// And that does not mean that you can't use your own
// freq : GAMEPOD_API.Beep(Your Freq)
// Like this : GAMEPOD_API.Beep(6500)
// Yeah, like that... *what am i doing with my life...*

#define STANDART_FREQ 900
#define SELECT_FREQ 400
#define GAMEOVER_FREQ 1200
#define EXIT_FREQ 50
#define ERROR_FREQ 1400

struct Game {
    const char* name;
    const char* description;
    void (*run)();
};

class GamePodAPI {
public:
    Adafruit_SSD1306* GetDisplay();

    // AddGame is the main command that you need!
    // use it like this : GAMEPOD_API.AddGame("Game name", "Game Description", function)
    bool AddGame(const char* name, const char* description, void (*run)());

    // Just... forget about it.
    void EndGame();

    // Use this for ON\OFF the sounds (Globally)
    void ToggleSounds(bool BoolStatement);

    // Are the sounds even ON?
    bool isSoundsOn();

    // ts is checking are button pressed
    // use like this : GAMEPOD_API.CheckButton(Button)
    // Example : GAMEPOD_API.CheckButton(OK_BUTTON) // Checks the middle one
    // Use this command in the loop becuse this command DOES NOT wait for the button
    // it just checks is the key pressed RIGHT IN THIS MOMENT!
    bool CheckButton(int PINTOCHECK);

    // Waits for any button
    void nop();

    // Sounds!!
    void Beep(int Freq, int Interval = 20);

    // Clear the screen and set the cursour to (0, 0)
    void ClsScr();
};

extern GamePodAPI GAMEPOD_API;
extern Game Games[MAX_GAMES];
extern int GameCount;

void RegisterAllGames();
