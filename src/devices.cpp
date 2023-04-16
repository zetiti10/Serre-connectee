/**
 * @file main.cpp
 * @author Louis L
 * @brief Fichier gérant les différents périphériques de la serre.
 * @version 1.0
 * @date 2023-04-13
 */

// Ajout des bibliothèques au programme.
#include <Arduino.h>
#include <Servo.h>

// Autres fichiers du programme.
#include <main.hpp>

// Fonctions et variables pour le servomoteur.
int roofOpeningServoPosition = 0;

void roofOpeningServoControl(int angle)
{
    roofOpeningServo.write(angle);
    roofOpeningServoPosition = angle;
}