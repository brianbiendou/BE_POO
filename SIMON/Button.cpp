//Bibliothèque
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "Adafruit_NeoPixel.h"
#include "rgb_lcd.h"
#include "Led.h"

#include "Button.h"
#include <Arduino.h>

Button::Button(int pin) : Device(pin), lastButtonState(HIGH) {
    pinMode(pin, INPUT_PULLUP); // Configure la broche en mode entrée avec pull-up
}

void Button::activate() {
    int state = digitalRead(pin);

    //Gestion des erreurs
    if (state != HIGH && state != LOW) {
        Serial.println("Erreur de lecture de l'état du bouton");
        return;
    }

    if (state == LOW) {
        // Le bouton est appuyé
        lastButtonState = LOW;
    }
}

bool Button::operator==(const Button& other) const {
    return this->pin == other.pin;
}

void Button::deactivate() {
    lastButtonState = HIGH; // Réinitialise l'état du bouton
}

bool Button::wasPressed() {
    bool wasPressed = (lastButtonState == LOW && digitalRead(pin) == HIGH);
    lastButtonState = digitalRead(pin); // Met à jour l'état du bouton
    return wasPressed;
}
