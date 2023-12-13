#include <Adafruit_NeoPixel.h>
#include "Game.h" 

#define NUMPIXELS 10 // Définissez le nombre de pixels
#define PIN D3 // Définissez le numéro de la broche pour NeoPixel
const int ledPins  =   D1;
const int buttonPin = D2;


Game game(NUMPIXELS, PIN, buttonPin, ledPins); // Créez une instance de la classe Game

void setup() {
    Serial.begin(115200); // Initialisez la communication série à 9600 bauds
}

void loop() {
    game.run(); // Exécutez la logique du jeu
}
