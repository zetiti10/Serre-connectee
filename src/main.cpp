/**
 * @file main.cpp
 * @author Louis L
 * @brief Fichier principal de la serre connectée.
 * @version 1.0
 * @date 2023-04-13
 */

// Ajout des bibliothèques au programme.
#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <rgb_lcd.h>

// Autres fichiers du programme.
#include <pinDefinitions.hpp>
#include <main.hpp>
#include <devices.hpp>
#include <display.hpp>

// Variables pour gérer l'arrosage.
int wateringDelay = 0;
int wateringInterval = 0;

// Création du servomoteur.
Servo roofOpeningServo;

// Création du capteur de température et humidité de l'air.
DHT_Unified airTempSensor(PIN_TEMPERATURE_HUMIDITY_SENSOR, DHT22);

// Création de l'écran LCD.
rgb_lcd screen;

void setup()
{
  // Initialisation et affichage du message initial sur l'écran.
  screen.begin(16, 2);
  displayText("Initialisation", 255, 255, 255);
  delay(1000);

  // Initialisation des broches des capteurs.
  pinMode(PIN_LIGHT_SENSOR, INPUT);
  pinMode(PIN_WATER_SENSOR, INPUT);

  // Initialisation des broches des actionneurs.
  pinMode(PIN_LED_STRIP_RELAY, OUTPUT);
  pinMode(PIN_HUMIDIFIER_RELAY, OUTPUT);
  pinMode(PIN_WATER_PUMP_RELAY, OUTPUT);

  // Initialisation des composants.
  roofOpeningServo.attach(PIN_ROOF_OPENING_SERVOMOTOR);
  roofOpeningServo.write(roofOpeningServoPosition);
  airTempSensor.begin();

  // Affichage du message de réussite.
  displayText("Pret !", 0, 255, 0);
}

void loop()
{

  // On attend 1 milliseconde pour "ralentir" le système.
  delay(1);

  // Gestion de l'écran.
  displaySchedule();

  sensors_event_t airValues;

  // Gestion de la température : ouverture du toit de la serre.
  airTempSensor.temperature().getEvent(&airValues);

  // Vérification que la température a bien été reçu.
  if (isnan(airValues.temperature))
  {
    displayText("Erreur 001", 255, 0, 0);
  }

  // En fonction de la température, on effectue une action.
  else
  {
    if (airValues.temperature >= 25 && roofOpeningServoPosition == 0)
    {
      roofOpeningServoControl(50);
      displayText("I Toit", 0, 255, 0);
    }

    else if (airValues.temperature <= 23 && roofOpeningServoPosition == 50)
    {
      roofOpeningServoControl(0);
      displayText("O Toit", 0, 255, 0);
    }
  }

  // Gestion de l'humidité : activation de l'humidificateur.
  airTempSensor.humidity().getEvent(&airValues);

  // Vérification que l'humidité a bien été reçu.
  if (isnan(airValues.relative_humidity))
  {
    displayText("Erreur 002", 255, 0, 0);
  }

  // En fonction de l'humidité, on effectue une action.
  else
  {
    int humidifierState = digitalRead(PIN_HUMIDIFIER_RELAY);

    if (airValues.relative_humidity < 50 && humidifierState == LOW)
    {
      digitalWrite(PIN_HUMIDIFIER_RELAY, HIGH);
      displayText("I Humidificateur", 0, 255, 0);
    }

    else if (airValues.relative_humidity > 60 && humidifierState == HIGH)
    {
      digitalWrite(PIN_HUMIDIFIER_RELAY, LOW);
      displayText("O Humidificateur", 0, 255, 0);
    }
  }

  // Gestion de la luminosité : contrôle des rubans de DEL sur le toit de la serre.
  int lightValue = analogRead(PIN_LIGHT_SENSOR);
  int LEDState = digitalRead(PIN_LED_STRIP_RELAY);
  if (lightValue < 800 && LEDState == LOW)
  {
    digitalWrite(PIN_LED_STRIP_RELAY, HIGH);
    displayText("I Eclairage", 0, 255, 0);
  }

  else if (lightValue > 900 && LEDState == HIGH)
  {
    digitalWrite(PIN_LED_STRIP_RELAY, LOW);
    displayText("O Eclairage", 0, 255, 0);
  }

  // Gestion de l'eau : contrôle de la pompe.
  //Cette partie est ignorée car non testée.
  /*int waterValue = analogRead(PIN_WATER_SENSOR);
  // int pumpState = digitalRead(PIN_WATER_PUMP_RELAY);
  if (waterValue < 200 && wateringDelay == 0 && wateringInterval == 0)
  {
    digitalWrite(PIN_WATER_PUMP_RELAY, HIGH);
    wateringDelay++;
    displayText("I Pompe", 0, 255, 0);
  }

  // Boucle pour éteindre automatiquement la pompe après 10 secondes.
  if (wateringDelay != 0)
  {
    if (wateringDelay == 10000)
    {
      wateringDelay = 0;
      wateringInterval = 1;
      digitalWrite(PIN_WATER_PUMP_RELAY, LOW);
      displayText("O Pompe", 0, 255, 0);
    }

    else
    {
      wateringDelay++;
    }
  }

  // Boucle pour limiter le nombre d'activations de la pompe.
  if (wateringInterval != 0)
  {
    if (wateringInterval == 60000)
    {
      wateringInterval = 0;
    }

    else
    {
      wateringInterval++;
    }
  }*/
}