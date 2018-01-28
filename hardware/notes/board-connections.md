# Faderboard connector layout

## 8-fader and OLED control

### By pin

|  **Connector** | **Pin** | **Signal Name** | **Notes** |
|  ------: | :------: | :------: | ------ |
|  FD-J1 | 1 | GND |  |
|  FD-J1 | 2 | VCC / VDD | +3.3v |
|  FD-J1 | 3 | VREF | Vref |
|  FD-J1 | 4 | GND |  |
|  FD-J1 | 5 | GND |  |
|  FD-J1 | 6 | VMOTOR | +12v TBC |
|  FD-J1 | 7 | GND |  |
|  FD-J1 | 8 | MCTLA | Motor Control |
|  FD-J1 | 9 | MCTLB | Motor Control |
|  FD-J1 | 10 | SDA | I2C |
|  FD-J1 | 11 | SCL | I2C |
|  FD-J1 | 12 | FIRQ | For touch (active Low) |
|  FD-J1 | 13 | GND |  |
|  FD-J1 | 14 | FADER | Fader GND->Vref |
|  FD-J1 | 15 | GND |  |
|  FD-J1 | 16 | MA0 | Motor address LSB |
|  FD-J1 | 17 | MA1 | Motor address |
|  FD-J1 | 18 | MA2 | Motor address MSB |
|  FD-J1 | 19 | MA3 | Motor enable for board (active High) |
|  FD-J1 | 20 | FA0 | Fader address LSB |
|  FD-J1 | 21 | FA1 | Fader address |
|  FD-J1 | 22 | FA2 | Fader address MSB |
|  FD-J1 | 23 | FA3 | Fader enable for board (active High) |
|  FD-J1 | 24 | DA0 | Display address LSB |
|  FD-J1 | 25 | DA1 | Display address |
|  FD-J1 | 26 | DA2 | Display address MSB |
|  FD-J1 | 27 | DA3 | Display enable for board (active High) |
|  FD-J1 | 28 | DRST | Display Reset |
|  FD-J1 | 29 | DDC | Display D/C |
|  FD-J1 | 30 | BSEL | Board select (MPR121 I2C addr GND = 0x5a, VCC = 0x5b) |
|  FD-J1 | 31 | GND |  |
|  FD-J1 | 32 | GND |  |

### Connector layout

|  **Name** | **Pin** | **Pin** | **Name** |
|  ------: | :------: | :------: | ------ |
|  GND | 1 | 2 | VCC / VDD |
|  VREF | 3 | 4 | GND |
|  GND | 5 | 6 | VMOTOR |
|  GND | 7 | 8 | MCTLA |
|  MCTLB | 9 | 10 | SDA |
|  SCL | 11 | 12 | FIRQ |
|  GND | 13 | 14 | FADER |
|  GND | 15 | 16 | MA0 |
|  MA1 | 17 | 18 | MA2 |
|  MA3 | 19 | 20 | FA0 |
|  FA1 | 21 | 22 | FA2 |
|  FA3 | 23 | 24 | DA0 |
|  DA1 | 25 | 26 | DA2 |
|  DA3 | 27 | 28 | DRST |
|  DDC | 29 | 30 | BSEL |
|  GND | 31 | 32 | GND |