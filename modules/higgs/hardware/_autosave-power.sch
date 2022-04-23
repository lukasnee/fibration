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
P 7600 5125
AR Path="/622A4B16" Ref="U?"  Part="1" 
AR Path="/62285344/622A4B16" Ref="U2"  Part="1" 
F 0 "U2" H 7600 5367 50  0000 C CNN
F 1 "AMS1117-3.3" H 7600 5276 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 7600 5325 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 7700 4875 50  0001 C CNN
	1    7600 5125
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 622A4B1C
P 2750 4475
AR Path="/622A4B1C" Ref="J?"  Part="1" 
AR Path="/62285344/622A4B1C" Ref="J1"  Part="1" 
F 0 "J1" H 2800 4892 50  0000 C CNN
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
P 6200 5125
AR Path="/622A4B2A" Ref="F?"  Part="1" 
AR Path="/62285344/622A4B2A" Ref="F1"  Part="1" 
F 0 "F1" H 6200 5310 50  0000 C CNN
F 1 "Fuse_Small" H 6200 5219 50  0000 C CNN
F 2 "" H 6200 5125 50  0001 C CNN
F 3 "~" H 6200 5125 50  0001 C CNN
	1    6200 5125
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 5125 6950 5125
Wire Wire Line
	6750 5125 6300 5125
$Comp
L power:GND #PWR?
U 1 1 622A4B32
P 7600 5475
AR Path="/622A4B32" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622A4B32" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 7600 5225 50  0001 C CNN
F 1 "GND" H 7605 5302 50  0000 C CNN
F 2 "" H 7600 5475 50  0001 C CNN
F 3 "" H 7600 5475 50  0001 C CNN
	1    7600 5475
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 5475 7600 5425
$Comp
L power:GND #PWR?
U 1 1 622A4B39
P 2400 4575
AR Path="/622A4B39" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622A4B39" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 2400 4325 50  0001 C CNN
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
AR Path="/62285344/622A4B40" Ref="D1"  Part="1" 
F 0 "D1" H 3500 4175 50  0000 C CNN
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
AR Path="/62285344/622A4B47" Ref="D2"  Part="1" 
F 0 "D2" H 3500 5025 50  0000 C CNN
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
AR Path="/62285344/622A4B5A" Ref="L2"  Part="1" 
F 0 "L2" V 4225 5025 50  0000 C CNN
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
AR Path="/62285344/622A4B63" Ref="C1"  Part="1" 
F 0 "C1" H 4792 4321 50  0000 L CNN
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
AR Path="/62285344/622A4B6B" Ref="C2"  Part="1" 
F 0 "C2" H 4792 4746 50  0000 L CNN
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
AR Path="/62285344/622A4B83" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 4450 4300 50  0001 C CNN
F 1 "GND" H 4450 4400 50  0000 C CNN
F 2 "" H 4450 4550 50  0001 C CNN
F 3 "" H 4450 4550 50  0001 C CNN
	1    4450 4550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4700 4075 4275 4075
Wire Wire Line
	4700 4925 4275 4925
Wire Wire Line
	4700 4800 4700 4925
$Comp
L Device:Ferrite_Bead_Small L?
U 1 1 622A4B8F
P 6850 5125
AR Path="/622A4B8F" Ref="L?"  Part="1" 
AR Path="/62285344/622A4B8F" Ref="L3"  Part="1" 
F 0 "L3" V 6800 5225 50  0000 C CNN
F 1 "100Ω@100MHz" V 6700 5225 50  0000 C CNN
F 2 "Sunlord" H 6850 5125 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/lcsc/2111221630_Sunlord-GZ2012D101TF_C1015.pdf" H 6850 5125 50  0001 C CNN
F 4 "GZ2012D101TF" V 6850 5125 50  0001 C CNN "Part"
F 5 "Sunlord" V 6850 5125 50  0001 C CNN "Mf"
	1    6850 5125
	0    -1   -1   0   
$EndComp
$Comp
L higgs:V7803-500 U?
U 1 1 622A4B95
P 6900 3700
AR Path="/622A4B95" Ref="U?"  Part="1" 
AR Path="/62285344/622A4B95" Ref="U1"  Part="1" 
F 0 "U1" H 6900 3975 50  0000 C CNN
F 1 "V7803-500" H 6900 3884 50  0000 C CNN
F 2 "higgs:V78-500" H 6900 3950 50  0001 C CNN
F 3 "https://eu.mouser.com/datasheet/2/670/v78_500-1774424.pdf" H 6900 3700 50  0001 C CNN
	1    6900 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 622A4B9D
