EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "Higgs"
Date "2021-05-30"
Rev "1.0"
Comp "Fibration"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch:SW_SPDT SW?
U 1 1 60B49D76
P 5650 4650
F 0 "SW?" H 5650 4935 50  0000 C CNN
F 1 "SW_SPDT" H 5650 4844 50  0000 C CNN
F 2 "" H 5650 4650 50  0001 C CNN
F 3 "~" H 5650 4650 50  0001 C CNN
	1    5650 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4650 5400 4650
$Comp
L power:+3.3V #PWR0103
U 1 1 60B4C024
P 5900 4500
F 0 "#PWR0103" H 5900 4350 50  0001 C CNN
F 1 "+3.3V" H 5915 4673 50  0000 C CNN
F 2 "" H 5900 4500 50  0001 C CNN
F 3 "" H 5900 4500 50  0001 C CNN
	1    5900 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 60B4C5FD
P 5900 4800
F 0 "#PWR0104" H 5900 4550 50  0001 C CNN
F 1 "GND" H 5905 4627 50  0000 C CNN
F 2 "" H 5900 4800 50  0001 C CNN
F 3 "" H 5900 4800 50  0001 C CNN
	1    5900 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4800 5900 4750
Wire Wire Line
	5900 4750 5850 4750
Wire Wire Line
	5900 4500 5900 4550
Wire Wire Line
	5900 4550 5850 4550
Text GLabel 5150 4650 0    50   Input ~ 0
BOOT0
Wire Wire Line
	5150 4650 5200 4650
Text Notes 4850 4900 0    50   ~ 0
TODO: is this necessary\n
$Comp
L Device:R_Small R?
U 1 1 60B4B181
P 5300 4650
F 0 "R?" V 5200 4650 50  0000 C CNN
F 1 "10K" V 5400 4650 50  0000 C CNN
F 2 "" H 5300 4650 50  0001 C CNN
F 3 "~" H 5300 4650 50  0001 C CNN
	1    5300 4650
	0    1    1    0   
$EndComp
$Sheet
S 3600 2750 950  500 
U 62285344
F0 "power" 50
F1 "power.sch" 50
$EndSheet
$Sheet
S 5100 2750 1000 500 
U 622A682E
F0 "digital" 50
F1 "digital.sch" 50
$EndSheet
$Sheet
S 6600 2750 1000 500 
U 622A6999
F0 "analog" 50
F1 "analog.sch" 50
$EndSheet
$EndSCHEMATC
