//Bibliothèque
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "rgb_lcd.h"


using namespace std;

// Définitions de NUMPIXELS et PIN
#define NUMPIXELS   10
#define PIN         D3

const int ledPins  =   D1;
const int buttonPins = D2;


// Classe de base pour les dispositifs
class Device {
protected:
    int pin; // Le numéro de la broche à laquelle le dispositif est connecté

public:
    Device(int pin) : pin(pin) {} // Constructeur qui initialise la broche

    virtual void activate() = 0; // Méthode abstraite pour activer le dispositif
    virtual void deactivate() = 0; // Méthode abstraite pour désactiver le dispositif
};

//CLASS POUR LES LEDS
class LED : public Device {
private:
    Adafruit_NeoPixel& strip; // Référence à la bande de LEDs NeoPixel
    int index; // Index de la LED sur la bande
    uint32_t color; // Couleur de la LED

public:
    // Ajouter un paramètre pour la couleur dans le constructeur
    LED(Adafruit_NeoPixel& strip, int index, uint32_t color) : Device(strip.getPin()), strip(strip), index(index), color(color) {}

    void activate() override {
        strip.setPixelColor(index, color); // Utiliser la couleur spécifiée
        strip.show();
    }

    void deactivate() override {
        strip.setPixelColor(index, strip.Color(0, 0, 0)); // Éteindre la LED
        strip.show();
    }
};


//CLASS POUR UN SIMPLE BOUTON POUSSOIR
class Button : public Device {
private:
    bool lastButtonState; // Dernier état connu du bouton

public:
    // Constructeur qui initialise la broche du bouton
    Button(int pin) : Device(pin), lastButtonState(HIGH) {
        pinMode(pin, INPUT_PULLUP); // Configure la broche en mode entrée avec pull-up
    }

    // Surcharge de la méthode activate pour vérifier si le bouton est appuyé
    void activate() override {

        //Gestion des erreurs
        int state = digitalRead(pin);
        if (state != HIGH && state != LOW) {
            Serial.println("Erreur de lecture de l'état du bouton");
            return;
        }

        if (digitalRead(pin) == LOW) {
            // Le bouton est appuyé
            lastButtonState = LOW;
        }
    }

    // Redéfinition de l'opérateur ==
    bool operator==(const Button& other) const {
        return this->pin == other.pin;
    }

    // Surcharge de la méthode deactivate pour réinitialiser l'état du bouton
    void deactivate() override {
        lastButtonState = HIGH; // Réinitialise l'état du bouton
    }

    // Méthode pour vérifier si le bouton a été appuyé depuis la dernière vérification
    bool wasPressed() {
        bool wasPressed = (lastButtonState == LOW && digitalRead(pin) == HIGH);
        lastButtonState = digitalRead(pin); // Met à jour l'état du bouton
        return wasPressed;
    }
};

//CLASSE POUR LE BOUTON POUSSOIR LED
class StartStopButton : public Device {
private:
    bool isGameActive; // État actif ou non du jeu
    const int ledPin; // La broche de la LED du bouton
    bool lastButtonState;

public:
    StartStopButton(int buttonPin, int ledPin) : Device(buttonPin), isGameActive(false), ledPin(ledPin) {

        pinMode(buttonPin, INPUT_PULLUP);
        pinMode(ledPin, OUTPUT);
    }

    void update() {
    if (digitalRead(pin) == LOW) {
        // Changez l'état du jeu lors de l'appui sur le bouton
        isGameActive = !isGameActive;
        if (isGameActive) {
            digitalWrite(ledPin, HIGH); // Allume la LED si le jeu est actif
            lastButtonState = LOW;
        } else {
            digitalWrite(ledPin, LOW); // Éteint la LED si le jeu est inactif
            lastButtonState = HIGH;
        }
        delay(50); // Pour éviter le rebond du bouton
      }
    }

      bool isActive() const {
          return isGameActive;
      }

      void activate() override {
        // Implémentation facultative, par exemple, allumer la LED
        digitalWrite(ledPin, HIGH);
      }

      void deactivate() override {
        // Implémentation facultative, par exemple, éteindre la LED
        digitalWrite(ledPin, LOW);
      }

};



//classe GAME
class Game {
    Adafruit_NeoPixel strip; // Déplacer la déclaration de strip ici
    std::vector<LED> leds; //un tableau de LED
    std::vector<Button> buttons; //un tableau de BOUTON
    String currentCode; // Code actuel à deviner
    String playerInput; // Entrée du joueur
    int currentLevel; 
    StartStopButton startStopButtons; // pour le bouton poussoir LED
    int delayTime; // Temps de retard entre les actions
    rgb_lcd lcd; // Objet pour l'écran LCD



    // Liste des codes possibles
    std::vector<String> codes = {"0132", "0231", "0213", "0321", "0312",
                                 "1320", "1302", "1230", "1203", "1023"};

public:
    //constructeur de la classe GAME
    Game(int numPixels, int pin, int buttonPin, int ledPin) : strip(numPixels, pin, NEO_GRB + NEO_KHZ800), currentLevel(1), startStopButtons(buttonPin, ledPin), delayTime(1000), lcd() {
        // Initialisation de l'écran LCD
        

        strip.begin();
        strip.setBrightness(100);

        // Définir les couleurs pour chaque binôme
        const uint32_t colors[] = {strip.Color(0, 255, 0), strip.Color(255, 0, 0), strip.Color(0, 0, 255), strip.Color(255, 255, 0)};


        // Initialisation des LEDs avec les couleurs
        for (int i = 0; i < strip.numPixels(); ++i) {
            int colorIndex = i / 2; // Chaque binôme a la même couleur
            leds.push_back(LED(strip, i, colors[colorIndex % 4]));
        }

        // Initialisation des boutons
        const int buttonPins[] = {D5, D6, D7, D8};
        for (int pin : buttonPins) {
            buttons.push_back(Button(pin));
        }
    }

