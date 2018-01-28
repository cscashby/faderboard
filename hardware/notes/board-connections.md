# Faderbaard connector layout

## 8-fader, OLED board

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
|  FD-J1 | 12 | 'IRQ | For touch (active Low) |
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
|  FD-J1 | 24 | BSEL | Board select (MPR121 I2C addr GND = 0x5a, VCC = 0x5b) |
