EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 10 15
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5100 4350 5400 4350
$Comp
L power:VCC #PWR?
U 1 1 60350349
P 5400 3700
AR Path="/60308700/60350349" Ref="#PWR?"  Part="1" 
AR Path="/6034DF24/60350349" Ref="#PWR013"  Part="1" 
AR Path="/603599D1/60350349" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 5400 3550 50  0001 C CNN
F 1 "VCC" H 5415 3873 50  0000 C CNN
F 2 "" H 5400 3700 50  0001 C CNN
F 3 "" H 5400 3700 50  0001 C CNN
	1    5400 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6035034F
P 4950 3900
AR Path="/60308700/6035034F" Ref="#PWR?"  Part="1" 
AR Path="/6034DF24/6035034F" Ref="#PWR011"  Part="1" 
AR Path="/603599D1/6035034F" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 4950 3650 50  0001 C CNN
F 1 "GND" V 4955 3772 50  0000 R CNN
F 2 "" H 4950 3900 50  0001 C CNN
F 3 "" H 4950 3900 50  0001 C CNN
	1    4950 3900
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 60350355
P 5200 3900
AR Path="/60308700/60350355" Ref="C?"  Part="1" 
AR Path="/6034DF24/60350355" Ref="C5"  Part="1" 
AR Path="/603599D1/60350355" Ref="C8"  Part="1" 
F 0 "C8" V 4948 3900 50  0000 C CNN
F 1 "0.1uF" V 5039 3900 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5238 3750 50  0001 C CNN
F 3 "~" H 5200 3900 50  0001 C CNN
F 4 "C14663" H 5200 3900 50  0001 C CNN "LCSC"
	1    5200 3900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5350 3900 5400 3900
Connection ~ 5400 3900
Wire Wire Line
	5400 3900 5400 3700
Wire Wire Line
	5050 3900 4950 3900
Wire Wire Line
	5400 3900 5400 4350
Wire Wire Line
	6100 4350 6100 3900
$Comp
L power:GND #PWR?
U 1 1 60350361
P 6100 3900
AR Path="/60308700/60350361" Ref="#PWR?"  Part="1" 
AR Path="/6034DF24/60350361" Ref="#PWR016"  Part="1" 
AR Path="/603599D1/60350361" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 6100 3650 50  0001 C CNN
F 1 "GND" H 6105 3727 50  0000 C CNN
F 2 "" H 6100 3900 50  0001 C CNN
F 3 "" H 6100 3900 50  0001 C CNN
	1    6100 3900
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Female J?
U 1 1 60350368
P 6600 4550
AR Path="/60308700/60350368" Ref="J?"  Part="1" 
AR Path="/6034DF24/60350368" Ref="J18"  Part="1" 
AR Path="/603599D1/60350368" Ref="J21"  Part="1" 
F 0 "J21" H 6550 4800 50  0000 L CNN
F 1 "Conn_01x04_Female" H 6250 4250 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 6600 4550 50  0001 C CNN
F 3 "~" H 6600 4550 50  0001 C CNN
F 4 "C124413" H 6600 4550 50  0001 C CNN "LCSC"
	1    6600 4550
	1    0    0    1   
$EndComp
Wire Wire Line
	6100 4350 6400 4350
$Comp
L Connector:Conn_01x04_Female J?
U 1 1 60350370
P 4900 4450
AR Path="/60308700/60350370" Ref="J?"  Part="1" 
AR Path="/6034DF24/60350370" Ref="J16"  Part="1" 
AR Path="/603599D1/60350370" Ref="J19"  Part="1" 
F 0 "J19" H 4900 4150 50  0000 C CNN
F 1 "Conn_01x04_Female" H 4900 4650 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 4900 4450 50  0001 C CNN
F 3 "~" H 4900 4450 50  0001 C CNN
F 4 "C124413" H 4900 4450 50  0001 C CNN "LCSC"
	1    4900 4450
	-1   0    0    -1  
