EESchema Schematic File Version 4
EELAYER 30 0
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
L MCU_Module:Arduino_UNO_R3 A?
U 1 1 5E77DDDE
P 6350 3000
F 0 "A?" H 6350 4181 50  0000 C CNN
F 1 "Arduino_UNO_R3" H 6350 4090 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 6500 1950 50  0001 L CNN
F 3 "https://www.arduino.cc/en/Main/arduinoBoardUno" H 6150 4050 50  0001 C CNN
	1    6350 3000
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC547 Q?
U 1 1 5E789462
P 3900 3100
F 0 "Q?" H 4091 3146 50  0000 L CNN
F 1 "BC547" H 4091 3055 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 4100 3025 50  0001 L CIN
F 3 "http://www.fairchildsemi.com/ds/BC/BC547.pdf" H 3900 3100 50  0001 L CNN
	1    3900 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 2900 4000 2600
Wire Wire Line
	4000 2600 5850 2600
$Comp
L power:GND #PWR?
U 1 1 5E78D9A3
P 4000 4500
F 0 "#PWR?" H 4000 4250 50  0001 C CNN
F 1 "GND" H 4005 4327 50  0000 C CNN
F 2 "" H 4000 4500 50  0001 C CNN
F 3 "" H 4000 4500 50  0001 C CNN
	1    4000 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 3300 4000 4250
Wire Wire Line
	6250 4100 6250 4250
Wire Wire Line
	6250 4250 4000 4250
Connection ~ 4000 4250
Wire Wire Line
	4000 4250 4000 4500
Wire Wire Line
	6350 4100 6350 4250
Wire Wire Line
	6350 4250 6250 4250
Connection ~ 6250 4250
Wire Wire Line
	6450 4100 6450 4250
Wire Wire Line
	6450 4250 6350 4250
Connection ~ 6350 4250
$Comp
L Device:Q_Photo_NPN Q?
U 1 1 5E7944CB
P 2750 2700
F 0 "Q?" H 2940 2746 50  0000 L CNN
F 1 "Q_Photo_NPN" H 2940 2655 50  0000 L CNN
F 2 "" H 2950 2800 50  0001 C CNN
F 3 "~" H 2750 2700 50  0001 C CNN
	1    2750 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 2900 2850 3100
Wire Wire Line
	2850 3100 3700 3100
$Comp
L power:+5V #PWR?
U 1 1 5E795E90
P 2850 1550
F 0 "#PWR?" H 2850 1400 50  0001 C CNN
F 1 "+5V" H 2865 1723 50  0000 C CNN
F 2 "" H 2850 1550 50  0001 C CNN
F 3 "" H 2850 1550 50  0001 C CNN
	1    2850 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 1550 2850 2500
$EndSCHEMATC
