//Bibliothèque
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "rgb_lcd.h"
#include "Led.h"
#include "StartStopButton.h"

// Définitions de NUMPIXELS et PIN
#define NUMPIXELS   10
#define PIN         D3

const int ledPins  =   D1;
const int buttonPins = D2;


StartStopButton::StartStopButton(int buttonPin, int ledPin)
    : Device(buttonPin), isGameActive(false), ledPin(ledPin) {

    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    lastButtonState = HIGH;
}

void StartStopButton::update() {
    if (digitalRead(pin) == LOW && lastButtonState == HIGH) {
        // Changez l'état du jeu lors de l'appui sur le bouton
        isGameActive = !isGameActive;
        if (isGameActive) {
            digitalWrite(ledPin, HIGH); // Allume la LED si le jeu est actif
        } else {
            digitalWrite(ledPin, LOW); // Éteint la LED si le jeu est inactif
        }
        lastButtonState = LOW;
        delay(50); // Pour éviter le rebond du bouton
    } else if (digitalRead(pin) == HIGH) {
        lastButtonState = HIGH;
    }
}

bool StartStopButton::isActive() const {
    return isGameActive;
}

void StartStopButton::activate() {
    // Implémentation facultative, par exemple, allumer la LED
    digitalWrite(ledPin, HIGH);
}

void StartStopButton::deactivate() {
    // Implémentation facultative, par exemple, éteindre la LED
    digitalWrite(ledPin, LOW);
}
