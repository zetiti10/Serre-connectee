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
#include <screen.hpp>

// Variables pour gérer l'arrosage.
int wateringDelay = 0;
int wateringInterval = 0;

// Création du servomoteur.
Servo roofOpeningServo;

// Création du capteur de température et humidité de l'air.
DHT_Unified airTempSensor(TEMPERATURE_HUMIDITY_SENSOR_PIN, DHT22);

// Création de l'écran LCD.
rgb_lcd screen;

void setup()
{
  // Initialisation et affichage du message initial sur l'écran.
  screen.begin(16, 2);
  displayText("Initialisation...", 255, 255, 255);
  delay(1000);

  // Initialisation des broches des capteurs.
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(WATER_SENSOR_PIN, INPUT);

  // Initialisation des broches des actionneurs.
  pinMode(LED_STRIP_RELAY_PIN, OUTPUT);
  pinMode(HUMIDIFIER_RELAY_PIN, OUTPUT);
  pinMode(WATER_PUMP_RELAY_PIN, OUTPUT);

  // Initialisation des composants.
  roofOpeningServo.attach(ROOF_OPENING_SERVOMOTOR_PIN);
  roofOpeningServo.write(roofOpeningServoPosition);
  airTempSensor.begin();

  // Affichage du message de réussite.
  displayText("Initialisation terminée !", 0, 255, 0);
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
    if (airValues.temperature >= 30 && roofOpeningServoPosition == 0)
    {
      roofOpeningServoControl(50);
      displayText("Ouverture du toit...", 0, 255, 0);
    }

    else if (roofOpeningServoPosition == 50)
    {
      roofOpeningServoControl(0);
      displayText("Fermeture du toit...", 0, 255, 0);
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
    int humidifierState = digitalRead(HUMIDIFIER_RELAY_PIN);

    if (airValues.relative_humidity < 50 && humidifierState == LOW)
    {
      digitalWrite(HUMIDIFIER_RELAY_PIN, HIGH);
      displayText("Humidification...", 0, 255, 0);
    }

    else if (humidifierState == HIGH)
    {
      digitalWrite(HUMIDIFIER_RELAY_PIN, LOW);
      displayText("Arrêt de l'humidification...", 0, 255, 0);
    }
  }

  // Gestion de la luminosité : contrôle des rubans de DEL sur le toit de la serre.
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  int LEDState = digitalRead(LED_STRIP_RELAY_PIN);
  if (lightValue < 800 && LEDState == LOW)
  {
    digitalWrite(LED_STRIP_RELAY_PIN, HIGH);
    displayText("Activation de l'éclairage...", 0, 255, 0);
  }

  else if (LEDState == HIGH)
  {
    digitalWrite(LED_STRIP_RELAY_PIN, LOW);
    displayText("Arrêt de l'éclairage...", 0, 255, 0);
  }

  // Gestion de l'eau : contrôle de la pompe.
  int waterValue = analogRead(WATER_SENSOR_PIN);
  int pumpState = digitalRead(WATER_PUMP_RELAY_PIN);
  if (waterValue < 200 && wateringDelay == 0 && wateringInterval == 0)
  {
    digitalWrite(WATER_PUMP_RELAY_PIN, HIGH);
    wateringDelay++;
    displayText("Activation de la pompe...", 0, 255, 0);
  }

  // Boucle pour éteindre automatiquement la pompe après 10 secondes.
  if (wateringDelay != 0)
  {
    if (wateringDelay == 10000)
    {
      wateringDelay = 0;
      wateringInterval = 1;
    }

    else
    {
      wateringDelay++;
    }
  }

  // Boucle pour limiter le nombre d'activations de la pompe.
  if(wateringInterval != 0)
  {
    if(wateringInterval == 60000)
    {
      wateringInterval = 0;
    }

    else
    {
      wateringInterval++;
    }
  }
}