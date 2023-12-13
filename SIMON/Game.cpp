#include "Game.h"
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"
#include "LED.h"
#include "Button.h"
#include "Device.h"
#include "StartStopButton.h"
#include "rgb_lcd.h"
#include <vector>

Game::Game(int numPixels, int pin, int buttonPin, int ledPin)
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ800), currentLevel(1), startStopButtons(buttonPin, ledPin), delayTime(1000), lcd(),
      codes({"0132", "0231", "0213", "0321", "0312", "1320", "1302", "1230", "1203", "1023"}) {

    

    // Initialisation des Leds
    strip.begin();
    strip.setBrightness(100);

    // Définir les couleurs pour chaque binôme
    const uint32_t colors[] = {strip.Color(0, 255, 0), strip.Color(255, 0, 0), strip.Color(0, 0, 255), strip.Color(255, 255, 0)};


    // Liste des codes possibles
    // Liste des codes possibles (sans spécification de type)
    const std::vector<String> codes = {"0132", "0231", "0213", "0321", "0312", "1320", "1302", "1230", "1203", "1023"};


    // Initialisation des LEDs avec les couleurs
    for (int i = 0; i < strip.numPixels(); ++i) {
        int colorIndex = i / 2;
        leds.push_back(LED(strip, i, colors[colorIndex % 4]));
    }


    // Initialisation des boutons
    const int buttonPins[] = {D5, D6, D7, D8};
    for (int pin : buttonPins) {
        buttons.push_back(Button(pin));
    }
}


//Modifie toutes les LEDS
void Game::setAllLedsColor(uint32_t color) {
    for (int i = 0; i < strip.numPixels(); ++i) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}


// Affiche la couleur pendant une seconde toutes en vert
void Game::showSuccess() {
    setAllLedsColor(strip.Color(0, 255, 0));
    delay(1000);
    setAllLedsColor(strip.Color(0, 0, 0));
}


// Affiche la couleur pendant une seconde toutes en ROUGE
void Game::showError() {
    setAllLedsColor(strip.Color(255, 0, 0));
    delay(1000);
    setAllLedsColor(strip.Color(0, 0, 0));
}

void Game::run() {

    startStopButtons.update();
    delay(100);

    if (startStopButtons.isActive()) {
        displaystart();

        selectNewCode();

        playSequence();

        waitForPlayerInput();

        checkPlayerInput();

        displayLevel();

        // On rinitialise playerInput pour le prochain tour
        playerInput = "";
        delay(2000);
    } else {

        // Éteindre toutes les LEDs
        for (LED& led : leds) {
            led.deactivate();
        }

        // Réinitialiser l'état du jeu
        playerInput = "";
    }
}

// Ajout d'une méthode pour mapper les chiffres aux paires de LEDs
void Game::mapCodeToLedPairs(const String& code, std::vector<int>& ledPairIndices) {
    for (char c : code) {
        switch (c) {
            case '0':
                ledPairIndices.push_back(0);
                break;
            case '1':
                ledPairIndices.push_back(1);
                break;
            case '2':
                ledPairIndices.push_back(2);
                break;
            case '3':
                ledPairIndices.push_back(3);
                break;
        }
    }
}

//afiche message de debut sur l'ecran
void Game::displaystart() {
    lcd.begin(16, 2);
    lcd.setRGB(128, 128, 128);
    lcd.print("Welcome to the ");
    lcd.setCursor(0, 1);
    lcd.print("GAME !! Level ");
    lcd.print(currentLevel);
}

void Game::displayLevel() {
    lcd.clear();
    Serial.println("avant");
    lcd.setCursor(0, 1);
    lcd.print("Level: ");
    lcd.print(currentLevel);
    Serial.println("apres");
    int remainingSpaces = 16 - 7 - String(currentLevel).length();
    for (int i = 0; i < remainingSpaces; i++) {
        lcd.print(" ");
    }
}

void Game::selectNewCode() {

    //Gestion des erreurs si l'index est dans la limite du tableau
    size_t index = random(0, codes.size());
    if (index >= codes.size()) {
        Serial.println("Index de code invalide");
        return;
    }

    String selectedCode = codes[index];

    // Mapper le code sélectionné aux indices des paires de LEDs
    std::vector<int> ledPairIndices;
    mapCodeToLedPairs(selectedCode, ledPairIndices);

    currentCode = "";// Réinitialiser l'entrée du joueur
    for (int index : ledPairIndices) {
        currentCode += String(index);
    }
}

void Game::playSequence() {

    // Paires de LEDs à allumer
    const int ledPairs[4][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};
    for (char c : currentCode) {
        int pairIndex = c - '0';// Convertir le caractère en index de paire
        // Vérifier si l'index de la paire est valide
        if (pairIndex >= 0 && pairIndex < 4) {
            // Allumer les LEDs de la paire courante
            leds[ledPairs[pairIndex][0]].activate();
            leds[ledPairs[pairIndex][1]].activate();
            delay(500); // Temps pendant lequel les LEDs restent allumées

            // Éteindre les LEDs de la paire courante
            leds[ledPairs[pairIndex][0]].deactivate();
            leds[ledPairs[pairIndex][1]].deactivate();
            delay(delayTime);// Temps d'attente entre chaque paire de LED
        }
    }
}

void Game::waitForPlayerInput() {
        // Paires de LEDs correspondant aux boutons
        const int ledPairs[4][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};

        while (playerInput.length() < currentCode.length()) {
            for (size_t i = 0; i < buttons.size(); ++i) {

                buttons[i].activate();
                if (buttons[i].wasPressed()) {
                    playerInput += String(i); // Ajouter l'indice du bouton à playerInput

                    // Activer les LEDs du binôme correspondant pour le feedback visuel
                    leds[ledPairs[i][0]].activate();
                    leds[ledPairs[i][1]].activate();
                    delay(500); // Durée du feedback visuel

                    // Éteindre les LEDs du binôme
                    leds[ledPairs[i][0]].deactivate();
                    leds[ledPairs[i][1]].deactivate();
                    delay(10);
                }
            }
            yield();
        }
}

void Game::checkPlayerInput() {
    if (playerInput == currentCode) {
        Serial.println("Correct!");
        showSuccess(); // Affiche toutes les LEDs en vert
        lcd.clear();
        displaySuccess();
        increaseLevel();
    } else {
        Serial.println("Incorrect, try again!");
        showError(); // Affiche toutes les LEDs en rouge
        lcd.clear();
        displayError();
    }
    // Réinitialiser playerInput pour le prochain tour
    playerInput = "";
}

// Méthode pour afficher un message de succès
void Game::displaySuccess() {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Correct! Well done!");
}

// Méthode pour afficher un message d'erreur
void Game::displayError() {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Incorrect! Try again!");
}

void Game::increaseLevel() {
    currentLevel++;
    Serial.println("niveau incrémenté"); 
    delayTime = max(100, delayTime - 100); // Réduire le temps de retard pour augmenter la difficulté
}
