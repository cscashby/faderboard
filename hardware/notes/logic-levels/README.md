# Logic level conversion

Unfortunately the MPR121 is a 3.3v device not a 5v device, so I have decided to replace this more functional device with two CAP1296 devices, which only support 6 channels but don't require complex level shifting technology.

In case I need the MPR121 - I have included a reference to the Adafruit level shifter library which appears to be a good I2C solution.

https://github.com/adafruit/4-Channel-Level-Shifter-PCB

