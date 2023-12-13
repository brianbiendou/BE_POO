#ifndef GAME_H
#define GAME_H

#include "Adafruit_NeoPixel.h"
#include "LED.h"
#include "Button.h"
#include "StartStopButton.h"
#include "rgb_lcd.h"
#include <vector>

class Game {
private:
    Adafruit_NeoPixel strip;
    std::vector<LED> leds;
    std::vector<Button> buttons;
    String currentCode;
    String playerInput;
    int currentLevel;
    StartStopButton startStopButtons;
    int delayTime;
    rgb_lcd lcd;

public:
    Game(int numPixels, int pin, int buttonPin, int ledPin);

    void setAllLedsColor(uint32_t color);
    void showSuccess();
    void showError();
    void run();

private:
    const std::vector<String> codes;

    void mapCodeToLedPairs(const String& code, std::vector<int>& ledPairIndices);
    void displaystart();
    void displayLevel();
    void selectNewCode();
    void playSequence();
    void waitForPlayerInput();
    void checkPlayerInput();
    void displaySuccess();
    void displayError();
    void increaseLevel();
};

#endif // GAME_H