$EndComp
Text GLabel 5250 4800 3    50   Input ~ 0
RXD
Text GLabel 5950 4350 1    50   Input ~ 0
TXD
Wire Wire Line
	5950 4350 5950 4450
Wire Wire Line
	5950 4450 6400 4450
Wire Wire Line
	5100 4650 5250 4650
Wire Wire Line
	5250 4650 5250 4800
Wire Wire Line
	5100 4450 5600 4450
Wire Wire Line
	5600 4450 5600 5400
$Comp
L Connector:Conn_01x04_Female J?
U 1 1 60354EB8
P 5600 5600
AR Path="/60308700/60354EB8" Ref="J?"  Part="1" 
AR Path="/6034DF24/60354EB8" Ref="J17"  Part="1" 
AR Path="/603599D1/60354EB8" Ref="J20"  Part="1" 
F 0 "J20" H 5550 5850 50  0000 L CNN
F 1 "Conn_01x04_Female" H 5250 5300 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 5600 5600 50  0001 C CNN
F 3 "~" H 5600 5600 50  0001 C CNN
F 4 "C124413" H 5600 5600 50  0001 C CNN "LCSC"
	1    5600 5600
	0    -1   1    0   
$EndComp
Wire Wire Line
	5500 5400 5500 5300
Wire Wire Line
	5100 4550 5500 4550
$Comp
L Device:R R14
U 1 1 603553A1
P 5600 4200
AR Path="/6034DF24/603553A1" Ref="R14"  Part="1" 
AR Path="/603599D1/603553A1" Ref="R18"  Part="1" 
F 0 "R18" H 5670 4246 50  0000 L CNN
F 1 "1M" H 5670 4155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5530 4200 50  0001 C CNN
F 3 "~" H 5600 4200 50  0001 C CNN
F 4 "C22935" H 5600 4200 50  0001 C CNN "LCSC"
	1    5600 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60355E43
P 5600 3900
AR Path="/60308700/60355E43" Ref="#PWR?"  Part="1" 
AR Path="/6034DF24/60355E43" Ref="#PWR014"  Part="1" 
AR Path="/603599D1/60355E43" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 5600 3650 50  0001 C CNN
F 1 "GND" H 5605 3727 50  0000 C CNN
F 2 "" H 5600 3900 50  0001 C CNN
F 3 "" H 5600 3900 50  0001 C CNN
	1    5600 3900
	-1   0    0    1   
$EndComp
Wire Wire Line
	5600 4050 5600 3900
Wire Wire Line
	5600 4350 5600 4450
Connection ~ 5600 4450
$Comp
L Device:R R13
U 1 1 603567F0
P 5250 5300
AR Path="/6034DF24/603567F0" Ref="R13"  Part="1" 
AR Path="/603599D1/603567F0" Ref="R17"  Part="1" 
F 0 "R17" V 5043 5300 50  0000 C CNN
F 1 "1M" V 5134 5300 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5180 5300 50  0001 C CNN
F 3 "~" H 5250 5300 50  0001 C CNN
F 4 "C22935" H 5250 5300 50  0001 C CNN "LCSC"
	1    5250 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 5300 5500 5300
Connection ~ 5500 5300
Wire Wire Line
	5500 5300 5500 4550
$Comp
L power:GND #PWR?
U 1 1 6035723E
P 4950 5300
AR Path="/60308700/6035723E" Ref="#PWR?"  Part="1" 
AR Path="/6034DF24/6035723E" Ref="#PWR012"  Part="1" 
AR Path="/603599D1/6035723E" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 4950 5050 50  0001 C CNN
F 1 "GND" H 4955 5127 50  0000 C CNN
F 2 "" H 4950 5300 50  0001 C CNN
F 3 "" H 4950 5300 50  0001 C CNN
	1    4950 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 5300 5100 5300
Wire Wire Line
	6250 4650 6400 4650
