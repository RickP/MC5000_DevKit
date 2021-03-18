EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 15
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
L LCSC_parts:MLT-7525 BZ1
U 1 1 60044231
P 5950 3500
F 0 "BZ1" H 6102 3529 50  0000 L CNN
F 1 "MLT-7525" H 6102 3438 50  0000 L CNN
F 2 "LCSC_parts:MagneticBuzzer_7.5x7.5_SMT" H 5950 3250 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1811141116_Jiangsu-Huaneng-Elec-MLT-7525_C95299.pdf" V 5925 3600 50  0001 C CNN
F 4 "C95299" H 5950 3500 50  0001 C CNN "LCSC"
	1    5950 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:D D2
U 1 1 60045237
P 5400 3500
F 0 "D2" V 5354 3580 50  0000 L CNN
F 1 "D" V 5445 3580 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123F" H 5400 3500 50  0001 C CNN
F 3 " 1903051030_MDD-Jiangsu-Yutai-Elec-SM4007PL_C64898.pdf ~" H 5400 3500 50  0001 C CNN
F 4 "C64898" H 5400 3500 50  0001 C CNN "LCSC"
	1    5400 3500
	0    1    1    0   
$EndComp
$Comp
L LCSC_parts:S9013 Q1
U 1 1 600481C3
P 5650 4000
F 0 "Q1" H 5840 4046 50  0000 L CNN
F 1 "S9013" H 5840 3955 50  0000 L CNN
F 2 "LCSC_parts:SOT-23_rotated" V 5900 3950 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/szlcsc/1912111437_Slkor-SLKORMICRO-Elec-S9013_C444724.pdf" H 5650 4000 50  0001 C CNN
F 4 "C6749" H 5650 4000 50  0001 C CNN "LCSC"
	1    5650 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 6004ADD9
P 5750 4350
F 0 "#PWR0116" H 5750 4100 50  0001 C CNN
F 1 "GND" H 5755 4177 50  0000 C CNN
F 2 "" H 5750 4350 50  0001 C CNN
F 3 "" H 5750 4350 50  0001 C CNN
	1    5750 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R21
U 1 1 6004B76C
P 4950 4000
F 0 "R21" V 4743 4000 50  0000 C CNN
F 1 "220R" V 4834 4000 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4880 4000 50  0001 C CNN
F 3 "~" H 4950 4000 50  0001 C CNN
F 4 "C22962" H 4950 4000 50  0001 C CNN "LCSC"
	1    4950 4000
	0    1    1    0   
$EndComp
$Comp
L Device:R R22
U 1 1 60060773
P 5300 4250
F 0 "R22" H 5230 4204 50  0000 R CNN
F 1 "100K" H 5230 4295 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5230 4250 50  0001 C CNN
F 3 "~" H 5300 4250 50  0001 C CNN
F 4 "C25803" H 5300 4250 50  0001 C CNN "LCSC"
	1    5300 4250
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 60061F85
P 5300 4550
F 0 "#PWR0119" H 5300 4300 50  0001 C CNN
F 1 "GND" H 5305 4377 50  0000 C CNN
F 2 "" H 5300 4550 50  0001 C CNN
F 3 "" H 5300 4550 50  0001 C CNN
	1    5300 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 3200 3600 3200
$Comp
L power:VCC #PWR?
U 1 1 6037FE8F
P 3600 2550
AR Path="/60308700/6037FE8F" Ref="#PWR?"  Part="1" 
AR Path="/603632C9/6037FE8F" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 3600 2400 50  0001 C CNN
F 1 "VCC" H 3615 2723 50  0000 C CNN
F 2 "" H 3600 2550 50  0001 C CNN
F 3 "" H 3600 2550 50  0001 C CNN
	1    3600 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6037FE95
P 3150 2750
AR Path="/60308700/6037FE95" Ref="#PWR?"  Part="1" 
AR Path="/603632C9/6037FE95" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 3150 2500 50  0001 C CNN
F 1 "GND" V 3155 2622 50  0000 R CNN
F 2 "" H 3150 2750 50  0001 C CNN
F 3 "" H 3150 2750 50  0001 C CNN
	1    3150 2750
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 6037FE9B
P 3400 2750
AR Path="/60308700/6037FE9B" Ref="C?"  Part="1" 
AR Path="/603632C9/6037FE9B" Ref="C11"  Part="1" 
F 0 "C11" V 3148 2750 50  0000 C CNN
F 1 "0.1uF" V 3239 2750 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder" H 3438 2600 50  0001 C CNN
F 3 "~" H 3400 2750 50  0001 C CNN
F 4 "C14663" H 3400 2750 50  0001 C CNN "LCSC"
	1    3400 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3550 2750 3600 2750
Connection ~ 3600 2750
Wire Wire Line
	3600 2750 3600 2550
Wire Wire Line
	3250 2750 3150 2750
Wire Wire Line
	3600 2750 3600 3200
Wire Wire Line
	4300 3200 4300 2750
