EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 15
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
L Device:LED D3
U 1 1 603673A6
P 4800 3650
F 0 "D3" H 4793 3866 50  0000 C CNN
F 1 "LED" H 4793 3775 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4800 3650 50  0001 C CNN
F 3 "~" H 4800 3650 50  0001 C CNN
F 4 "C84256" H 4800 3650 50  0001 C CNN "LCSC"
	1    4800 3650
	0    1    1    0   
$EndComp
$Comp
L Device:LED D4
U 1 1 60383DEE
P 5400 3650
F 0 "D4" H 5393 3866 50  0000 C CNN
F 1 "LED" H 5393 3775 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5400 3650 50  0001 C CNN
F 3 "~" H 5400 3650 50  0001 C CNN
F 4 "C2296" H 5400 3650 50  0001 C CNN "LCSC"
	1    5400 3650
	0    1    1    0   
$EndComp
$Comp
L Device:LED D5
U 1 1 60387BD3
P 6000 3650
F 0 "D5" H 5993 3866 50  0000 C CNN
F 1 "LED" H 5993 3775 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6000 3650 50  0001 C CNN
F 3 "~" H 6000 3650 50  0001 C CNN
F 4 "C2297" H 6000 3650 50  0001 C CNN "LCSC"
	1    6000 3650
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 60399B8F
P 4800 3250
AR Path="/603632C9/60399B8F" Ref="R?"  Part="1" 
AR Path="/6036720E/60399B8F" Ref="R25"  Part="1" 
F 0 "R25" H 4730 3204 50  0000 R CNN
F 1 "220R" H 4730 3295 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4730 3250 50  0001 C CNN
F 3 "~" H 4800 3250 50  0001 C CNN
F 4 "C22962" H 4800 3250 50  0001 C CNN "LCSC"
	1    4800 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	4800 3400 4800 3500
$Comp
L Device:R R?
U 1 1 6039D17A
P 5400 3250
AR Path="/603632C9/6039D17A" Ref="R?"  Part="1" 
AR Path="/6036720E/6039D17A" Ref="R28"  Part="1" 
F 0 "R28" H 5330 3204 50  0000 R CNN
F 1 "220R" H 5330 3295 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5330 3250 50  0001 C CNN
F 3 "~" H 5400 3250 50  0001 C CNN
F 4 "C22962" H 5400 3250 50  0001 C CNN "LCSC"
	1    5400 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	5400 3400 5400 3500
$Comp
L Device:R R?
U 1 1 6039DCC9
P 6000 3250
AR Path="/603632C9/6039DCC9" Ref="R?"  Part="1" 
AR Path="/6036720E/6039DCC9" Ref="R31"  Part="1" 
F 0 "R31" H 5930 3204 50  0000 R CNN
F 1 "120R" H 5930 3295 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5930 3250 50  0001 C CNN
F 3 "~" H 6000 3250 50  0001 C CNN
F 4 "C22787" H 6000 3250 50  0001 C CNN "LCSC"
	1    6000 3250
	-1   0    0    1   
$EndComp
Wire Wire Line
	6000 3400 6000 3500
Text GLabel 4800 4100 3    50   Input ~ 0
LED_RED
Text GLabel 5400 4100 3    50   Input ~ 0
LED_YELLOW
Text GLabel 6000 4100 3    50   Input ~ 0
LED_GREEN
Wire Wire Line
	5400 3800 5400 4100
Wire Wire Line
	6000 3800 6000 4100
Wire Wire Line
	4800 3800 4800 4100
Wire Wire Line
	6000 3100 6000 2850
Wire Wire Line
	5400 3100 5400 2850
Wire Wire Line
	4800 3100 4800 2850
$Comp
L power:GND #PWR0121
U 1 1 6038E1A7
P 4800 2850
F 0 "#PWR0121" H 4800 2600 50  0001 C CNN
F 1 "GND" H 4805 2677 50  0000 C CNN
F 2 "" H 4800 2850 50  0001 C CNN
F 3 "" H 4800 2850 50  0001 C CNN
	1    4800 2850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 6038EAD1
P 5400 2850
F 0 "#PWR0122" H 5400 2600 50  0001 C CNN
F 1 "GND" H 5405 2677 50  0000 C CNN
F 2 "" H 5400 2850 50  0001 C CNN
F 3 "" H 5400 2850 50  0001 C CNN
	1    5400 2850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 6038EDC9
P 6000 2850
F 0 "#PWR0123" H 6000 2600 50  0001 C CNN
F 1 "GND" H 6005 2677 50  0000 C CNN
F 2 "" H 6000 2850 50  0001 C CNN
F 3 "" H 6000 2850 50  0001 C CNN
	1    6000 2850
	-1   0    0    1   
$EndComp
$EndSCHEMATC