$Comp
L Device:C C6
U 1 1 60358F4E
P 5850 4950
AR Path="/6034DF24/60358F4E" Ref="C6"  Part="1" 
AR Path="/603599D1/60358F4E" Ref="C9"  Part="1" 
F 0 "C9" V 5598 4950 50  0000 C CNN
F 1 "0.1uF" V 5689 4950 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 5888 4800 50  0001 C CNN
F 3 "~" H 5850 4950 50  0001 C CNN
F 4 "C14663" H 5850 4950 50  0001 C CNN "LCSC"
	1    5850 4950
	0    1    1    0   
$EndComp
$Comp
L Device:R R15
U 1 1 6035BBD0
P 5850 4550
AR Path="/6034DF24/6035BBD0" Ref="R15"  Part="1" 
AR Path="/603599D1/6035BBD0" Ref="R19"  Part="1" 
F 0 "R19" V 5643 4550 50  0000 C CNN
F 1 "2K2" V 5734 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5780 4550 50  0001 C CNN
F 3 "~" H 5850 4550 50  0001 C CNN
F 4 "C4190" H 5850 4550 50  0001 C CNN "LCSC"
	1    5850 4550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6035DD5C
P 6050 4800
AR Path="/60308700/6035DD5C" Ref="#PWR?"  Part="1" 
AR Path="/6034DF24/6035DD5C" Ref="#PWR015"  Part="1" 
AR Path="/603599D1/6035DD5C" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 6050 4550 50  0001 C CNN
F 1 "GND" H 6050 4650 50  0000 C CNN
F 2 "" H 6050 4800 50  0001 C CNN
F 3 "" H 6050 4800 50  0001 C CNN
	1    6050 4800
	-1   0    0    1   
$EndComp
Wire Wire Line
	5800 5100 5800 5400
$Comp
L Device:R R16
U 1 1 60361E47
P 6250 4900
AR Path="/6034DF24/60361E47" Ref="R16"  Part="1" 
AR Path="/603599D1/60361E47" Ref="R20"  Part="1" 
F 0 "R20" H 6180 4854 50  0000 R CNN
F 1 "2K2" H 6180 4945 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 6180 4900 50  0001 C CNN
F 3 "~" H 6250 4900 50  0001 C CNN
F 4 "C4190" H 6250 4900 50  0001 C CNN "LCSC"
	1    6250 4900
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 4750 6250 4650
Wire Wire Line
	6250 5050 6250 5100
Wire Wire Line
	5800 5100 6250 5100
$Comp
L Device:C C7
U 1 1 603636E1
P 6250 5300
AR Path="/6034DF24/603636E1" Ref="C7"  Part="1" 
AR Path="/603599D1/603636E1" Ref="C10"  Part="1" 
F 0 "C10" H 6135 5254 50  0000 R CNN
F 1 "0.1uF" H 6135 5345 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 6288 5150 50  0001 C CNN
F 3 "~" H 6250 5300 50  0001 C CNN
F 4 "C14663" H 6250 5300 50  0001 C CNN "LCSC"
	1    6250 5300
	-1   0    0    1   
$EndComp
Wire Wire Line
	6250 5150 6250 5100
Connection ~ 6250 5100
Wire Wire Line
	6250 5450 6250 5550
$Comp
L power:GND #PWR?
U 1 1 60364D99
P 6250 5550
AR Path="/60308700/60364D99" Ref="#PWR?"  Part="1" 
AR Path="/6034DF24/60364D99" Ref="#PWR017"  Part="1" 
AR Path="/603599D1/60364D99" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 6250 5300 50  0001 C CNN
F 1 "GND" H 6250 5400 50  0000 C CNN
F 2 "" H 6250 5550 50  0001 C CNN
F 3 "" H 6250 5550 50  0001 C CNN
	1    6250 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4550 6400 4550
Connection ~ 5700 4950
Wire Wire Line
	5700 4950 5700 5400
Wire Wire Line
	5700 4550 5700 4950
Wire Wire Line
	6000 4950 6050 4950
Wire Wire Line
	6050 4800 6050 4950
$EndSCHEMATC
