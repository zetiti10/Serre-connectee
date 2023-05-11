/**
 * @file display.cpp
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
int refreshRateCounter = 0;

// Permet d'afficher un élément sur l'écran à la deuxième ligne avec une couleur en arrière plan. l'affichage reviendra à la normale automatiquement.
void displayText(String text, int r, int g, int b)
{
    screen.clear();
    screen.setRGB(r, g, b);
    screen.setCursor(0, 0);
    screen.print("Serre connectee");
    screen.setCursor(0, 1);
    screen.print(text);
    displayElementTimer = 1;
}

// Cette fonction est utilisé par la displaySchedule(). Elle permet d'afficher un élément (valeur d'un capteur, état...) à l'écran.
void displayData(int data)
{
    // Affichage de la permière ligne.
    screen.clear();
    screen.setRGB(50, 50, 50);
    screen.setCursor(0, 0);
    screen.print("Serre connectee");
    screen.setCursor(0, 1);

    switch (data)
    {
    case 1:
    {
        // Affichage de la température.
        screen.print("Temp. : ");
        sensors_event_t temperatureValue;
        airTempSensor.temperature().getEvent(&temperatureValue);
        screen.print(temperatureValue.temperature);
        screen.print(" C");
        break;
    }

    case 2:
    {
        // Affichage de l'état du toit.
        screen.print("Toit ");
        if (roofOpeningServoPosition == 0)
        {
            screen.print("O");
        }

        else
        {
            screen.print("I");
        }
        break;
    }

    case 3:
    {
        // Affichage de la luminosité.
        screen.print("Lumin. : ");
        int lightValue = analogRead(PIN_LIGHT_SENSOR);
        screen.print(lightValue);
        screen.print(" lux");
        break;
    }

    case 4:
    {
        // Affichage de l'état des DEL.
        screen.print("Lumiere ");
        int LEDState = digitalRead(PIN_LED_STRIP_RELAY);
        if (LEDState == 0)
        {
            screen.print("O");
        }

        else
        {
            screen.print("I");
        }
        break;
    }

    case 5:
    {
        // Affichage de l'humidité.
        screen.print("Hum. : ");
        sensors_event_t airValues;
        airTempSensor.humidity().getEvent(&airValues);
        screen.print(airValues.relative_humidity);
        screen.print(" %");
        break;
    }

    case 6:
    {
        // Affichage de l'état de l'humidificateur.
        screen.print("Humidificateur ");
        int humidifierState = digitalRead(PIN_HUMIDIFIER_RELAY);
        if (humidifierState == 0)
        {
            screen.print("O");
        }

        else
        {
            screen.print("I");
        }
        break;
    }

    case 7:
    {
        // Affichage de l'eau.
        screen.print("Eau : ");
        int waterValue = analogRead(PIN_WATER_SENSOR);
        screen.print(waterValue);
        break;
    }

    case 8:
    {
        // Affichage de l'état de la pompe.
        screen.print("Pompe ");
        int pumpState = digitalRead(PIN_WATER_PUMP_RELAY);
        if (pumpState == 0)
        {
            screen.print("O");
        }

        else
        {
            screen.print("I");
        }
        screen.print(".");
        break;
    }

    default:
    {
        // Affichage du message basique.
        screen.print("Operationel !");
        break;
    }
    }
}

// Fonction permettant de contrôler l'affichage de l'écran : au bout d'un certain temps, si un élément spécial a été affiché, l'écran reviendra à la normale.
void displaySchedule()
{
    // On limite le taux de rafraichissement pour que l'affichage soit correct.
    refreshRateCounter++;

    if (refreshRateCounter == 1000)
    {
        refreshRateCounter = 0;

        // On vérifie qu'un message spécial (action, erreur...) n'est pas affiché.
        if (displayElementTimer == 0)
        {
            // Pendant 10 secondes, l'affichage reste basique.
            if (displayDataType == 0)
            {
                if (displayDataTimer == 10)
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
                if (displayDataTimer == 1)
                {
                    displayDataTimer = 0;

                    // Si on a fait le tout, on revient à l'affichage basique pendans 10 secondes.
                    if (displayDataType == 8)
                    {
                        displayDataType = 0;
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
            if (displayElementTimer == 3)
            {
                displayData(displayDataType);
                displayElementTimer = 0;
            }

            else
            {
                displayElementTimer++;
            }
        }
    }
}