EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 15 17
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
L Device:R R23
U 1 1 6038C567
P 5750 3950
F 0 "R23" H 5820 3996 50  0000 L CNN
F 1 "100K" H 5820 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5680 3950 50  0001 C CNN
F 3 "~" H 5750 3950 50  0001 C CNN
	1    5750 3950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 6038D1BC
P 5750 4450
F 0 "#PWR024" H 5750 4200 50  0001 C CNN
F 1 "GND" H 5755 4277 50  0000 C CNN
F 2 "" H 5750 4450 50  0001 C CNN
F 3 "" H 5750 4450 50  0001 C CNN
	1    5750 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4450 5750 4100
$Comp
L Device:R R24
U 1 1 6038D944
P 5750 3300
F 0 "R24" H 5820 3346 50  0000 L CNN
F 1 "NTC - 100k" H 5820 3255 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5680 3300 50  0001 C CNN
F 3 "~" H 5750 3300 50  0001 C CNN
	1    5750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3450 5750 3600
Wire Wire Line
	5750 3600 6450 3600
Connection ~ 5750 3600
Wire Wire Line
	5750 3600 5750 3800
Text GLabel 6450 3600 2    50   Input ~ 0
TEMP_SENSOR
$Comp
L power:VCC #PWR022
U 1 1 6038E229
P 5750 3050
F 0 "#PWR022" H 5750 2900 50  0001 C CNN
F 1 "VCC" H 5765 3223 50  0000 C CNN
F 2 "" H 5750 3050 50  0001 C CNN
F 3 "" H 5750 3050 50  0001 C CNN
	1    5750 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 3050 5750 3150
$EndSCHEMATC
