add_library(fibsys INTERFACE)

add_subdirectory(core/libraries)

target_link_libraries(fibsys INTERFACE STM32F3xx_HAL_Driver ARM_MATH_CM4_GCC
                                       FreeRTOS-Cpp)
target_include_directories(
  fibsys
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
            include
            ${PROJECT_SOURCE_DIR}/modules/higgs # for version.def # TODO: fix
                                                # cohesion
            ${PROJECT_SOURCE_DIR} # for gitHash.def # TODO: fix cohesion
)

# TODO: imrpove lib structure and composition TODO: separate hw specific and
# generic code

target_include_directories(fibsys INTERFACE msp)
target_sources(
  fibsys
  INTERFACE msp/stm32f3xx_hal_msp.c
            msp/stm32f3xx_hal_timebase_tim.c
            msp/stm32f3xx_it.c
            msp/syscalls.cpp
            msp/sysmem.c
            msp/system_stm32f3xx.c
            msp/startup_stm32f303retx.s)

target_include_directories(fibsys INTERFACE core core/Libraries)
target_sources(
  fibsys
  INTERFACE core/main.cpp core/system.cpp
  INTERFACE core/memory.cpp)

target_sources(fibsys INTERFACE dsp/oscillator.cpp)

target_include_directories(fibsys INTERFACE utilities)

# TODO: Make all peripherals work over interfaces
target_include_directories(fibsys INTERFACE peripherals/interfaces
                                            peripherals/stm32f303r peripherals)
target_sources(
  fibsys
  INTERFACE peripherals/interfaces/ioDataIF.hpp
            peripherals/interfaces/i2sIF.cpp
            peripherals/interfaces/adcIF.hpp
            peripherals/stm32f303r/tim6.cpp
            peripherals/stm32f303r/uart2.cpp
            peripherals/stm32f303r/uart3.cpp
            peripherals/stm32f303r/i2s2.cpp
            peripherals/stm32f303r/adc2.cpp
            peripherals/resources.cpp)

# NOTE: these files contain STM32 HAL weak function overrides so they must be
# compiled and linked directly into the final executable. They cannot be
# compiled into a static library and then linked.
target_sources(
  fibsys
  INTERFACE peripherals/stm32f303r/uartIsr.cpp
            peripherals/stm32f303r/i2sIsr.cpp peripherals/stm32f303r/adcIsr.cpp)

target_include_directories(fibsys INTERFACE streams/interfaces streams)
target_sources(
  fibsys INTERFACE streams/interfaces/asciiStreamIF.cpp streams/i2sStreamer.cpp
                   streams/ioStream.cpp streams/txStream.cpp)

target_include_directories(fibsys INTERFACE components components/shell)

target_link_libraries(
  fibsys
  INTERFACE ln::port::STM32F3_FreeRTOS
            ln::port::FreeRTOS::shell_cmds
            ln::core::shell_cmds
            ln::algo
            ln::shell
            ln::lua
            ln::shell::cmds::general
            ln::logger::cmds)
