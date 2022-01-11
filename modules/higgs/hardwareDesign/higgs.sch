EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L MCU_ST_STM32F3:STM32F303RETx U?
U 1 1 60B41874
P 6100 4050
F 0 "U?" H 6650 5800 50  0000 C CNN
F 1 "STM32F303RETx" H 6650 2300 50  0000 C CNN
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
Text GLabel 5350 2450 0    50   Input ~ 0
NRST
Text GLabel 5350 2650 0    50   Input ~ 0
BOOT0
$Comp
L Device:C_Small C?
U 1 1 60B485F3
P 4700 3950
F 0 "C?" H 4792 3996 50  0000 L CNN
F 1 "C_Small" H 4792 3905 50  0000 L CNN
F 2 "" H 4700 3950 50  0001 C CNN
F 3 "~" H 4700 3950 50  0001 C CNN
	1    4700 3950
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPDT SW?
U 1 1 60B49D76
P 2750 2500
F 0 "SW?" H 2750 2785 50  0000 C CNN
F 1 "SW_SPDT" H 2750 2694 50  0000 C CNN
F 2 "" H 2750 2500 50  0001 C CNN
F 3 "~" H 2750 2500 50  0001 C CNN
	1    2750 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2500 2500 2500
$Comp
L power:+3.3V #PWR?
U 1 1 60B4C024
P 3000 2350
F 0 "#PWR?" H 3000 2200 50  0001 C CNN
F 1 "+3.3V" H 3015 2523 50  0000 C CNN
F 2 "" H 3000 2350 50  0001 C CNN
F 3 "" H 3000 2350 50  0001 C CNN
	1    3000 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60B4C5FD
P 3000 2650
F 0 "#PWR?" H 3000 2400 50  0001 C CNN
F 1 "GND" H 3005 2477 50  0000 C CNN
F 2 "" H 3000 2650 50  0001 C CNN
F 3 "" H 3000 2650 50  0001 C CNN
	1    3000 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2650 3000 2600
Wire Wire Line
	3000 2600 2950 2600
Wire Wire Line
	3000 2350 3000 2400
Wire Wire Line
	3000 2400 2950 2400
Text GLabel 2250 2500 0    50   Input ~ 0
BOOT0
Wire Wire Line
	2250 2500 2300 2500
Text Notes 1950 2750 0    50   ~ 0
TODO: is this necessary\n
Text GLabel 5350 3650 0    50   BiDi ~ 0
HSE_IN
Text GLabel 5350 3750 0    50   BiDi ~ 0
HSE_OUT
Text GLabel 6850 3750 2    50   Input ~ 0
SWDIO
Text GLabel 6850 3850 2    50   Input ~ 0
SWCLK
Text GLabel 6850 4450 2    50   Input ~ 0
SWO
Text Notes 7200 4500 0    50   ~ 0
TODO: for debugger printf, could be useful\n
Text GLabel 6850 3650 2    50   BiDi ~ 0
USB_D+
Text GLabel 6850 3550 2    50   BiDi ~ 0
USB_D-
Text GLabel 6850 3050 2    50   Input ~ 0
ADC2_IN3
Text GLabel 6850 3150 2    50   Input ~ 0
ADC2_IN4
Wire Wire Line
	5400 2450 5350 2450
Wire Wire Line
	5350 2650 5400 2650
Wire Wire Line
	5350 3650 5400 3650
Wire Wire Line
	5400 3750 5350 3750
Wire Wire Line
	5350 4550 5400 4550
Wire Wire Line
	5400 4650 5350 4650
Wire Wire Line
	6850 4450 6800 4450
Wire Wire Line
	6850 3050 6800 3050
Wire Wire Line
	6800 3150 6850 3150
Wire Wire Line
	6850 3550 6800 3550
Wire Wire Line
	6800 3650 6850 3650
Wire Wire Line
	6850 3750 6800 3750
Wire Wire Line
	6800 3850 6850 3850
Text GLabel 6850 5250 2    50   Input ~ 0
ADC2_IN14
Wire Wire Line
	6850 5250 6800 5250
Wire Wire Line
	5400 4150 5350 4150
$Comp
L Device:R_Small R?
U 1 1 60B4B181
P 2400 2500
F 0 "R?" V 2300 2500 50  0000 C CNN
F 1 "10K" V 2500 2500 50  0000 C CNN
F 2 "" H 2400 2500 50  0001 C CNN
F 3 "~" H 2400 2500 50  0001 C CNN
	1    2400 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 4250 5350 4250
Wire Wire Line
	5350 4350 5400 4350
Wire Wire Line
	5400 4450 5350 4450
Text GLabel 6850 4350 2    50   Input ~ 0
ADC2_IN12
Wire Wire Line
	6850 4350 6800 4350
