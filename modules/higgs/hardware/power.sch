EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
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
L Regulator_Linear:AMS1117-3.3 U?
U 1 1 622A4B16
P 7650 4125
AR Path="/622A4B16" Ref="U?"  Part="1" 
AR Path="/62285344/622A4B16" Ref="U?"  Part="1" 
F 0 "U?" H 7650 4367 50  0000 C CNN
F 1 "AMS1117-3.3" H 7650 4276 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 7650 4325 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 7750 3875 50  0001 C CNN
	1    7650 4125
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 622A4B1C
P 2750 4475
AR Path="/622A4B1C" Ref="J?"  Part="1" 
AR Path="/62285344/622A4B1C" Ref="J?"  Part="1" 
F 0 "J?" H 2800 4892 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 2800 4801 50  0000 C CNN
F 2 "" H 2750 4475 50  0001 C CNN
F 3 "~" H 2750 4475 50  0001 C CNN
	1    2750 4475
	1    0    0    1   
$EndComp
Wire Wire Line
	2500 4575 2550 4575
Wire Wire Line
	2500 4575 2500 4475
Wire Wire Line
	2500 4375 2550 4375
Connection ~ 2500 4475
Wire Wire Line
	2500 4475 2500 4375
Wire Wire Line
	2500 4475 2400 4475
Wire Wire Line
	3050 4275 2550 4275
Wire Wire Line
	2550 4675 3050 4675
$Comp
L Device:Fuse_Small F?
U 1 1 622A4B2A
P 6250 4125
AR Path="/622A4B2A" Ref="F?"  Part="1" 
AR Path="/62285344/622A4B2A" Ref="F?"  Part="1" 
F 0 "F?" H 6250 4310 50  0000 C CNN
F 1 "Fuse_Small" H 6250 4219 50  0000 C CNN
F 2 "" H 6250 4125 50  0001 C CNN
F 3 "~" H 6250 4125 50  0001 C CNN
	1    6250 4125
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 4125 7000 4125
Wire Wire Line
	6800 4125 6350 4125
$Comp
L power:GND #PWR?
U 1 1 622A4B32
P 7650 4475
AR Path="/622A4B32" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622A4B32" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7650 4225 50  0001 C CNN
F 1 "GND" H 7655 4302 50  0000 C CNN
F 2 "" H 7650 4475 50  0001 C CNN
F 3 "" H 7650 4475 50  0001 C CNN
	1    7650 4475
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 4475 7650 4425
$Comp
L power:GND #PWR?
U 1 1 622A4B39
P 2400 4575
AR Path="/622A4B39" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622A4B39" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2400 4325 50  0001 C CNN
F 1 "GND" H 2405 4402 50  0000 C CNN
F 2 "" H 2400 4575 50  0001 C CNN
F 3 "" H 2400 4575 50  0001 C CNN
	1    2400 4575
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2400 4475 2400 4575
$Comp
L Device:D_Schottky_Small D?
U 1 1 622A4B40
P 3500 4075
AR Path="/622A4B40" Ref="D?"  Part="1" 
AR Path="/62285344/622A4B40" Ref="D?"  Part="1" 
F 0 "D?" H 3500 4175 50  0000 C CNN
F 1 "B5819W" H 3500 3975 50  0000 C CNN
F 2 "" V 3500 4075 50  0001 C CNN
F 3 "~" V 3500 4075 50  0001 C CNN
	1    3500 4075
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 4075 3200 4075
$Comp
L Device:D_Schottky_Small D?
U 1 1 622A4B47
P 3500 4925
AR Path="/622A4B47" Ref="D?"  Part="1" 
AR Path="/62285344/622A4B47" Ref="D?"  Part="1" 
F 0 "D?" H 3500 5025 50  0000 C CNN
F 1 "B5819W" H 3500 4825 50  0000 C CNN
F 2 "" V 3500 4925 50  0001 C CNN
F 3 "~" V 3500 4925 50  0001 C CNN
	1    3500 4925
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 4925 3200 4925
Wire Wire Line
	4075 4075 3600 4075
