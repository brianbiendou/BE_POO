//Bibliothèque
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "rgb_lcd.h"
#include "Led.h"

LED::LED(Adafruit_NeoPixel& strip, int index, uint32_t color)
    : Device(strip.getPin()), strip(strip), index(index), color(color) {}

void LED::activate() {
    strip.setPixelColor(index, color); // Utiliser la couleur spécifiée
    strip.show();
}

void LED::deactivate() {
    strip.setPixelColor(index, strip.Color(0, 0, 0)); // Éteindre la LED
    strip.show();
}
