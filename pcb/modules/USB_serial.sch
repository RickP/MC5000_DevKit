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
F 4 "C404969" H 3600 3550 50  0001 C CNN "LCSC"
	1    3600 3550
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
Connection ~ 3600 3950
Wire Wire Line
	3500 3950 3600 3950
Wire Wire Line
	3600 4150 3600 3950
$Comp
L Device:C C1
U 1 1 6030ED7F
P 5850 3250
F 0 "C1" H 5965 3296 50  0000 L CNN
F 1 "0.1uF" H 5965 3205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5888 3100 50  0001 C CNN
F 3 "~" H 5850 3250 50  0001 C CNN
F 4 "C14663" H 5850 3250 50  0001 C CNN "LCSC"
	1    5850 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 3100 5850 2950
$Comp
L power:GND #PWR04
U 1 1 6031068E
P 5850 2950
F 0 "#PWR04" H 5850 2700 50  0001 C CNN
F 1 "GND" H 5855 2777 50  0000 C CNN
F 2 "" H 5850 2950 50  0001 C CNN
F 3 "" H 5850 2950 50  0001 C CNN
	1    5850 2950
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR05
U 1 1 60310A8D
P 5550 2500
F 0 "#PWR05" H 5550 2350 50  0001 C CNN
F 1 "VCC" H 5565 2673 50  0000 C CNN
F 2 "" H 5550 2500 50  0001 C CNN
F 3 "" H 5550 2500 50  0001 C CNN
	1    5550 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 2650 5400 2650
Connection ~ 5550 2650
Wire Wire Line
	5550 2650 5550 2500
$Comp
L Device:C C2
U 1 1 60312027
P 5250 2650
F 0 "C2" V 4998 2650 50  0000 C CNN
F 1 "0.1uF" V 5089 2650 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5288 2500 50  0001 C CNN
F 3 "~" H 5250 2650 50  0001 C CNN
F 4 "C14663" H 5250 2650 50  0001 C CNN "LCSC"
	1    5250 2650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR06
U 1 1 603127D8
P 4950 2650
F 0 "#PWR06" H 4950 2400 50  0001 C CNN
F 1 "GND" V 4955 2522 50  0000 R CNN
F 2 "" H 4950 2650 50  0001 C CNN
F 3 "" H 4950 2650 50  0001 C CNN
	1    4950 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 2650 5100 2650
Text GLabel 4100 2400 1    50   Input ~ 0
RXD
Text GLabel 4200 2700 1    50   Input ~ 0
TXD
$Comp
L LCSC_parts:CH340G U5
U 1 1 6050214F
P 4750 3450
F 0 "U5" H 4750 4005 50  0000 C CNN
F 1 "CH340G" H 4750 3914 50  0000 C CNN
F 2 "LCSC_parts:SOP-16_3.9x9.9mm_P1.27mm_rotated" H 4800 3900 50  0001 C CNN
F 3 "" H 4750 3450 50  0001 C CNN
F 4 "C14267" H 4750 3823 50  0000 C CNN "LCSC"
	1    4750 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3450 5850 3450
Wire Wire Line
	5850 3450 5850 3400
Wire Wire Line
	5200 3350 5550 3350
Wire Wire Line
	5550 2650 5550 3350
Wire Wire Line
	4100 2400 4100 3350
Wire Wire Line
	4100 3350 4300 3350
Wire Wire Line
	4200 2700 4200 3250
Wire Wire Line
	4200 3250 4300 3250
$Comp
L power:GND #PWR03
U 1 1 6050B107
P 4750 4100
F 0 "#PWR03" H 4750 3850 50  0001 C CNN
F 1 "GND" H 4755 3927 50  0000 C CNN
F 2 "" H 4750 4100 50  0001 C CNN
F 3 "" H 4750 4100 50  0001 C CNN
	1    4750 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3850 4750 4100
$Comp
L Device:C C13
U 1 1 6050BEE3
P 6250 3950
F 0 "C13" H 6365 3996 50  0000 L CNN
F 1 "33pF" H 6365 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6288 3800 50  0001 C CNN
F 3 "~" H 6250 3950 50  0001 C CNN
F 4 "C1663" H 6250 3950 50  0001 C CNN "LCSC"
	1    6250 3950
	0    1    1    0   
$EndComp
$Comp
L Device:C C12
U 1 1 6050C4DC
P 5250 3950
F 0 "C12" H 5365 3996 50  0000 L CNN
F 1 "33pF" H 5365 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5288 3800 50  0001 C CNN
F 3 "~" H 5250 3950 50  0001 C CNN
F 4 "C1663" H 5250 3950 50  0001 C CNN "LCSC"
	1    5250 3950
	0    1    1    0   
$EndComp
$Comp
L Device:Crystal_GND24 Y1
U 1 1 6050D0A3
P 5650 3950
F 0 "Y1" H 5700 4150 50  0000 L CNN
F 1 "Crystal_GND24" H 5700 3750 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm" H 5650 3950 50  0001 C CNN
F 3 "~" H 5650 3950 50  0001 C CNN
F 4 "C9002" H 5650 3950 50  0001 C CNN "LCSC"
	1    5650 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3550 6000 3550
Wire Wire Line
	6000 3550 6000 3950
Wire Wire Line
	6000 3950 5800 3950
Wire Wire Line
	6000 3950 6100 3950
Connection ~ 6000 3950
Wire Wire Line
	5400 3950 5450 3950
Wire Wire Line
	5450 3950 5450 3650
Wire Wire Line
	5450 3650 5200 3650
Connection ~ 5450 3950
Wire Wire Line
	5450 3950 5500 3950
$Comp
L power:GND #PWR028
U 1 1 6051287B
P 5000 3950
F 0 "#PWR028" H 5000 3700 50  0001 C CNN
F 1 "GND" H 5005 3777 50  0000 C CNN
F 2 "" H 5000 3950 50  0001 C CNN
F 3 "" H 5000 3950 50  0001 C CNN
	1    5000 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	5000 3950 5100 3950
$Comp
L power:GND #PWR029
U 1 1 6051426C
P 5650 3700
F 0 "#PWR029" H 5650 3450 50  0001 C CNN
F 1 "GND" H 5500 3650 50  0000 C CNN
F 2 "" H 5650 3700 50  0001 C CNN
F 3 "" H 5650 3700 50  0001 C CNN
	1    5650 3700
	-1   0    0    1   
$EndComp
Wire Wire Line
	5650 3700 5650 3750
$Comp
L power:GND #PWR030
U 1 1 60515413
P 5650 4300
F 0 "#PWR030" H 5650 4050 50  0001 C CNN
F 1 "GND" H 5500 4250 50  0000 C CNN
F 2 "" H 5650 4300 50  0001 C CNN
F 3 "" H 5650 4300 50  0001 C CNN
	1    5650 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4300 5650 4150
$Comp
L power:GND #PWR031
U 1 1 605160C4
P 6550 3950
F 0 "#PWR031" H 6550 3700 50  0001 C CNN
F 1 "GND" H 6400 3900 50  0000 C CNN
F 2 "" H 6550 3950 50  0001 C CNN
F 3 "" H 6550 3950 50  0001 C CNN
	1    6550 3950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6400 3950 6550 3950
$EndSCHEMATC