P 7550 3900
AR Path="/622A4B9D" Ref="C?"  Part="1" 
AR Path="/62285344/622A4B9D" Ref="C4"  Part="1" 
F 0 "C4" H 7642 3946 50  0000 L CNN
F 1 "25V 22uF" H 7642 3855 50  0000 L CNN
F 2 "" H 7550 3900 50  0001 C CNN
F 3 "" H 7550 3900 50  0001 C CNN
F 4 "CL31A226KAHNNNE" H 7550 3900 50  0001 C CNN "Part"
F 5 "Samsung Electro-Mechanics" H 7550 3900 50  0001 C CNN "Mf"
	1    7550 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3650 7550 3650
Wire Wire Line
	7550 3650 7550 3800
Wire Wire Line
	7550 4000 7550 4100
Wire Wire Line
	6900 4100 6900 4000
$Comp
L Device:Ferrite_Bead_Small L?
U 1 1 622A4B50
P 4175 4075
AR Path="/622A4B50" Ref="L?"  Part="1" 
AR Path="/62285344/622A4B50" Ref="L1"  Part="1" 
F 0 "L1" V 4125 4175 50  0000 C CNN
F 1 "100Ω@100MHz" V 4025 4175 50  0000 C CNN
F 2 "Sunlord" H 4175 4075 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/lcsc/2111221630_Sunlord-GZ2012D101TF_C1015.pdf" H 4175 4075 50  0001 C CNN
F 4 "GZ2012D101TF" V 4175 4075 50  0001 C CNN "Part"
F 5 "Sunlord" V 4175 4075 50  0001 C CNN "Mf"
	1    4175 4075
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6150 4100 6150 4000
Wire Wire Line
	6550 3650 6150 3650
$Comp
L Device:C_Small C?
U 1 1 622C5797
P 6150 3900
AR Path="/622C5797" Ref="C?"  Part="1" 
AR Path="/62285344/622C5797" Ref="C3"  Part="1" 
F 0 "C3" H 6242 3946 50  0000 L CNN
F 1 "25V 10uF" H 6242 3855 50  0000 L CNN
F 2 "capacitor:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 6150 3900 50  0001 C CNN
F 3 "https://datasheet.lcsc.com/lcsc/1811121310_Samsung-Electro-Mechanics-CL21A106KAYNNNE_C15850.pdf" H 6150 3900 50  0001 C CNN
F 4 "CL21A106KAYNNNE" H 6150 3900 50  0001 C CNN "Part"
F 5 "Samsung Electro-Mechanics" H 6150 3900 50  0001 C CNN "Mf."
F 6 "X5R 25V ±10% 10uF 0805 Multilayer Ceramic Capacitors MLCC - SMD/SMT ROHS" H 6150 3900 50  0001 C CNN "Descr"
	1    6150 3900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 622DB339
P 6900 4100
AR Path="/622DB339" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622DB339" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6900 3850 50  0001 C CNN
F 1 "GND" H 6905 3927 50  0000 C CNN
F 2 "" H 6900 4100 50  0001 C CNN
F 3 "" H 6900 4100 50  0001 C CNN
	1    6900 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 622DB60E
P 6150 4100
AR Path="/622DB60E" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622DB60E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6150 3850 50  0001 C CNN
F 1 "GND" H 6155 3927 50  0000 C CNN
F 2 "" H 6150 4100 50  0001 C CNN
F 3 "" H 6150 4100 50  0001 C CNN
	1    6150 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 622DB92A
P 7550 4100
AR Path="/622DB92A" Ref="#PWR?"  Part="1" 
AR Path="/62285344/622DB92A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7550 3850 50  0001 C CNN
F 1 "GND" H 7555 3927 50  0000 C CNN
F 2 "" H 7550 4100 50  0001 C CNN
F 3 "" H 7550 4100 50  0001 C CNN
	1    7550 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 3650 6150 3800
Wire Wire Line
	6150 3650 4700 3650
Wire Wire Line
	4700 3650 4700 4075
Connection ~ 6150 3650
Connection ~ 4700 4075
Wire Wire Line
	4700 4075 4700 4175
$EndSCHEMATC
