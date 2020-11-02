# fibration
Fibration is a family of eurorack synth modules. Homebrew :)
	- MCU software framework specifically designed for digital synth developement!
 	- takes care of hardware, unifies it and gives portabilyness-thing-a-ma-gig
 	- like Arduino, but oriented for sound sythesis and such (utilises DMA, I2S, 
		ADC, DAC...), gives a nice playground for creating and manipulating 
		sounds on a MCU. 

# framework
> (note. MSA - Modular Synth Application)

    MSA #1		MSA #2		MSA #3 		MSA #... 	MSA #N	
        |___________|___________|___________|____________|
                            |
                    Fibration lib (C++)
        ____________________|___________________________
        |					|							|
    FreeRTOS		STM32_HAL/LL lib (C)		Third-party libs (C/C++)
                            |
                        HARDWARE 
                    (STM32F3 for now) 

# license
- software under GNU GPL-3.0
- hardware under CERN OHL-2.0 
