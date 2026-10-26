Heres how to use our API

first you need to import it:
#include "GamePod_API.h"

then recommended to import some other stuff:
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

then you need to register the game, make a function that returns void
and name it RegisterGame, This function is like setup in Arduino
and there you need to write the following line of code:
GAMEPOD_API.AddGame("Name", "Description", function)

Where "name" enter the game name, where "Description" enter the game description
and where's function enter the main function of the game

Game code example:

#include "GamePod_API.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void MyOwnGameFunction() {
  Serial.println("This is my game!"); // if we run it were gonna print "This is my game!"
}

void RegisterGame() {
  GAMEPOD_API.AddGame("My own game!", "My game!", MyOwnGameFunction); // adding a new game onto the menu
}

This code gonna add a new game named as "My own game!"
and when you open it it gonna print "This is my game" using serial

Serial.begin() is already in the code (in file GAMEPOD.ino)
so you dont need to add it (it works in 9600 btw)

now lets draw something on screen and gonna try to
check the button press:

#include "GamePod_API.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void MyOwnGameFunction() {
  GAMEPOD_API.ClsScr(); // clear the screen
  while (true) { // starting the loop
    if (GAMEPOD_API.CheckButton(OK_BUTTON)) { // if the button OK (the middle one) gonna be pressed in this moment
      Serial.println("This is my game!"); // were gonna print a "This is my game"
    }

    if (GAMEPOD_API.CheckButton(LEFT_BUTTON)) {
      GAMEPOD_API.ClsScr(); // clearing the screen
      auto* lcd = GAMEPOD_API.GetDisplay(); // Getting our screen
      lcd->drawPixel(random(0, 20), random(0, 20), SSD1306_WHITE); // drawing a white pixel on random coordinate
      lcd->display(); // DONT FORGET TO ADD THIS LINE!
    }

    if (GAMEPOD_API.CheckButton(HOME_BUTTON)) { // if we gonna try to exit from the game
      GAMEPOD_API.Beep(EXIT_FREQ); // were gonna play a sound
      break; // and end the loop
    }
  }
  // after this line the game is over
  // because the function is over
}

void RegisterGame() {
  GAMEPOD_API.AddGame("My own game!", "My game!", MyOwnGameFunction); // adding our game
}

now if we press the left button then a pixel gonna be displayed
if we press the middle one then were gonna print a text in console
and if we press the home button were gonna exit from the game

i think thats all you need
if you need to print the text on the screen or draw a rectangle (not pixel) then
view the documentation of Adafruit_SSD1306

AND DONT MAKE THE auto* lcd = GAMEPOD_API.GetDisplay(); A GLOBAL VARIABLE
I WILL GONNA CAUSE A ERROR
(if you gonna fix it send the code to me to get a big nothing btw)