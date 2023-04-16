# Serre connectée

Ce programme permet la gestion de la serre connectée. L'Arduino UNO est connecté aux différents capteurs et actionneurs de la serre pour rendre la culture des plantes "automatique".  

Le but est de subvenir à tous les besoins de la plante automatiquement : eau, lumière, température.  

Le programme a été écrit en C++ pour l'Arduino UNO à l'aide de platformIO. Plusieurs bibliothèques ont été utilisées pour faire fonctionner le programme (liste disponible dans le fichier platformio.ini).

> La serre connectée utilisée est celle-ci : [serre connectée](https://technologieservices.fr/ts_fr/277630.html).

Le programme est amené à évoluer.

## Connexions à l'Arduino UNO

| Broche  | Périphérique connecté                | Nom dans le programme             |
| ------- | ------------------------------------ | --------------------------------- |
| A0 / 14 | Capteur d'eau                        | `WATER_SENSOR_PIN`                |
| A1 / 15 | Capteur de luminosité                | `LIGHT_SENSOR_PIN`                |
|  8      | Relais humidificateur                | `HUMIDIFIER_RELAY_PIN`            |
|  6      | Servomoteur ouverture toit           | `ROOF_OPENING_SERVOMOTOR_PIN`     |
|  3      | Capteur d'humidité et de température | `TEMPERATURE_HUMIDITY_SENSOR_PIN` |
|  2      | Bouton                               | `BUTTON_PIN`                      |
|  7      | Relais ruban DEL                     | `LED_STRIP_RELAY_PIN`             |
|  5      | Relais de la pompe                   | `WATER_PUMP_RELAY_PIN`            |
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
- [ ] Tester le programme.
- [ ] Vérifier la broche du relais de la pompe à eau.