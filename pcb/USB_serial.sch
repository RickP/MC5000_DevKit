EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 15
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
L LCSC_parts:USB_B_Micro-Connector J?
U 1 1 602FFA24
P 3600 3550
AR Path="/602FFA24" Ref="J?"  Part="1" 
AR Path="/602FF29A/602FFA24" Ref="J1"  Part="1" 
F 0 "J1" H 3657 4017 50  0000 C CNN
F 1 "USB_B_Micro" H 3657 3926 50  0000 C CNN
F 2 "LCSC_parts:MicroXNJ" H 3750 3500 50  0001 C CNN
F 3 "~" H 3750 3500 50  0001 C CNN
	1    3600 3550
	1    0    0    -1  
$EndComp
$Comp
L LCSC_parts:CH340K U5
U 1 1 60307685
P 4750 3750
F 0 "U5" H 4750 3377 50  0000 C CNN
F 1 "CH340K" H 4750 3286 50  0000 C CNN
F 2 "LCSC_parts:ESSOP-10_3.9x4.9mm_P1.00mm" H 4750 3300 50  0001 C CNN
F 3 "" H 4750 3750 50  0001 C CNN
	1    4750 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3350 4000 3350
Wire Wire Line
	4000 3350 4000 2850
$Comp
L power:GND #PWR01
U 1 1 6030B20C
P 3600 4150
F 0 "#PWR01" H 3600 3900 50  0001 C CNN
F 1 "GND" H 3605 3977 50  0000 C CNN
F 2 "" H 3600 4150 50  0001 C CNN
F 3 "" H 3600 4150 50  0001 C CNN
	1    3600 4150
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR02
U 1 1 6030BC2E
P 4000 2850
F 0 "#PWR02" H 4000 2700 50  0001 C CNN
F 1 "VCC" H 4015 3023 50  0000 C CNN
F 2 "" H 4000 2850 50  0001 C CNN
F 3 "" H 4000 2850 50  0001 C CNN
	1    4000 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3550 4300 3550
Wire Wire Line
	4300 3650 3900 3650
NoConn ~ 3900 3750
NoConn ~ 4300 3850
NoConn ~ 4300 3950
NoConn ~ 5200 3950
Wire Wire Line
	4300 3750 4000 3750
Wire Wire Line
	4000 3750 4000 3950
Wire Wire Line
	4000 3950 3600 3950
Connection ~ 3600 3950
Wire Wire Line
	3500 3950 3600 3950
Wire Wire Line
	3600 4150 3600 3950
$Comp
L Device:C C1
U 1 1 6030ED7F
P 5350 3400
F 0 "C1" H 5465 3446 50  0000 L CNN
F 1 "0.1uF" H 5465 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5388 3250 50  0001 C CNN
F 3 "~" H 5350 3400 50  0001 C CNN
	1    5350 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 6030F8A4
P 4750 3200
F 0 "#PWR03" H 4750 2950 50  0001 C CNN
F 1 "GND" H 4755 3027 50  0000 C CNN
F 2 "" H 4750 3200 50  0001 C CNN
F 3 "" H 4750 3200 50  0001 C CNN
	1    4750 3200
	-1   0    0    1   
$EndComp
Wire Wire Line
	4750 3200 4750 3350
Wire Wire Line
	5350 3550 5200 3550
Wire Wire Line
	5350 3250 5350 3100
$Comp
L power:GND #PWR04
U 1 1 6031068E
P 5350 3100
F 0 "#PWR04" H 5350 2850 50  0001 C CNN
F 1 "GND" H 5355 2927 50  0000 C CNN
F 2 "" H 5350 3100 50  0001 C CNN
F 3 "" H 5350 3100 50  0001 C CNN
	1    5350 3100
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR05
U 1 1 60310A8D
P 5350 4250
F 0 "#PWR05" H 5350 4100 50  0001 C CNN
F 1 "VCC" H 5365 4423 50  0000 C CNN
F 2 "" H 5350 4250 50  0001 C CNN
F 3 "" H 5350 4250 50  0001 C CNN
	1    5350 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	5350 3850 5200 3850
Wire Wire Line
	5350 3850 5350 4100
Wire Wire Line
	5350 4100 5500 4100
Connection ~ 5350 4100
Wire Wire Line
	5350 4100 5350 4250
$Comp
L Device:C C2
U 1 1 60312027
P 5650 4100
F 0 "C2" V 5398 4100 50  0000 C CNN
F 1 "0.1uF" V 5489 4100 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5688 3950 50  0001 C CNN
F 3 "~" H 5650 4100 50  0001 C CNN
	1    5650 4100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 603127D8
P 5950 4100
F 0 "#PWR06" H 5950 3850 50  0001 C CNN
F 1 "GND" V 5955 3972 50  0000 R CNN
F 2 "" H 5950 4100 50  0001 C CNN
F 3 "" H 5950 4100 50  0001 C CNN
	1    5950 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5950 4100 5800 4100
Wire Wire Line
	5200 3650 6050 3650
Text GLabel 6050 3650 2    50   Input ~ 0
RXD
Text GLabel 5850 3750 2    50   Input ~ 0
TXD
Wire Wire Line
	5850 3750 5200 3750
$EndSCHEMATC
