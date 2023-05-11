# Serre connectée

Ce programme permet la gestion de la serre connectée. L'Arduino UNO est connecté aux différents capteurs et actionneurs de la serre pour rendre la culture des plantes "automatique".  

Le but est de subvenir à tous les besoins de la plante automatiquement : eau, lumière, température.  

Le programme a été écrit en C++ pour l'Arduino UNO à l'aide de platformIO. Plusieurs bibliothèques ont été utilisées pour faire fonctionner le programme (liste disponible dans le fichier platformio.ini).

> La serre connectée utilisée est celle-ci : [serre connectée](https://technologieservices.fr/ts_fr/277630.html).

Le programme est amené à évoluer.

## Connexions à l'Arduino UNO

| Broche  | Périphérique connecté                | Nom dans le programme             |
| ------- | ------------------------------------ | --------------------------------- |
| A0 / 14 | Capteur d'eau                        | `PIN_WATER_SENSOR`                |
| A1 / 15 | Capteur de luminosité                | `PIN_LIGHT_SENSOR`                |
|  8      | Relais humidificateur                | `PIN_HUMIDIFIER_RELAY`            |
|  6      | Servomoteur ouverture toit           | `PIN_ROOF_OPENING_SERVOMOTOR`     |
|  3      | Capteur d'humidité et de température | `PIN_TEMPERATURE_HUMIDITY_SENSOR` |
|  2      | Bouton                               | `PIN_BUTTON`                      |
|  7      | Relais ruban DEL                     | `PIN_LED_STRIP_RELAY`             |
|  5      | Relais de la pompe                   | `PIN_WATER_PUMP_RELAY`            |
|  I2C    | Ecran LCD                            |                                   |

## Ecran LCD

L'écran LCD dispose d'un rétroéclairage RGB. Cette fonctionnalité permet d'améliorer l'interface utilisateur en permettant d'augmenter la compréhension de ce qu'il se passe sur la serre.  

Différents codes couleurs sont utilisés :

- Blanc (peu lumineux) : affichage de veille. Il affiche des informations basiques tels que les valeurs des capteurs, les états des éléments du système...
- Vert : action effectuée. Par exemple, le toit a été ouvert.
- Rouge : erreur. Les codes d'erreur sont :
    - 001 : Erreur de lecture de la température.
    - 002 : Erreur de lecture de l'humidité.

## Choses à faire

- [x] Programmer la base du programme.
- [ ] Tester la partie gestion de l'eau.
- [ ] Régler le problème du servomoteur.