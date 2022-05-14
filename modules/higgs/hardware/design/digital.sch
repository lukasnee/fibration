EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L power:GND #PWR?
U 1 1 622B268A
P 3350 5700
AR Path="/622B268A" Ref="#PWR?"  Part="1" 
AR Path="/622A682E/622B268A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3350 5450 50  0001 C CNN
F 1 "GND" H 3355 5527 50  0000 C CNN
F 2 "" H 3350 5700 50  0001 C CNN
F 3 "" H 3350 5700 50  0001 C CNN
	1    3350 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 5700 3350 5650
$Comp
L power:+3.3VA #PWR?
U 1 1 622B2691
P 3850 1950
AR Path="/622B2691" Ref="#PWR?"  Part="1" 
AR Path="/622A682E/622B2691" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3850 1800 50  0001 C CNN
F 1 "+3.3VA" H 3865 2123 50  0000 C CNN
F 2 "" H 3850 1950 50  0001 C CNN
F 3 "" H 3850 1950 50  0001 C CNN
	1    3850 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 1950 3850 2050
Text GLabel 4300 3550 2    50   Input ~ 0
SWDIO
Text GLabel 4300 3650 2    50   Input ~ 0
SWCLK
Text GLabel 4300 4250 2    50   Input ~ 0
SWO
Text Notes 4650 4300 0    50   ~ 0
TODO: for debugger printf, could be useful\n
Text GLabel 4300 3450 2    50   BiDi ~ 0
USB_D+
Text GLabel 4300 3350 2    50   BiDi ~ 0
USB_D-
Text GLabel 4300 2850 2    50   Input ~ 0
ADC2_IN3
Text GLabel 4300 2950 2    50   Input ~ 0
ADC2_IN4
Wire Wire Line
	2800 2450 2850 2450
Wire Wire Line
	2800 4350 2850 4350
Wire Wire Line
	2850 4450 2800 4450
Wire Wire Line
	4300 4250 4250 4250
Wire Wire Line
	4300 2850 4250 2850
Wire Wire Line
	4250 2950 4300 2950
Wire Wire Line
	4300 3350 4250 3350
Wire Wire Line
	4250 3450 4300 3450
Wire Wire Line
	4300 3550 4250 3550
Wire Wire Line
	4250 3650 4300 3650
Text GLabel 4300 5050 2    50   Input ~ 0
ADC2_IN14
Wire Wire Line
	4300 5050 4250 5050
Wire Wire Line
	2850 3950 2800 3950
Wire Wire Line
	2850 4050 2800 4050
Wire Wire Line
	2800 4150 2850 4150
Wire Wire Line
	2850 4250 2800 4250
Text GLabel 4300 4150 2    50   Input ~ 0
ADC2_IN12
Wire Wire Line
	4300 4150 4250 4150
Text GLabel 2800 4250 0    50   Input ~ 0
ADC2_IN9
Text GLabel 2800 4150 0    50   Input ~ 0
ADC2_IN8
Text GLabel 2800 4050 0    50   Input ~ 0
ADC2_IN7
Text GLabel 2800 3950 0    50   Input ~ 0
ADC2_IN6
Text GLabel 2800 4450 0    50   Input ~ 0
ADC2_IN11
Text GLabel 2800 4350 0    50   Input ~ 0
ADC2_IN5
Text GLabel 5400 3450 0    50   Input ~ 0
ADC2_IN9
Text GLabel 5400 3350 0    50   Input ~ 0
ADC2_IN8
Text GLabel 5400 3250 0    50   Input ~ 0
ADC2_IN7
Text GLabel 5400 3150 0    50   Input ~ 0
ADC2_IN6
Text GLabel 5400 3650 0    50   Input ~ 0
ADC2_IN11
Text GLabel 5400 3550 0    50   Input ~ 0
ADC2_IN5
Text GLabel 5400 3850 0    50   Input ~ 0
ADC2_IN14
Text GLabel 5400 3750 0    50   Input ~ 0
ADC2_IN12
Text GLabel 5400 2950 0    50   Input ~ 0
ADC2_IN3
Text GLabel 5400 3050 0    50   Input ~ 0
ADC2_IN4
Text GLabel 5750 2950 2    50   Input ~ 0
POT_A
Text GLabel 5750 3050 2    50   Input ~ 0
POT_B
Text GLabel 5750 3150 2    50   Input ~ 0
POT_C
Text GLabel 5750 3250 2    50   Input ~ 0
POT_D
Text GLabel 5750 3350 2    50   Input ~ 0
POT_E
Text GLabel 5750 3450 2    50   Input ~ 0
POT_F
Text GLabel 5750 3550 2    50   Input ~ 0
POT_G
Text GLabel 5750 3650 2    50   Input ~ 0
POT_H
Text GLabel 5750 3750 2    50   Input ~ 0
POT_I
Text GLabel 5750 3850 2    50   Input ~ 0
POT_J
Wire Wire Line
	5400 2950 5750 2950
