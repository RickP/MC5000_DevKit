EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 16 17
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
L LCSC_parts:SMD3528C-50 D6
U 1 1 6038FAE2
P 5450 3550
F 0 "D6" H 5400 3333 50  0000 C CNN
F 1 "SMD3528C-50" H 5400 3424 50  0000 C CNN
F 2 "LED_SMD:LED_1210_3225Metric" H 5450 3350 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1809172049_Senba-Sensing-Tech-SMD3528C-50_C250859.pdf" H 5400 3550 50  0001 C CNN
	1    5450 3550
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR025
U 1 1 60390853
P 4850 3550
F 0 "#PWR025" H 4850 3400 50  0001 C CNN
F 1 "VCC" V 4865 3677 50  0000 L CNN
F 2 "" H 4850 3550 50  0001 C CNN
F 3 "" H 4850 3550 50  0001 C CNN
	1    4850 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4850 3550 5350 3550
$Comp
L Device:R R26
U 1 1 603912E1
P 6150 3550
F 0 "R26" V 5943 3550 50  0000 C CNN
F 1 "10K" V 6034 3550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 6080 3550 50  0001 C CNN
F 3 "~" H 6150 3550 50  0001 C CNN
	1    6150 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	5650 3550 5800 3550
Wire Wire Line
	5800 3550 5800 4100
Connection ~ 5800 3550
Wire Wire Line
	5800 3550 6000 3550
$Comp
L power:GND #PWR027
U 1 1 60391B44
P 6550 3550
F 0 "#PWR027" H 6550 3300 50  0001 C CNN
F 1 "GND" V 6555 3422 50  0000 R CNN
F 2 "" H 6550 3550 50  0001 C CNN
F 3 "" H 6550 3550 50  0001 C CNN
	1    6550 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 3550 6550 3550
Text GLabel 5800 4100 3    50   Input ~ 0
LIGHT_SENSOR
$EndSCHEMATC