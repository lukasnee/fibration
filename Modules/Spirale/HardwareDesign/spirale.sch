EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Spirale"
Date "2021-05-30"
Rev "1.0"
Comp "Fibration"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32F3:STM32F303RETx U?
U 1 1 60B41874
P 6100 4050
F 0 "U?" H 6650 5800 50  0000 C CNN
F 1 "STM32F303RETx" H 6700 2250 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 5500 2350 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00118585.pdf" H 6100 4050 50  0001 C CNN
	1    6100 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60B48111
P 5900 5900
F 0 "#PWR?" H 5900 5650 50  0001 C CNN
F 1 "GND" H 5905 5727 50  0000 C CNN
F 2 "" H 5900 5900 50  0001 C CNN
F 3 "" H 5900 5900 50  0001 C CNN
	1    5900 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 5900 5900 5850
Wire Wire Line
	5900 5850 6000 5850
Connection ~ 5900 5850
Wire Wire Line
	6100 5850 6000 5850
Connection ~ 6000 5850
Connection ~ 6100 5850
Wire Wire Line
	6100 5850 6200 5850
Connection ~ 6200 5850
Wire Wire Line
	6200 5850 6300 5850
$Comp
L power:+3.3V #PWR?
U 1 1 60B49EE9
P 5900 2200
F 0 "#PWR?" H 5900 2050 50  0001 C CNN
F 1 "+3.3V" H 5915 2373 50  0000 C CNN
F 2 "" H 5900 2200 50  0001 C CNN
F 3 "" H 5900 2200 50  0001 C CNN
	1    5900 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 2250 5900 2200
Wire Wire Line
	5900 2250 6000 2250
Connection ~ 5900 2250
Connection ~ 6000 2250
Wire Wire Line
	6000 2250 6100 2250
Connection ~ 6100 2250
Wire Wire Line
	6100 2250 6200 2250
Connection ~ 6200 2250
Wire Wire Line
	6200 2250 6300 2250
$Comp
L power:+3.3VA #PWR?
U 1 1 60B4B505
P 6400 2200
F 0 "#PWR?" H 6400 2050 50  0001 C CNN
F 1 "+3.3VA" H 6415 2373 50  0000 C CNN
F 2 "" H 6400 2200 50  0001 C CNN
F 3 "" H 6400 2200 50  0001 C CNN
	1    6400 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2200 6400 2250
$EndSCHEMATC