Wire Wire Line
	5400 3050 5750 3050
Wire Wire Line
	5750 3150 5400 3150
Wire Wire Line
	5400 3250 5750 3250
Wire Wire Line
	5750 3350 5400 3350
Wire Wire Line
	5400 3450 5750 3450
Wire Wire Line
	5750 3550 5400 3550
Wire Wire Line
	5400 3650 5750 3650
Wire Wire Line
	5750 3750 5400 3750
Wire Wire Line
	5400 3850 5750 3850
Text GLabel 4300 2450 2    50   Output ~ 0
USART2_TX
Text GLabel 4300 2550 2    50   Input ~ 0
USART2_RX
Wire Wire Line
	4300 2450 4250 2450
Wire Wire Line
	4250 2550 4300 2550
Text GLabel 2800 4550 0    50   Output ~ 0
I2S2_MCK
Text GLabel 4300 5450 2    50   Output ~ 0
I2S2_SD_TX
Wire Wire Line
	2850 4550 2800 4550
Wire Wire Line
	4300 5450 4250 5450
Text GLabel 4300 5350 2    50   Input ~ 0
I2S2_SD_RX
Wire Wire Line
	4300 5350 4250 5350
Text GLabel 4300 5250 2    50   Output ~ 0
I2S2_CLK
Wire Wire Line
	4300 5250 4250 5250
Text GLabel 4300 5150 2    50   Output ~ 0
I2S2_WS
Wire Wire Line
	4250 5150 4300 5150
Text GLabel 4300 4950 2    50   Output ~ 0
USART3_TX
Wire Wire Line
	4300 4950 4250 4950
Wire Wire Line
	2850 5050 2800 5050
Text GLabel 7600 2450 0    50   Output ~ 0
USART2_TX
Text GLabel 7600 2550 0    50   Input ~ 0
USART2_RX
Text Notes 7650 2500 0    50   ~ 0
TODO: \n
Text GLabel 7600 2950 0    50   Input ~ 0
USART3_RX
Text GLabel 7600 2850 0    50   Output ~ 0
USART3_TX
Text Notes 7650 2850 0    50   ~ 0
TODO: \n
Text GLabel 7600 3900 0    50   Input ~ 0
SWO
Text GLabel 7600 3700 0    50   Input ~ 0
SWCLK
Text GLabel 7600 3800 0    50   Input ~ 0
SWDIO
Text Notes 7650 3850 0    50   ~ 0
TODO: \n
Text GLabel 7600 4400 0    50   BiDi ~ 0
USB_D-
Text GLabel 7600 4500 0    50   BiDi ~ 0
USB_D+
Text Notes 7650 4450 0    50   ~ 0
TODO: \n
Text GLabel 7550 5150 0    50   Output ~ 0
I2S2_SD_TX
Text GLabel 7550 5050 0    50   Input ~ 0
I2S2_SD_RX
Text GLabel 7550 4950 0    50   Output ~ 0
I2S2_CLK
Text GLabel 7550 4850 0    50   Output ~ 0
I2S2_WS
Text GLabel 7550 4750 0    50   Output ~ 0
I2S2_MCK
Text Notes 7650 4900 0    50   ~ 0
TODO: \n
NoConn ~ 4250 2250
NoConn ~ 4250 2350
NoConn ~ 4250 2650
NoConn ~ 4250 2750
NoConn ~ 4250 3050
NoConn ~ 4250 3150
NoConn ~ 4250 3250
NoConn ~ 4250 3750
NoConn ~ 4250 3950
NoConn ~ 4250 4050
NoConn ~ 4250 4350
NoConn ~ 4250 4450
NoConn ~ 4250 4550
NoConn ~ 4250 4650
NoConn ~ 4250 4750
NoConn ~ 4250 4850
NoConn ~ 2850 5150
NoConn ~ 2850 5250
NoConn ~ 2850 5350
NoConn ~ 2850 5450
NoConn ~ 2850 4650
NoConn ~ 2850 4750
NoConn ~ 2850 4850
NoConn ~ 2850 4950
NoConn ~ 2850 3750
Connection ~ 3550 5650
Connection ~ 3450 5650
Connection ~ 3350 5650
Wire Wire Line
	3550 5650 3450 5650
