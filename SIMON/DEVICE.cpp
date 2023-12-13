//Bibliothèque
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "rgb_lcd.h"
#include "Device.h"

Device::Device(int pin) : pin(pin) {} // Constructeur qui initialise la broche