    //Modifie toutes les LEDS
    void setAllLedsColor(uint32_t color) {
    for (int i = 0; i < strip.numPixels(); ++i) {
        strip.setPixelColor(i, color);
    }
    strip.show();
    }

    void showSuccess() {
        setAllLedsColor(strip.Color(0, 255, 0)); // Vert pour succès
        delay(1000); // Affiche la couleur pendant une seconde
        setAllLedsColor(strip.Color(0, 0, 0)); // Éteindre toutes les LEDs
    }

    void showError() {
        setAllLedsColor(strip.Color(255, 0, 0)); // Rouge pour erreur
        delay(1000); // Affiche la couleur pendant une seconde
        setAllLedsColor(strip.Color(0, 0, 0)); // Éteindre toutes les LEDs

    }


    void run() {

            startStopButtons.update();
            delay(100);

            if(startStopButtons.isActive()){
                displaystart();
                
                selectNewCode();
                //displayLevel();
                playSequence();
                
                waitForPlayerInput();
                
                checkPlayerInput(); 

                // On rinitialise playerInput pour le prochain tour
                playerInput = "";
                delay(4000); // Délai de 4 secondes 

            } else {

                // Éteindre toutes les LEDs
                for (LED& led : leds) {
                    led.deactivate();
                }

                // Éteindre la LED du bouton poussoir
                startStopButtons.deactivate();

                // Réinitialiser l'état du jeu
                playerInput = ""; // Réinitialiser l'entrée du joueur
                currentLevel = 1; // Réinitialiser le niveau
                
            }
            
    }

private:
        // Ajout d'une méthode pour mapper les chiffres aux paires de LEDs
    void mapCodeToLedPairs(const String& code, std::vector<int>& ledPairIndices) {
        for (char c : code) {
            switch (c) {
                case '0':
                    ledPairIndices.push_back(0); // Binôme (0 et 1)
                    break;
                case '1':
                    ledPairIndices.push_back(1); // Binôme (2 et 3)
                    break;
                case '2':
                    ledPairIndices.push_back(2); // Binôme (4 et 5)
                    break;
                case '3':
                    ledPairIndices.push_back(3); // Binôme (6 et 7)
                    break;
            }
        }
    }
    void displaystart(){
            lcd.begin(16, 2);
            lcd.setRGB(128, 128, 128);
            lcd.print("Welcome to the "); 
            lcd.setCursor(0, 1);
            lcd.print("GAME !! Level ");
            lcd.print(currentLevel);
    }

    void displayLevel() {
        lcd.clear();
        Serial.println("avant");
        lcd.setCursor(0, 1); // Positionne le curseur sur la deuxième ligne
        lcd.print("Level: ");
        lcd.print(currentLevel);
        Serial.println("apres");
        // ON compléte le reste de la ligne avec des espaces
        int remainingSpaces = 16 - 7 - String(currentLevel).length(); // 16 caractères au total, "Level: " prend 7 caractères
        for (int i = 0; i < remainingSpaces; i++) {
            lcd.print(" ");
        }
    }

    // Modification de selectNewCode
    void selectNewCode() {

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

        currentCode = ""; // Réinitialiser l'entrée du joueur
        for (int index : ledPairIndices) {
            currentCode += String(index);
        }
    }


    void playSequence() {
        // Paires de LEDs à allumer
        const int ledPairs[4][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};

        for (char c : currentCode) {
            int pairIndex = c - '0'; // Convertir le caractère en index de paire

            // Vérifier si l'index de la paire est valide
            if (pairIndex >= 0 && pairIndex < 4) {
                // Allumer les LEDs de la paire courante
                leds[ledPairs[pairIndex][0]].activate();
                leds[ledPairs[pairIndex][1]].activate();
                delay(500); // Temps pendant lequel les LEDs restent allumées

                // Éteindre les LEDs de la paire courante
                leds[ledPairs[pairIndex][0]].deactivate();
                leds[ledPairs[pairIndex][1]].deactivate();
                delay(delayTime); // Temps d'attente entre chaque paire de LEDs
            }
        }
    }


    void waitForPlayerInput() {
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


    void checkPlayerInput() {
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
    void displaySuccess() {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Correct! Well done!");
    }

    // Méthode pour afficher un message d'erreur
    void displayError() {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("Incorrect! Try again!");
    }


    void increaseLevel() {
        currentLevel++;
        Serial.println("niveau incrémenté")
        delayTime = max(100, delayTime - 100); // Réduire le temps de retard pour augmenter la difficulté
        // Optionnel : ajouter d'autres logiques pour augmenter la difficulté
    }
};


Game game(NUMPIXELS, PIN, buttonPins, ledPins); // Crée une instance de la classe Game

void setup() {
    Serial.begin(115200); // Initialise la communication série à 9600 bauds
}

void loop() {
    game.run(); // Exécute la logique du jeu
}