Wire Wire Line
	3600 4925 4075 4925
$Comp
L Device:Ferrite_Bead_Small L?
U 1 1 622A4B5A
P 4175 4925
AR Path="/622A4B5A" Ref="L?"  Part="1" 
AR Path="/62285344/622A4B5A" Ref="L?"  Part="1" 
F 0 "L?" V 4225 5025 50  0000 C CNN
F 1 "100Ω@100MHz" V 4025 4925 50  0000 C CNN
F 2 "Sunlord" H 4175 4925 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/lcsc/2111221630_Sunlord-GZ2012D101TF_C1015.pdf" H 4175 4925 50  0001 C CNN
F 4 "GZ2012D101TF" V 4175 4925 50  0001 C CNN "Part"
F 5 "Sunlord" V 4175 4925 50  0001 C CNN "Mf"
	1    4175 4925
	0    -1   -1   0   
$EndComp
Text Notes 2250 5175 0    50   ~ 0
TODO: doublecheck\nif pins in right orientation\n
$Comp
L Device:C_Small C?
U 1 1 622A4B63
P 4700 4275
AR Path="/622A4B63" Ref="C?"  Part="1" 
AR Path="/62285344/622A4B63" Ref="C?"  Part="1" 
F 0 "C?" H 4792 4321 50  0000 L CNN
F 1 "25V 22uF ±10%" H 4792 4230 50  0000 L CNN
F 2 "" H 4700 4275 50  0001 C CNN
F 3 "" H 4700 4275 50  0001 C CNN
F 4 "CL31A226KAHNNNE" H 4700 4275 50  0001 C CNN "Part"
F 5 "Samsung Electro-Mechanics" H 4700 4275 50  0001 C CNN "Mf"
	1    4700 4275
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 622A4B6B
P 4700 4700
AR Path="/622A4B6B" Ref="C?"  Part="1" 
AR Path="/62285344/622A4B6B" Ref="C?"  Part="1" 
F 0 "C?" H 4792 4746 50  0000 L CNN
F 1 "25V 22uF ±10%" H 4792 4655 50  0000 L CNN
F 2 "" H 4700 4700 50  0001 C CNN
F 3 "" H 4700 4700 50  0001 C CNN
F 4 "CL31A226KAHNNNE" H 4700 4700 50  0001 C CNN "Part"
F 5 "Samsung Electro-Mechanics" H 4700 4700 50  0001 C CNN "Mf"
	1    4700 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 4375 4700 4475
Wire Wire Line
	4700 4475 4450 4475
Connection ~ 4700 4475
Wire Wire Line
	4700 4475 4700 4600
Connection ~ 2550 4375
Connection ~ 2550 4475
Wire Wire Line
	2550 4475 2500 4475
Connection ~ 2550 4575
Wire Wire Line
	2550 4375 3050 4375
Wire Wire Line
	2550 4475 3050 4475
Wire Wire Line
	2550 4575 3050 4575
Wire Wire Line
	3200 4925 3200 4675
Wire Wire Line
	3200 4675 3050 4675
Connection ~ 3050 4675
Wire Wire Line
	3200 4075 3200 4275
Wire Wire Line
	3200 4275 3050 4275
Connection ~ 3050 4275
Wire Wire Line
	4450 4475 4450 4550
$Comp
L power:GND #PWR?
U 1 1 622A4B83
P 4450 4550
AR Path="/622A4B83" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622A4B83" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4450 4300 50  0001 C CNN
F 1 "GND" H 4450 4400 50  0000 C CNN
F 2 "" H 4450 4550 50  0001 C CNN
F 3 "" H 4450 4550 50  0001 C CNN
	1    4450 4550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4700 4175 4700 4075
Wire Wire Line
	4700 4075 4275 4075
Wire Wire Line
	4700 4925 4275 4925
