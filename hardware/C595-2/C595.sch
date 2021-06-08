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
L Connector_Generic:Conn_02x14_Counter_Clockwise J101
U 1 1 60832089
P 6450 3350
F 0 "J101" H 6500 4167 50  0000 C CNN
F 1 "Conn_02x14_Counter_Clockwise" H 6500 4076 50  0000 C CNN
F 2 "Housings_DIP:DIP-28_W15.24mm" H 6450 3350 50  0001 C CNN
F 3 "~" H 6450 3350 50  0001 C CNN
	1    6450 3350
	1    0    0    -1  
$EndComp
Text Label 6250 2750 2    50   ~ 0
SegD
Text Label 6250 2850 2    50   ~ 0
SegE
Text Label 6250 2950 2    50   ~ 0
SegF
Text Label 6250 3050 2    50   ~ 0
SegG
Text Label 6250 3250 2    50   ~ 0
KP
Text Label 6250 3350 2    50   ~ 0
KO
Text Label 6250 3450 2    50   ~ 0
KN
Text Label 6750 2750 0    50   ~ 0
SegC
Text Label 6750 2850 0    50   ~ 0
SegB
Text Label 6750 2950 0    50   ~ 0
SegA
Text Label 6750 3050 0    50   ~ 0
DP
Text Label 6750 3150 0    50   ~ 0
D1
Text Label 6750 3250 0    50   ~ 0
D2
Text Label 6750 3350 0    50   ~ 0
D3
Text Label 6750 3450 0    50   ~ 0
D4
Text Label 6750 3550 0    50   ~ 0
D5
Text Label 6750 3650 0    50   ~ 0
D6
Text Label 6750 3750 0    50   ~ 0
D7
Text Label 6750 3850 0    50   ~ 0
D8
Text Label 6750 3950 0    50   ~ 0
D9
Text Label 6750 4050 0    50   ~ 0
VCC
NoConn ~ 6250 3150
NoConn ~ 6250 3550
NoConn ~ 6250 3750
NoConn ~ 6250 3850
NoConn ~ 6250 3950
NoConn ~ 6250 4050
Wire Wire Line
	6250 3650 5950 3650
Wire Wire Line
	5950 3650 5950 3800
$Comp
L power:GND #PWR0101
U 1 1 60835E31
P 5950 3800
F 0 "#PWR0101" H 5950 3550 50  0001 C CNN
F 1 "GND" H 5955 3627 50  0000 C CNN
F 2 "" H 5950 3800 50  0001 C CNN
F 3 "" H 5950 3800 50  0001 C CNN
	1    5950 3800
	1    0    0    -1  
$EndComp
Text Label 3600 4050 3    50   ~ 0
D9
Text Label 4450 4050 3    50   ~ 0
D8
Text Label 4650 4050 3    50   ~ 0
D7
Text Label 4950 3700 0    50   ~ 0
D6
Text Label 4950 3600 0    50   ~ 0
D5
Text Label 4950 3500 0    50   ~ 0
D4
Text Label 4950 3400 0    50   ~ 0
D3
Text Label 4950 2900 0    50   ~ 0
D2
Text Label 4700 2550 0    50   ~ 0
D1
Text Label 4550 2550 0    50   ~ 0
DP
Text Label 3800 2550 1    50   ~ 0
SegA
Text Label 3650 2550 1    50   ~ 0
SegB
Text Label 3350 2750 2    50   ~ 0
SegC
Text Label 3350 2900 2    50   ~ 0
SegD
Text Label 3350 3050 2    50   ~ 0
SegE
Text Label 3350 3200 2    50   ~ 0
SegF
Text Label 3350 3400 2    50   ~ 0
SegG
Text Label 3350 3500 2    50   ~ 0
KP
Text Label 3350 3600 2    50   ~ 0
KO
Text Label 3350 3700 2    50   ~ 0
KN
Text Label 4250 2550 1    50   ~ 0
VCC
Text Label 4050 4050 3    50   ~ 0
VCC
Text Label 3750 4050 3    50   ~ 0
VCC
NoConn ~ 3900 4050
NoConn ~ 4300 4050
$Comp
L power:GND #PWR0102
U 1 1 60858A95
P 4400 2550
F 0 "#PWR0102" H 4400 2300 50  0001 C CNN
F 1 "GND" H 4405 2377 50  0000 C CNN
F 2 "" H 4400 2550 50  0001 C CNN
F 3 "" H 4400 2550 50  0001 C CNN
	1    4400 2550
	-1   0    0    1   
$EndComp
$Comp
L Device:C C101
U 1 1 6085ADB9
P 5500 2200
F 0 "C101" H 5615 2246 50  0000 L CNN
F 1 "100n" H 5615 2155 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5538 2050 50  0001 C CNN
F 3 "~" H 5500 2200 50  0001 C CNN
	1    5500 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C102
