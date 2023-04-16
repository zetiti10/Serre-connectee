#ifndef MAIN_DEFINITIONS
#define MAIN_DEFINITIONS

// Ajout des bibliothèques au programme.
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <rgb_lcd.h>

// Fonctions.
extern rgb_lcd screen;
extern DHT_Unified airTempSensor;
extern Servo roofOpeningServo;

#endif