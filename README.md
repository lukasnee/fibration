![Hopf Fibration](Misc/hopf-fibration.png)
# Fibration
Fibration is an open source synthesizer framework for embedded applications.
Utilising RTOS, MCU in-built DMA, I2S, ADC and DAC peripheralsfor efficient real-time DSP.
Currently being developed for STM32F303 MCU but designed to be portable. 

# Framework
> (note. MSA - Modular Synth Application)

                              MSA #1      MSA #2       MSA #3     MSA #...     MSA #N
                                |___________|___________|___________|____________|
                                                        |
                                                  ______|________
                                  ________________|Fib lib (C++)|__________________
                                 |                |_____________|                  |
                            FreeRTOS (C)                |                  Third-party libraries
                                                        |                       (C/C++)                                             
                                                STM32_HAL/LL lib (C)
                                                        |
                                                    HARDWARE 
                                                (STM32F303 for now) 

# License
- software GNU GPL-3.0
- hardware CERN OHL-2.0 