U 1 1 6085B1BA
P 5900 2200
F 0 "C102" H 6015 2246 50  0000 L CNN
F 1 "100n" H 6015 2155 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5938 2050 50  0001 C CNN
F 3 "~" H 5900 2200 50  0001 C CNN
	1    5900 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2350 5900 2350
Connection ~ 5900 2350
Wire Wire Line
	5900 2350 6050 2350
Wire Wire Line
	5500 2050 5900 2050
Connection ~ 5900 2050
Wire Wire Line
	5900 2050 6050 2050
Text Label 6050 2050 0    50   ~ 0
VCC
$Comp
L power:GND #PWR0103
U 1 1 6085BB22
P 6050 2350
F 0 "#PWR0103" H 6050 2100 50  0001 C CNN
F 1 "GND" H 6055 2177 50  0000 C CNN
F 2 "" H 6050 2350 50  0001 C CNN
F 3 "" H 6050 2350 50  0001 C CNN
	1    6050 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R101
U 1 1 6085C02E
P 5300 3300
F 0 "R101" V 5200 3300 50  0000 C CNN
F 1 "10k" V 5300 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5230 3300 50  0001 C CNN
F 3 "~" H 5300 3300 50  0001 C CNN
	1    5300 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	5150 3300 5100 3300
Text Label 5450 3300 0    50   ~ 0
VCC
$Comp
L C595:ATmega48PV-10AU-Phys U101
U 1 1 60860800
P 4200 3500
F 0 "U101" V 2950 3400 50  0000 L CNN
F 1 "ATmega48PV-10AU-Phys" V 5000 3000 50  0000 L CNN
F 2 "Housings_QFP:TQFP-32_7x7mm_Pitch0.8mm" H 2450 1900 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8025-8-bit-AVR-Microcontroller-ATmega48P-88P-168P_Datasheet.pdf" H 4200 3500 50  0001 C CNN
	1    4200 3500
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 60AFD0E9
P 4100 2550
F 0 "#PWR0104" H 4100 2300 50  0001 C CNN
F 1 "GND" H 4105 2377 50  0000 C CNN
F 2 "" H 4100 2550 50  0001 C CNN
F 3 "" H 4100 2550 50  0001 C CNN
	1    4100 2550
	-1   0    0    1   
$EndComp
Text Label 3950 2550 1    50   ~ 0
VCC
$Comp
L power:GND #PWR0105
U 1 1 60AFDB46
P 4200 4050
F 0 "#PWR0105" H 4200 3800 50  0001 C CNN
F 1 "GND" H 4205 3877 50  0000 C CNN
F 2 "" H 4200 4050 50  0001 C CNN
F 3 "" H 4200 4050 50  0001 C CNN
	1    4200 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J102
U 1 1 60B0200E
P 5500 3000
F 0 "J102" H 5472 2882 50  0000 R CNN
F 1 "Conn_01x02_Male" H 5472 2973 50  0000 R CNN
F 2 "Capacitors_SMD:C_0805" H 5500 3000 50  0001 C CNN
F 3 "~" H 5500 3000 50  0001 C CNN
	1    5500 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4950 3050 5100 3050
Wire Wire Line
	5100 3050 5100 2900
Wire Wire Line
	5100 2900 5300 2900
Wire Wire Line
	4950 3150 5200 3150
Wire Wire Line
	5200 3150 5200 3000
Wire Wire Line
	5200 3000 5300 3000
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J103
U 1 1 60B06840
P 7700 3250
F 0 "J103" H 7750 3567 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 7750 3476 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03_Pitch1.27mm_SMD" H 7700 3250 50  0001 C CNN
F 3 "~" H 7700 3250 50  0001 C CNN
	1    7700 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 3700 3100 3700
Wire Wire Line
	3350 3600 3100 3600
Wire Wire Line
	3600 4050 3600 4200
Wire Wire Line
	3600 4200 3100 4200
Wire Wire Line
	5100 3300 5100 3450
Wire Wire Line
	5100 3450 5250 3450
Connection ~ 5100 3300
Wire Wire Line
	5100 3300 4950 3300
Text Label 3100 3600 2    50   ~ 0
MOSI
Text Label 3100 3700 2    50   ~ 0
MISO
Text Label 3100 4200 2    50   ~ 0
SCK
Text Label 5250 3450 0    50   ~ 0
RST
Text Label 7500 3150 2    50   ~ 0
MISO
Text Label 7500 3250 2    50   ~ 0
SCK
Text Label 7500 3350 2    50   ~ 0
RST
Text Label 8000 3150 0    50   ~ 0
VCC
Text Label 8000 3250 0    50   ~ 0
MOSI
$Comp
L power:GND #PWR0106
U 1 1 60B09A4B
P 8000 3350
F 0 "#PWR0106" H 8000 3100 50  0001 C CNN
F 1 "GND" H 8005 3177 50  0000 C CNN
F 2 "" H 8000 3350 50  0001 C CNN
F 3 "" H 8000 3350 50  0001 C CNN
	1    8000 3350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