Text GLabel 5350 4450 0    50   Input ~ 0
ADC2_IN9
Text GLabel 5350 4350 0    50   Input ~ 0
ADC2_IN8
Text GLabel 5350 4250 0    50   Input ~ 0
ADC2_IN7
Text GLabel 5350 4150 0    50   Input ~ 0
ADC2_IN6
Text GLabel 5350 4650 0    50   Input ~ 0
ADC2_IN11
Text GLabel 5350 4550 0    50   Input ~ 0
ADC2_IN5
Text GLabel 9200 2350 0    50   Input ~ 0
ADC2_IN9
Text GLabel 9200 2250 0    50   Input ~ 0
ADC2_IN8
Text GLabel 9200 2150 0    50   Input ~ 0
ADC2_IN7
Text GLabel 9200 2050 0    50   Input ~ 0
ADC2_IN6
Text GLabel 9200 2550 0    50   Input ~ 0
ADC2_IN11
Text GLabel 9200 2450 0    50   Input ~ 0
ADC2_IN5
Text GLabel 9200 2750 0    50   Input ~ 0
ADC2_IN14
Text GLabel 9200 2650 0    50   Input ~ 0
ADC2_IN12
Text GLabel 9200 1850 0    50   Input ~ 0
ADC2_IN3
Text GLabel 9200 1950 0    50   Input ~ 0
ADC2_IN4
Text GLabel 9550 1850 2    50   Input ~ 0
POT_A
Text GLabel 9550 1950 2    50   Input ~ 0
POT_B
Text GLabel 9550 2050 2    50   Input ~ 0
POT_C
Text GLabel 9550 2150 2    50   Input ~ 0
POT_D
Text GLabel 9550 2250 2    50   Input ~ 0
POT_E
Text GLabel 9550 2350 2    50   Input ~ 0
POT_F
Text GLabel 9550 2450 2    50   Input ~ 0
POT_G
Text GLabel 9550 2550 2    50   Input ~ 0
POT_H
Text GLabel 9550 2650 2    50   Input ~ 0
POT_I
Text GLabel 9550 2750 2    50   Input ~ 0
POT_J
Wire Wire Line
	9200 1850 9550 1850
Wire Wire Line
	9200 1950 9550 1950
Wire Wire Line
	9550 2050 9200 2050
Wire Wire Line
	9200 2150 9550 2150
Wire Wire Line
	9550 2250 9200 2250
Wire Wire Line
	9200 2350 9550 2350
Wire Wire Line
	9550 2450 9200 2450
Wire Wire Line
	9200 2550 9550 2550
Wire Wire Line
	9550 2650 9200 2650
Wire Wire Line
	9200 2750 9550 2750
Text GLabel 6850 2650 2    50   Output ~ 0
USART2_TX
Text GLabel 6850 2750 2    50   Input ~ 0
USART2_RX
Wire Wire Line
	6850 2650 6800 2650
Wire Wire Line
	6800 2750 6850 2750
Text GLabel 5350 4750 0    50   Output ~ 0
I2S2_MCK
Text GLabel 6850 5650 2    50   Output ~ 0
I2S2_SD_TX
Wire Wire Line
	5400 4750 5350 4750
Wire Wire Line
	6850 5650 6800 5650
Text GLabel 6850 5550 2    50   Input ~ 0
I2S2_SD_RX
Wire Wire Line
	6850 5550 6800 5550
Text GLabel 6850 5450 2    50   Output ~ 0
I2S2_CLK
Wire Wire Line
	6850 5450 6800 5450
Text GLabel 6850 5350 2    50   Output ~ 0
I2S2_WS
Wire Wire Line
	6800 5350 6850 5350
Text GLabel 6850 5150 2    50   Output ~ 0
USART3_TX
Wire Wire Line
	6850 5150 6800 5150
Text GLabel 5350 5250 0    50   Input ~ 0
USART3_RX
Wire Wire Line
	5400 5250 5350 5250
Text GLabel 9600 3500 0    50   Output ~ 0
USART2_TX
Text GLabel 9600 3600 0    50   Input ~ 0
USART2_RX
Text Notes 9650 3550 0    50   ~ 0
TODO: \n
Text GLabel 9600 4000 0    50   Input ~ 0
USART3_RX
Text GLabel 9600 3900 0    50   Output ~ 0
USART3_TX
Text Notes 9650 3900 0    50   ~ 0
TODO: \n
$Comp
L power:GNDA #PWR?
U 1 1 60B81134
P 6300 5900
F 0 "#PWR?" H 6300 5650 50  0001 C CNN
F 1 "GNDA" H 6305 5727 50  0000 C CNN
F 2 "" H 6300 5900 50  0001 C CNN
F 3 "" H 6300 5900 50  0001 C CNN
	1    6300 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 5900 6300 5850
Text GLabel 9600 4950 0    50   Input ~ 0
SWO
Text GLabel 9600 4750 0    50   Input ~ 0
SWCLK
Text GLabel 9600 4850 0    50   Input ~ 0
SWDIO
Text Notes 9650 4900 0    50   ~ 0
TODO: \n
Text Notes 4750 3700 0    50   ~ 0
TODO: \n
Text GLabel 9600 5450 0    50   BiDi ~ 0
USB_D-
Text GLabel 9600 5550 0    50   BiDi ~ 0
USB_D+
Text Notes 9650 5500 0    50   ~ 0
TODO: \n
Text GLabel 9550 6200 0    50   Output ~ 0
I2S2_SD_TX
Text GLabel 9550 6100 0    50   Input ~ 0
I2S2_SD_RX
Text GLabel 9550 6000 0    50   Output ~ 0
I2S2_CLK
Text GLabel 9550 5900 0    50   Output ~ 0
I2S2_WS
Text GLabel 9550 5800 0    50   Output ~ 0
I2S2_MCK
Text Notes 9650 5950 0    50   ~ 0
TODO: \n
NoConn ~ 6800 2450
NoConn ~ 6800 2550
NoConn ~ 6800 2850
NoConn ~ 6800 2950
NoConn ~ 6800 3250
NoConn ~ 6800 3350
NoConn ~ 6800 3450
NoConn ~ 6800 3950
NoConn ~ 6800 4150
NoConn ~ 6800 4250
NoConn ~ 6800 4550
NoConn ~ 6800 4650
NoConn ~ 6800 4750
NoConn ~ 6800 4850
NoConn ~ 6800 4950
NoConn ~ 6800 5050
NoConn ~ 5400 5350
NoConn ~ 5400 5450
NoConn ~ 5400 5550
NoConn ~ 5400 5650
NoConn ~ 5400 4850
NoConn ~ 5400 4950
NoConn ~ 5400 5050
NoConn ~ 5400 5150
NoConn ~ 5400 3950
$EndSCHEMATC
