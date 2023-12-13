#ifndef LED_H
#define LED_H

#include "DEVICE.h" // Inclure le fichier d'en-tête de la classe de base

class LED : public Device {
private:
    Adafruit_NeoPixel& strip; // Référence à la bande de LEDs NeoPixel
    int index; // Index de la LED sur la bande
    uint32_t color; // Couleur de la LED

public:
    LED(Adafruit_NeoPixel& strip, int index, uint32_t color);

    void activate() override;
    void deactivate() override;
};

#endif // LED_H
