#

                                           _____ __            __  _
                                          / __(_) /  _______ _/ /_(_)__  ___
                                         / _// / _ \/ __/ _ `/ __/ / _ \/ _ \
                                        /_/ /_/_.__/_/  \_,_/\__/_/\___/_//_/

Eurorack synthesizer development platform targeting STM32 ARM Cortex-M4/M7
microcontrollers.

> [!NOTE] Project is in early development stage.

## Potential Applications

- [Eurorack](https://en.wikipedia.org/wiki/Eurorack) module
- Effect pedal

## Features and Technical Highlights

- Written in C++
- Currently focused on STM32F303 microcontroller
- RTOS driven ([FreeRTOS](https://www.freertos.org/) + [ccp_freertos](https://github.com/michaelbecker/freertos-addons) wrappers),
- I2S+DMA driven audio input/output
- Hardware FPU and
   [CMSIS-DSP](https://arm-software.github.io/CMSIS_5/DSP/html/index.html)
   utilization
- CLI shell and logger over UART for experimenting and troubleshooting

## What's Planned

- Hardware component drivers and abstractions: knob, slider, button, LED, rotary
  encoder, etc.
- STM32H7xx support
- SEGGER [SystemView](https://www.segger.com/products/development-tools/systemview/) support

## Documentation

- [Maintenance](docs/MAINTENANCE.md)

## Licenses

- GNU GPL-3.0 for software
- CERN OHL-2.0 for hardware
