<!-- ![Hopf Fibration](Misc/hopf-fibration.png) -->


                                       _____ __            __  _
                                      / __(_) /  _______ _/ /_(_)__  ___
                                     / _// / _ \/ __/ _ `/ __/ / _ \/ _ \
                                    /_/ /_/_.__/_/  \_,_/\__/_/\___/_//_/
                                            FOSS Modular Synth Framework

Open source modular synthesizer embedded software framework [WIP]

## Feature Idea
- C++
- based on STM32 (F303xx atm) 
- based on FreeRTOS and interfaced over [ccp_freertos](https://github.com/michaelbecker/freertos-addons) wrapper
- DMA/I2S driven audio streamer for easy external ADC/DAC interfacing
- hardware FPU enabled CMSIS DSP support
- thread-safe and DSP oriented peripheral hardware resource manager (In progress)
- Common synth component high-level implementations (pot, slider, button, led, rotary encoder... TBD)
- lightweight command shell and logger over UART (nice for experimenting and debugging)
- *Tracealyzer* support (TBD)

## Framework Idea

                          MSA #1      MSA #2       MSA #3     MSA #...     MSA #N
                            |___________|___________|___________|____________|
                                                    |
                                           +--------+--------+
                             +-------------| Fibration (C++) +------------------+
                             |             +--------+--------+                  |
                      ccp_freertos (C++)            |                     Third-party
                             |                      |                   libraries (C/C++)
                        FreeRTOS (C)         STM32_HAL lib (C)
                                                    |
                                                STM32F303XX

> MSA - Modular Synth Application

# Status

Unreleased, still very much work in progress.

# Development Enviroment
Linux. Developed on Raspberry Pi4 (Raspbian OS) remotely using VSCode "Remote Development" extension from Windows PC. Pretty weird but flexible setup IMO.

# License
- software GNU GPL-3.0
- hardware CERN OHL-2.0

