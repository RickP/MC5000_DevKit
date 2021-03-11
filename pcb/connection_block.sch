EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 17 11
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
L Connector:Conn_01x04_Female J24
U 1 1 60383D01
P 5800 3550
F 0 "J24" H 5828 3526 50  0000 L CNN
F 1 "Conn_01x04_Female" H 5828 3435 50  0000 L CNN
F 2 "LCSC_parts:PinSocket_1x04_P2.54mm_Vertical_centered" H 5800 3550 50  0001 C CNN
F 3 "~" H 5800 3550 50  0001 C CNN
F 4 "C124413" H 5800 3550 50  0001 C CNN "LCSC"
	1    5800 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Female J25
U 1 1 6038543F
P 5800 2900
F 0 "J25" H 5828 2926 50  0000 L CNN
F 1 "Conn_01x05_Female" H 5828 2835 50  0000 L CNN
F 2 "LCSC_parts:PinSocket_1x05_P2.54mm_Vertical_centered" H 5800 2900 50  0001 C CNN
F 3 "~" H 5800 2900 50  0001 C CNN
F 4 "C718240" H 5800 2900 50  0001 C CNN "LCSC"
	1    5800 2900
	1    0    0    -1  
$EndComp
Text GLabel 5300 3450 0    50   Input ~ 0
SWITCH_1
Wire Wire Line
	5300 3450 5600 3450
Text GLabel 5300 3550 0    50   Input ~ 0
SWITCH_2
Wire Wire Line
	5300 3550 5600 3550
Text GLabel 5300 3650 0    50   Input ~ 0
TEMP_SENSOR
Wire Wire Line
	5300 3650 5600 3650
Text GLabel 5300 3750 0    50   Input ~ 0
LIGHT_SENSOR
Wire Wire Line
	5300 3750 5600 3750
Text GLabel 5300 2700 0    50   Input ~ 0
DISPLAY
Text GLabel 5300 2800 0    50   Input ~ 0
BUZZER
Text GLabel 5300 2900 0    50   Input ~ 0
LED_RED
Text GLabel 5300 3000 0    50   Input ~ 0
LED_YELLOW
Text GLabel 5300 3100 0    50   Input ~ 0
LED_GREEN
Wire Wire Line
	5300 2700 5600 2700
Wire Wire Line
	5300 2800 5600 2800
Wire Wire Line
	5300 2900 5600 2900
Wire Wire Line
	5300 3000 5600 3000
Wire Wire Line
	5300 3100 5600 3100
$EndSCHEMATC
