#ifndef STARTSTOPBUTTON_H
#define STARTSTOPBUTTON_H

#include "DEVICE.h" // Inclure le fichier d'en-tête de la classe de base

class StartStopButton : public Device {
private:
    bool isGameActive; // État actif ou non du jeu
    const int ledPin; // La broche de la LED du bouton
    bool lastButtonState;

public:
    StartStopButton(int buttonPin, int ledPin);

    void update();
    bool isActive() const;

    void activate() override;
    void deactivate() override;
};

#endif // STARTSTOPBUTTON_H
