#ifndef BUTTON_H
#define BUTTON_H

#include "DEVICE.h" // Inclure le fichier d'en-tête de la classe de base

class Button : public Device {
private:
    bool lastButtonState; // Dernier état connu du bouton

public:
    Button(int pin); // Constructeur

    void activate() override;
    void deactivate() override;
    bool wasPressed();

    bool operator==(const Button& other) const; // Surcharge de l'opérateur ==
};

#endif // BUTTON_H
