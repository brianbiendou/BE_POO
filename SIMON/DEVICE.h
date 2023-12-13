#ifndef DEVICE_H
#define DEVICE_H

class Device {
protected:
    int pin; // Le numéro de la broche à laquelle le dispositif est connecté

public:
    Device(int pin); // Constructeur qui initialise la broche

    virtual void activate() = 0; // Méthode abstraite pour activer le dispositif
    virtual void deactivate() = 0; // Méthode abstraite pour désactiver le dispositif
};

#endif // DEVICE_H