Wire Wire Line
	3350 5650 3450 5650
Connection ~ 3750 5700
$Comp
L power:GNDA #PWR?
U 1 1 622B2719
P 3750 5700
AR Path="/622B2719" Ref="#PWR?"  Part="1" 
AR Path="/622A682E/622B2719" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3750 5450 50  0001 C CNN
F 1 "GNDA" H 3755 5527 50  0000 C CNN
F 2 "" H 3750 5700 50  0001 C CNN
F 3 "" H 3750 5700 50  0001 C CNN
	1    3750 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 5700 3750 5650
$Comp
L power:GNDA #PWR?
U 1 1 622B2720
P 3750 5700
AR Path="/622B2720" Ref="#PWR?"  Part="1" 
AR Path="/622A682E/622B2720" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3750 5450 50  0001 C CNN
F 1 "GNDA" H 3755 5527 50  0000 C CNN
F 2 "" H 3750 5700 50  0001 C CNN
F 3 "" H 3750 5700 50  0001 C CNN
	1    3750 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 5650 3650 5650
$Comp
L MCU_ST_STM32F3:STM32F303RETx U?
U 1 1 622B2727
P 3550 3850
AR Path="/622B2727" Ref="U?"  Part="1" 
AR Path="/622A682E/622B2727" Ref="U?"  Part="1" 
F 0 "U?" H 4100 5600 50  0000 C CNN
F 1 "STM32F303RETx" H 4100 2100 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 2950 2150 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00118585.pdf" H 3550 3850 50  0001 C CNN
	1    3550 3850
	1    0    0    -1  
$EndComp
Text GLabel 2800 2250 0    50   Input ~ 0
NRST
Text GLabel 2800 2450 0    50   Input ~ 0
BOOT0
Wire Wire Line
	2850 2250 2800 2250
Wire Wire Line
	3750 2000 3750 2050
Wire Wire Line
	3650 2000 3650 2050
Connection ~ 3650 2000
Wire Wire Line
	3650 2000 3750 2000
Wire Wire Line
	3550 2000 3550 2050
Connection ~ 3550 2000
Wire Wire Line
	3550 2000 3650 2000
Wire Wire Line
	3450 2000 3450 2050
Wire Wire Line
	3450 2000 3550 2000
$Comp
L power:+3.3V #PWR?
U 1 1 622B2739
P 3350 1950
AR Path="/622B2739" Ref="#PWR?"  Part="1" 
AR Path="/622A682E/622B2739" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3350 1800 50  0001 C CNN
F 1 "+3.3V" H 3365 2123 50  0000 C CNN
F 2 "" H 3350 1950 50  0001 C CNN
F 3 "" H 3350 1950 50  0001 C CNN
	1    3350 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1950 3350 2000
Wire Wire Line
	3450 2000 3350 2000
Connection ~ 3450 2000
Connection ~ 3350 2000
Wire Wire Line
	3350 2000 3350 2050
$EndSCHEMATC
