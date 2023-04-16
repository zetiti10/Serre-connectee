/**
 * @file main.cpp
 * @author Louis L
 * @brief Fichier qui gère l'écran LCD de la serre connectée.
 * @version 1.0
 * @date 2023-04-13
 */

// Ajout des bibliothèques au programme.
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>

// Autres fichiers du programme.
#include <main.hpp>
#include <devices.hpp>
#include <pinDefinitions.hpp>

// Variable du système d'affichage de messages spéciaux.
int displayElementTimer = 0;

// Variables du système d'affichage d'informations continuelles.
int displayDataTimer = 0;
int displayDataType = 0;

// Permet d'afficher un élément sur l'écran à la deuxième ligne avec une couleur en arrière plan. l'affichage reviendra à la normale automatiquement.
void displayText(String text, int r, int g, int b)
{
    screen.setRGB(r, g, b);
    screen.setCursor(0, 0);
    screen.print("Serre connectée");
    screen.setCursor(0, 1);
    screen.print(text);
    displayElementTimer = 1;
}

// Fonction permettant de contrôler l'affichage de l'écran : au bout d'un certain temps, si un élément spécial a été affiché, l'écran reviendra à la normale.
void displaySchedule()
{
    // On vérifie qu'un message spécial (action, erreur...) n'est pas affiché.
    if (displayElementTimer == 0)
    {
        // Pendant 10 secondes, l'affichage reste basique.
        if (displayDataType == 0)
        {
            if (displayDataTimer == 10000)
            {
                displayDataTimer = 0;
                displayDataType = 1;
                displayData(displayDataType);
            }

            else
            {
                displayDataTimer++;
            }
        }

        // Après les 10 secondes d'affichage basique, on fait un tour des informations avec 1 seconde par information.
        else
        {
            if (displayDataTimer == 1000)
            {
                displayDataTimer = 0;

                // Si on a fait le tout, on revient à l'affichage basique pendans 10 secondes.
                if (displayDataType == 8)
                {
                    displayDataType == 0;
                }

                else
                {
                    displayDataType++;
                }

                displayData(displayDataType);
            }

            else
            {
                displayDataTimer++;
            }
        }
    }

    // Si un message spécial est actuellement affiché, on vérifie si le temps de son affichage est écoulé pour remettre un message basique ou on incrémente le compteur.
    else
    {
        if (displayElementTimer == 3000)
        {
            displayData(displayDataType);
        }

        else
        {
            displayElementTimer++;
        }
    }
}

// Cette fonction est utilisé par la displaySchedule(). Elle permet d'afficher un élément (valeur d'un capteur, état...) à l'écran.
void displayData(int data)
{
    // Affichage de la permière ligne.
    screen.setRGB(100, 100, 100);
    screen.setCursor(0, 0);
    screen.print("Serre connectée");
    screen.setCursor(0, 1);

    switch (data)
    {
    case 1:
        // Affichage de la température.
        screen.print("Temp. : ");
        sensors_event_t temperatureValue;
        airTempSensor.temperature().getEvent(&temperatureValue);
        screen.print(temperatureValue.temperature);
        screen.print(" °C.");
        break;

    case 2:
        // Affichage de l'état du toit.
        screen.print("Toit ");
        if (roofOpeningServoPosition == 0)
        {
            screen.print("fermé");
        }

        else
        {
            screen.print("ouvert");
        }
        screen.print(".");
        break;

    case 3:
        // Affichage de la luminosité.
        screen.print("Lumin. : ");
        int lightValue = analogRead(LIGHT_SENSOR_PIN);
        screen.print(lightValue);
        screen.print(" lux.");
        break;

    case 4:
        // Affichage de l'état des DEL.
        screen.print("Lumière ");
        int LEDState = digitalRead(LED_STRIP_RELAY_PIN);
        if (LEDState == 0)
        {
            screen.print("éteinte");
        }

        else
        {
            screen.print("allumée");
        }
        screen.print(".");
        break;

    case 5:
        // Affichage de l'humidité.
        screen.print("Hum. : ");
        sensors_event_t humidityValue;
        airTempSensor.temperature().getEvent(&humidityValue);
        screen.print(humidityValue.temperature);
        screen.print(" %.");
        break;

    case 6:
        // Affichage de l'état de l'humidificateur.
        screen.print("Humidifi. ");
        int humidifierState = digitalRead(HUMIDIFIER_RELAY_PIN);
        if (humidifierState == 0)
        {
            screen.print("éteint");
        }

        else
        {
            screen.print("allumé");
        }
        screen.print(".");
        break;

    case 7:
        // Affichage de l'eau.
        screen.print("Eau : ");
        int waterValue = analogRead(WATER_SENSOR_PIN);
        screen.print(waterValue);
        screen.print(".");
        break;

    case 8:
        // Affichage de l'état de la pompe.
        screen.print("Pompe ");
        int pumpState = digitalRead(WATER_PUMP_RELAY_PIN);
        if (pumpState == 0)
        {
            screen.print("éteinte");
        }

        else
        {
            screen.print("allumée");
        }
        screen.print(".");
        break;

    default:
        // Affichage du message basique.
        screen.print("Système opérationnel.");
        break;
    }
}