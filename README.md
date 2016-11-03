# PongDuino

It's a fork of famous vidéo game, **PONG**.
It has been reinvented for your enjoyment on the popular Arduino uno.

<h2>Materials</h2>

**Necessary**

- Arduino Uno
- Joystick
- Nokia 5110 screen
- 1 resistor 1KΩ
- 4 resistor 10kΩ

**Optional**

- Buzzer

<h2>Connection</h2>

- **Nokia Screen** : 

Acronym | Role | Connection
---|---|---
RST | Reset | 1 resistor 10kΩ and pin 6
CE | Chip enable | 1 resistor 1kΩ and pin 7
DC | Data/Command selection | 1 resistor 10kΩ and pin 5
Din | Serial input | 1 resistor 10kΩ and pin 4
Clk | Clock input | 1 resistor 10kΩ and pin 3
Vcc | Alimentation | pin 5v
BL | Led | pin 3v
GND | Mass | pin GND

- **Jostick** : 

Acronym | Role | Connection
---|---|---
GND | Mass | pin GND
+5v | Alimentation | pin 5v
VRx | Potentiometer on axis X | pin A0 or A1
VRy | Potentiometer on axis Y | pin A0 or A1
SW | Push button | pin 8

- **Buzzer** :

Acronyme | Rôle | Branchement
---|---|---
GND | Mass | pin GND
+ | Alimentation | pin 12

