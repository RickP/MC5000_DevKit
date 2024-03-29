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
$Comp
L Switch:SW_Push SW1
U 1 1 6037B734
P 4500 3700
AR Path="/6037B4F8/6037B734" Ref="SW1"  Part="1" 
AR Path="/603926D8/6037B734" Ref="SW2"  Part="1" 
AR Path="/6038A837/6037B734" Ref="SW?"  Part="1" 
F 0 "SW1" H 4500 3985 50  0000 C CNN
F 1 "SW_MEC_5E" H 4500 3894 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_CK_KSC6xxJ" H 4500 3900 50  0001 C CNN
F 3 "~" H 4500 3900 50  0001 C CNN
F 4 "C318889" H 4500 3700 50  0001 C CNN "LCSC"
	1    4500 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R33
U 1 1 6037B996
P 5150 3700
AR Path="/6037B4F8/6037B996" Ref="R33"  Part="1" 
AR Path="/603926D8/6037B996" Ref="R35"  Part="1" 
AR Path="/6038A837/6037B996" Ref="R?"  Part="1" 
F 0 "R33" V 4943 3700 50  0000 C CNN
F 1 "220R" V 5034 3700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 5080 3700 50  0001 C CNN
F 3 "~" H 5150 3700 50  0001 C CNN
F 4 "C22962" H 5150 3700 50  0001 C CNN "LCSC"
	1    5150 3700
	0    1    1    0   
$EndComp
$Comp
L Device:R R32
U 1 1 6037CBB9
P 4850 4000
AR Path="/6037B4F8/6037CBB9" Ref="R32"  Part="1" 
AR Path="/603926D8/6037CBB9" Ref="R34"  Part="1" 
AR Path="/6038A837/6037CBB9" Ref="R?"  Part="1" 
F 0 "R32" H 4780 3954 50  0000 R CNN
F 1 "100K" H 4780 4045 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 4780 4000 50  0001 C CNN
F 3 "~" H 4850 4000 50  0001 C CNN
F 4 "C25803" H 4850 4000 50  0001 C CNN "LCSC"
	1    4850 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4700 3700 4850 3700
Wire Wire Line
	4850 3850 4850 3700
Connection ~ 4850 3700
Wire Wire Line
	4850 3700 5000 3700
Wire Wire Line
	5300 3700 5750 3700
Text GLabel 5750 3700 2    50   Input ~ 0
SWITCH_1
$Comp
L power:GND #PWR0115
U 1 1 6037D460
P 4850 4300
AR Path="/6037B4F8/6037D460" Ref="#PWR0115"  Part="1" 
AR Path="/603926D8/6037D460" Ref="#PWR0118"  Part="1" 
AR Path="/6038A837/6037D460" Ref="#PWR?"  Part="1" 
F 0 "#PWR0115" H 4850 4050 50  0001 C CNN
F 1 "GND" H 4855 4127 50  0000 C CNN
F 2 "" H 4850 4300 50  0001 C CNN
F 3 "" H 4850 4300 50  0001 C CNN
	1    4850 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4300 4850 4150
$Comp
L power:VCC #PWR0117
U 1 1 6037DA63
P 4200 3700
AR Path="/6037B4F8/6037DA63" Ref="#PWR0117"  Part="1" 
AR Path="/603926D8/6037DA63" Ref="#PWR0120"  Part="1" 
AR Path="/6038A837/6037DA63" Ref="#PWR?"  Part="1" 
F 0 "#PWR0117" H 4200 3550 50  0001 C CNN
F 1 "VCC" V 4215 3827 50  0000 L CNN
F 2 "" H 4200 3700 50  0001 C CNN
F 3 "" H 4200 3700 50  0001 C CNN
	1    4200 3700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4200 3700 4300 3700
$Comp
L Switch:SW_Push SW2
U 1 1 6039577E
P 6300 4550
AR Path="/6037B4F8/6039577E" Ref="SW2"  Part="1" 
AR Path="/603926D8/6039577E" Ref="SW?"  Part="1" 
AR Path="/6038A837/6039577E" Ref="SW?"  Part="1" 
F 0 "SW2" H 6300 4835 50  0000 C CNN
F 1 "SW_MEC_5E" H 6300 4744 50  0000 C CNN
F 2 "Button_Switch_SMD:SW_Push_1P1T_NO_CK_KSC6xxJ" H 6300 4750 50  0001 C CNN
F 3 "~" H 6300 4750 50  0001 C CNN
F 4 "C318889" H 6300 4550 50  0001 C CNN "LCSC"
	1    6300 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R35
U 1 1 60395784
P 6950 4550
AR Path="/6037B4F8/60395784" Ref="R35"  Part="1" 
AR Path="/603926D8/60395784" Ref="R?"  Part="1" 
AR Path="/6038A837/60395784" Ref="R?"  Part="1" 
F 0 "R35" V 6743 4550 50  0000 C CNN
F 1 "220R" V 6834 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 6880 4550 50  0001 C CNN
F 3 "~" H 6950 4550 50  0001 C CNN
F 4 "C22962" H 6950 4550 50  0001 C CNN "LCSC"
	1    6950 4550
	0    1    1    0   
$EndComp
$Comp
L Device:R R34
U 1 1 6039578A
P 6650 4850
AR Path="/6037B4F8/6039578A" Ref="R34"  Part="1" 
AR Path="/603926D8/6039578A" Ref="R?"  Part="1" 
AR Path="/6038A837/6039578A" Ref="R?"  Part="1" 
F 0 "R34" H 6580 4804 50  0000 R CNN
F 1 "100K" H 6580 4895 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder" V 6580 4850 50  0001 C CNN
F 3 "~" H 6650 4850 50  0001 C CNN
F 4 "C25803" H 6650 4850 50  0001 C CNN "LCSC"
	1    6650 4850
	-1   0    0    1   
$EndComp
Wire Wire Line
	6500 4550 6650 4550
Wire Wire Line
	6650 4700 6650 4550
Connection ~ 6650 4550
Wire Wire Line
	6650 4550 6800 4550
Wire Wire Line
	7100 4550 7550 4550
Text GLabel 7550 4550 2    50   Input ~ 0
SWITCH_2
$Comp
L power:GND #PWR0118
U 1 1 60395796
P 6650 5150
AR Path="/6037B4F8/60395796" Ref="#PWR0118"  Part="1" 
AR Path="/603926D8/60395796" Ref="#PWR?"  Part="1" 
AR Path="/6038A837/60395796" Ref="#PWR?"  Part="1" 
F 0 "#PWR0118" H 6650 4900 50  0001 C CNN
F 1 "GND" H 6655 4977 50  0000 C CNN
F 2 "" H 6650 5150 50  0001 C CNN
F 3 "" H 6650 5150 50  0001 C CNN
	1    6650 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 5150 6650 5000
$Comp
L power:VCC #PWR0120
U 1 1 6039579D
P 6000 4550
AR Path="/6037B4F8/6039579D" Ref="#PWR0120"  Part="1" 
AR Path="/603926D8/6039579D" Ref="#PWR?"  Part="1" 
AR Path="/6038A837/6039579D" Ref="#PWR?"  Part="1" 
F 0 "#PWR0120" H 6000 4400 50  0001 C CNN
F 1 "VCC" V 6015 4677 50  0000 L CNN
F 2 "" H 6000 4550 50  0001 C CNN
F 3 "" H 6000 4550 50  0001 C CNN
	1    6000 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6000 4550 6100 4550
$EndSCHEMATC
