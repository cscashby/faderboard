EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:arduino
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Arduino_Mega2560_Shield ARD?
U 1 1 5A577670
P 4800 4100
F 0 "ARD?" V 4900 4100 60  0000 C CNN
F 1 "Arduino_Mega2560_Shield" V 4700 4100 60  0000 C CNN
F 2 "" H 5500 6850 60  0001 C CNN
F 3 "" H 5500 6850 60  0001 C CNN
	1    4800 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5A5776E6
P 3100 5550
F 0 "#PWR?" H 3100 5300 50  0001 C CNN
F 1 "GND" H 3100 5400 50  0000 C CNN
F 2 "" H 3100 5550 50  0001 C CNN
F 3 "" H 3100 5550 50  0001 C CNN
	1    3100 5550
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 5A5776FC
P 3100 6050
F 0 "#PWR?" H 3100 5900 50  0001 C CNN
F 1 "VCC" H 3100 6200 50  0000 C CNN
F 2 "" H 3100 6050 50  0001 C CNN
F 3 "" H 3100 6050 50  0001 C CNN
	1    3100 6050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR?
U 1 1 5A577712
P 2850 5800
F 0 "#PWR?" H 2850 5650 50  0001 C CNN
F 1 "+3.3V" H 2850 5940 50  0000 C CNN
F 2 "" H 2850 5800 50  0001 C CNN
F 3 "" H 2850 5800 50  0001 C CNN
	1    2850 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 5350 3400 5350
Wire Wire Line
	3400 5350 3400 5450
Wire Wire Line
	3400 5450 3400 5550
Wire Wire Line
	3400 5550 3400 5650
Wire Wire Line
	3400 5650 3400 5750
Wire Wire Line
	3400 5750 3500 5750
Wire Wire Line
	3500 5650 3400 5650
Connection ~ 3400 5650
Wire Wire Line
	3500 5550 3400 5550
Connection ~ 3400 5550
Wire Wire Line
	3100 5450 3400 5450
Wire Wire Line
	3400 5450 3500 5450
Connection ~ 3400 5450
Wire Wire Line
	3100 5450 3100 5550
Wire Wire Line
	3500 5950 3400 5950
Wire Wire Line
	3400 5950 3400 6050
Wire Wire Line
	3400 6050 3400 6150
Wire Wire Line
	3500 6150 3400 6150
Wire Wire Line
	3100 6050 3400 6050
Wire Wire Line
	3400 6050 3500 6050
Connection ~ 3400 6050
Wire Wire Line
	2850 5800 2850 5850
Wire Wire Line
	2850 5850 3500 5850
$EndSCHEMATC
