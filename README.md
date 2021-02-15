![Hopf Fibration](Misc/hopf-fibration.png)
# Fibration

Fibration is an open source synthesizer framework for embedded applications. Currently being developed for STM32F303 MCU but aim is to be portable. Very much in progress (nothing really works ATM). 

Key features: 
- Written in C++
- based on FreeRTOS (wrapped in [ccp_freertos](https://github.com/michaelbecker/freertos-addons))
- unified peripheral API (for ease of use and future portability)
- use of DMA enabled I2S + FPU for efficient real-time digital signal processing.
- quick drivers for common synth components (signalIn, signalOut, knob, slider, button, led, rotary encoder...)
- lightweight command shell over UART ! (cool, could be used for configuring system in real-time, helpful for debugging)
- lightweight logger over UART ! (helpful for debugging)
- Tracealyzer support (TBD? would be helpful for debugging)

# Framework
> MSA - Modular Synth Application

                              MSA #1      MSA #2       MSA #3     MSA #...     MSA #N
                                |___________|___________|___________|____________|
                                                        |
                                                  ______|________
                                  ________________|Fib lib (C++)|__________________
                                 |                |_____________|                  |
                          ccp_freertos (C++)            |                  Third-party libraries
                                 |                      |                       (C/C++)                                             
                            FreeRTOS (C)        STM32_HAL/LL lib (C)
                                                        |
                                                    HARDWARE 
                                                (STM32F303 for now) 

# License
- software GNU GPL-3.0
- hardware CERN OHL-2.0 