$Comp
L power:GND #PWR?
U 1 1 6037FEA7
P 4300 2750
AR Path="/60308700/6037FEA7" Ref="#PWR?"  Part="1" 
AR Path="/603632C9/6037FEA7" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 4300 2500 50  0001 C CNN
F 1 "GND" H 4305 2577 50  0000 C CNN
F 2 "" H 4300 2750 50  0001 C CNN
F 3 "" H 4300 2750 50  0001 C CNN
	1    4300 2750
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Female J?
U 1 1 6037FEAE
P 4800 3400
AR Path="/60308700/6037FEAE" Ref="J?"  Part="1" 
AR Path="/603632C9/6037FEAE" Ref="J23"  Part="1" 
F 0 "J23" H 4750 3650 50  0000 L CNN
F 1 "Conn_01x04_Female" H 4450 3100 50  0000 L CNN
F 2 "LCSC_parts:PinHeader_1x04_P2.54mm_Vertical_centered" H 4800 3400 50  0001 C CNN
F 3 "~" H 4800 3400 50  0001 C CNN
F 4 "C124378" H 4800 3400 50  0001 C CNN "LCSC"
	1    4800 3400
	1    0    0    1   
$EndComp
Wire Wire Line
	4300 3200 4600 3200
$Comp
L Connector:Conn_01x04_Female J?
U 1 1 6037FEB6
P 3100 3300
AR Path="/60308700/6037FEB6" Ref="J?"  Part="1" 
AR Path="/603632C9/6037FEB6" Ref="J22"  Part="1" 
F 0 "J22" H 3100 3000 50  0000 C CNN
F 1 "Conn_01x04_Female" H 3100 3500 50  0000 C CNN
F 2 "LCSC_parts:PinHeader_1x04_P2.54mm_Vertical_centered" H 3100 3300 50  0001 C CNN
F 3 "~" H 3100 3300 50  0001 C CNN
F 4 "C124378" H 3100 3300 50  0001 C CNN "LCSC"
	1    3100 3300
	-1   0    0    -1  
$EndComp
NoConn ~ 3300 3500
NoConn ~ 3300 3300
Text GLabel 3600 3750 0    50   Input ~ 0
BUZZER
Wire Wire Line
	3600 3750 3750 3750
Wire Wire Line
	4200 3500 4200 4000
Wire Wire Line
	4200 3500 4600 3500
Wire Wire Line
	3750 3400 3750 3750
Wire Wire Line
	3300 3400 3750 3400
NoConn ~ 4600 3300
NoConn ~ 4600 3400
$Comp
L power:VCC #PWR?
U 1 1 6039EC78
P 5750 3200
AR Path="/60308700/6039EC78" Ref="#PWR?"  Part="1" 
AR Path="/603632C9/6039EC78" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 5750 3050 50  0001 C CNN
F 1 "VCC" H 5765 3373 50  0000 C CNN
F 2 "" H 5750 3200 50  0001 C CNN
F 3 "" H 5750 3200 50  0001 C CNN
	1    5750 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 4000 4200 4000
Wire Wire Line
	5750 4350 5750 4200
Wire Wire Line
	5100 4000 5300 4000
Wire Wire Line
	5300 4100 5300 4000
Connection ~ 5300 4000
Wire Wire Line
	5300 4000 5450 4000
Wire Wire Line
	5300 4550 5300 4400
Wire Wire Line
	5400 3350 5750 3350
Wire Wire Line
	5850 3350 5850 3400
Wire Wire Line
	5850 3600 5850 3650
Wire Wire Line
	5850 3650 5750 3650
Wire Wire Line
	5750 3800 5750 3650
Connection ~ 5750 3650
Wire Wire Line
	5750 3650 5400 3650
Wire Wire Line
	5750 3200 5750 3350
Connection ~ 5750 3350
Wire Wire Line
	5750 3350 5850 3350
$Comp
L Device:R R?
U 1 1 60489C78
P 3750 4100
AR Path="/6034DF24/60489C78" Ref="R?"  Part="1" 
AR Path="/603599D1/60489C78" Ref="R?"  Part="1" 
AR Path="/603632C9/60489C78" Ref="R12"  Part="1" 
F 0 "R12" V 3543 4100 50  0000 C CNN
F 1 "1M" V 3634 4100 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 3680 4100 50  0001 C CNN
F 3 "~" H 3750 4100 50  0001 C CNN
F 4 "C22935" H 3750 4100 50  0001 C CNN "LCSC"
	1    3750 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3750 3750 3950
Connection ~ 3750 3750
$Comp
L power:GND #PWR023
U 1 1 6048A272
P 3750 4350
F 0 "#PWR023" H 3750 4100 50  0001 C CNN
F 1 "GND" H 3755 4177 50  0000 C CNN
F 2 "" H 3750 4350 50  0001 C CNN
F 3 "" H 3750 4350 50  0001 C CNN
	1    3750 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4250 3750 4350
$EndSCHEMATC