Wire Wire Line
	4700 4800 4700 4925
$Comp
L Device:Ferrite_Bead_Small L?
U 1 1 622A4B8F
P 6900 4125
AR Path="/622A4B8F" Ref="L?"  Part="1" 
AR Path="/62285344/622A4B8F" Ref="L?"  Part="1" 
F 0 "L?" V 6850 4225 50  0000 C CNN
F 1 "100Ω@100MHz" V 6750 4225 50  0000 C CNN
F 2 "Sunlord" H 6900 4125 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/lcsc/2111221630_Sunlord-GZ2012D101TF_C1015.pdf" H 6900 4125 50  0001 C CNN
F 4 "GZ2012D101TF" V 6900 4125 50  0001 C CNN "Part"
F 5 "Sunlord" V 6900 4125 50  0001 C CNN "Mf"
	1    6900 4125
	0    -1   -1   0   
$EndComp
$Comp
L higgs:V7803-500 U?
U 1 1 622A4B95
P 7100 2400
AR Path="/622A4B95" Ref="U?"  Part="1" 
AR Path="/62285344/622A4B95" Ref="U?"  Part="1" 
F 0 "U?" H 7100 2675 50  0000 C CNN
F 1 "V7803-500" H 7100 2584 50  0000 C CNN
F 2 "higgs:V78-500" H 7100 2650 50  0001 C CNN
F 3 "https://eu.mouser.com/datasheet/2/670/v78_500-1774424.pdf" H 7100 2400 50  0001 C CNN
	1    7100 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 622A4B9D
P 7750 2600
AR Path="/622A4B9D" Ref="C?"  Part="1" 
AR Path="/62285344/622A4B9D" Ref="C?"  Part="1" 
F 0 "C?" H 7842 2646 50  0000 L CNN
F 1 "25V 22uF" H 7842 2555 50  0000 L CNN
F 2 "" H 7750 2600 50  0001 C CNN
F 3 "" H 7750 2600 50  0001 C CNN
F 4 "CL31A226KAHNNNE" H 7750 2600 50  0001 C CNN "Part"
F 5 "Samsung Electro-Mechanics" H 7750 2600 50  0001 C CNN "Mf"
	1    7750 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2350 7750 2350
Wire Wire Line
	7750 2350 7750 2500
Wire Wire Line
	7750 2700 7750 2800
Wire Wire Line
	7750 2800 7100 2800
Wire Wire Line
	7100 2800 7100 2700
$Comp
L Device:Ferrite_Bead_Small L?
U 1 1 622A4B50
P 4175 4075
AR Path="/622A4B50" Ref="L?"  Part="1" 
AR Path="/62285344/622A4B50" Ref="L?"  Part="1" 
F 0 "L?" V 4125 4175 50  0000 C CNN
F 1 "100Ω@100MHz" V 4025 4175 50  0000 C CNN
F 2 "Sunlord" H 4175 4075 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/lcsc/2111221630_Sunlord-GZ2012D101TF_C1015.pdf" H 4175 4075 50  0001 C CNN
F 4 "GZ2012D101TF" V 4175 4075 50  0001 C CNN "Part"
F 5 "Sunlord" V 4175 4075 50  0001 C CNN "Mf"
	1    4175 4075
	0    -1   -1   0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 622C5797
P 6350 2600
AR Path="/622C5797" Ref="C?"  Part="1" 
AR Path="/62285344/622C5797" Ref="C?"  Part="1" 
F 0 "C?" H 6442 2646 50  0000 L CNN
F 1 "25V 22uF ±10%" H 6442 2555 50  0000 L CNN
F 2 "" H 6350 2600 50  0001 C CNN
F 3 "" H 6350 2600 50  0001 C CNN
F 4 "CL31A226KAHNNNE" H 6350 2600 50  0001 C CNN "Part"
F 5 "Samsung Electro-Mechanics" H 6350 2600 50  0001 C CNN "Mf"
	1    6350 2600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
