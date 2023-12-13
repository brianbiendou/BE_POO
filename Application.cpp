/*********************************************************************
 * @file  Apllication.cpp
 * @author <mettre l'adresse mail ou nom prenom>
 * @brief Fichier source de l'application
 *********************************************************************/
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include "Application.h"
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "rgb_lcd.h"


Application::Application()
    : game(NUMPIXELS, PIN, buttonPins, ledPins) // Initialisation de l'objet Game avec les paramètres
{
    // Initialisez ici les variables d'état ou les configurations qui ne dépendent pas de l'environnement Arduino.
    // Par exemple, définir des états de jeu initiaux, des niveaux, etc.
    // Mais gardez à l'esprit que la plupart des initialisations matérielles doivent être faites dans 'init()' et non ici.
}

  
Application::~Application()
{
  // Code
  ;
}  

void Application::init(void)
{
  // Code
    ;
}


void Application::run(void)
{
  // Code
    ;
}
