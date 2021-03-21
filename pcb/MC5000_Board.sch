EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 15
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 5000 1400 1600 1000
U 602FF29A
F0 "USB serial" 50
F1 "modules/USB_serial.sch" 50
$EndSheet
$Sheet
S 7100 1500 1600 1100
U 60303416
F0 "PFS172 breakout - MCU1" 50
F1 "modules/PFS172_breakout.sch" 50
$EndSheet
$Sheet
S 9000 1500 1600 1050
U 6030344A
F0 "PFS172 breakout - MCU2" 50
F1 "modules/PFS172_breakout.sch" 50
$EndSheet
$Sheet
S 2900 2900 1600 1000
U 60308700
F0 "Display" 50
F1 "modules/display.sch" 50
$EndSheet
$Sheet
S 7100 3000 1600 1050
U 6034DF24
F0 "MCU5000 #1" 50
F1 "modules/MCU5000.sch" 50
$EndSheet
$Sheet
S 9000 3000 1600 1050
U 603599D1
F0 "MCU5000 #2" 50
F1 "modules/MCU5000.sch" 50
$EndSheet
$Sheet
S 1000 2900 1600 1000
U 603632C9
F0 "Buzzer" 50
F1 "modules/buzzer.sch" 50
$EndSheet
$Sheet
S 1000 4300 1600 1000
U 6036720E
F0 "LEDs" 50
F1 "modules/LEDs.sch" 50
$EndSheet
$Sheet
S 1000 6200 1600 1000
U 6037B4F8
F0 "Switches" 50
F1 "modules/switches.sch" 50
$EndSheet
$Sheet
S 2800 6200 1600 1000
U 6038A837
F0 "Temp sensor" 50
F1 "modules/temp_sensor.sch" 50
$EndSheet
$Sheet
S 4600 6200 1500 1000
U 6038ECB3
F0 "Light sensor" 50
F1 "modules/light_sensor.sch" 50
$EndSheet
$Sheet
S 5000 4200 1600 1050
U 603839E8
F0 "Connection block" 50
F1 "connection_block.sch" 50
$EndSheet
$Sheet
S 2900 1500 1600 1050
U 602FB936
F0 "PFS172 breakout - Display Ctrl" 50
F1 "modules/PFS172_breakout.sch" 50
$EndSheet
$Sheet
S 1000 1500 1600 1050
U 603034BC
F0 "PFS172 breakout - Buzzer" 50
F1 "modules/PFS172_breakout.sch" 50
$EndSheet
Text Notes 7100 1200 0    150  ~ 0
Microcontrollers
Wire Notes Line
	6900 800  6900 4300
Wire Notes Line
	6900 4300 10800 4300
Wire Notes Line
	10800 4300 10800 800 
Wire Notes Line
	10800 800  6900 800 
Text Notes 5000 3900 0    150  ~ 0
Interconnect
Wire Notes Line
	4800 3600 6800 3600
Wire Notes Line
	6800 3600 6800 5500
Wire Notes Line
	6800 5500 4800 5500
Wire Notes Line
	4800 5500 4800 3600
Text Notes 5000 1200 0    150  ~ 0
USB
Wire Notes Line
	4800 800  6800 800 
Wire Notes Line
	6800 800  6800 2700
Wire Notes Line
	6800 2700 4800 2700
Wire Notes Line
	4800 2700 4800 800 
Text Notes 1000 1200 0    150  ~ 0
Output
Wire Notes Line
	800  800  4700 800 
Wire Notes Line
	4700 800  4700 5500
Wire Notes Line
	4700 5500 800  5500
Wire Notes Line
	800  5500 800  800 
Text Notes 1000 5900 0    150  ~ 0
Input
Wire Notes Line
	800  5600 6300 5600
Wire Notes Line
	6300 5600 6300 7400
Wire Notes Line
	6300 7400 800  7400
Wire Notes Line
	800  7400 800  5600
$EndSCHEMATC
