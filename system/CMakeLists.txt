add_library(fibsys_obj_lib OBJECT)
# TODO: improve lib structure and composition

# TODO: separate hw specific and generic code

target_link_libraries(
  fibsys_obj_lib PUBLIC STM32F3xx_HAL_Driver ARM_MATH_CM4_GCC FreeRTOS-Cpp
                        ln::prof)

target_include_directories(
  fibsys_obj_lib
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
            include
            ${PROJECT_SOURCE_DIR}/modules/higgs # for version.def # TODO: fix
                                                # cohesion
            ${PROJECT_SOURCE_DIR} # for gitHash.def # TODO: fix cohesion
)

target_include_directories(fibsys_obj_lib PUBLIC msp)
target_sources(
  fibsys_obj_lib
  PUBLIC msp/stm32f3xx_hal_msp.c
         msp/stm32f3xx_hal_timebase_tim.c
         msp/stm32f3xx_it.c
         msp/syscalls.cpp
         msp/sysmem.c
         msp/system_stm32f3xx.c
         msp/startup_stm32f303retx.s)

target_include_directories(fibsys_obj_lib PUBLIC . core core/Libraries)
target_sources(
  fibsys_obj_lib
  PUBLIC core/memory.cpp # PUBLIC for FreeRTOS for vApplicationGetIdleTaskMemory
                         # and vApplicationGetTimerTaskMemory
  PRIVATE core/main.cpp core/system.cpp)

target_include_directories(fibsys_obj_lib PUBLIC utilities)

# TODO: Make all peripherals work over interfaces
target_include_directories(
  fibsys_obj_lib PUBLIC peripherals/interfaces peripherals/stm32f303r
                        peripherals)
target_sources(
  fibsys_obj_lib
  PRIVATE peripherals/interfaces/i2sIF.cpp
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
  fibsys_obj_lib
  PUBLIC peripherals/stm32f303r/uartIsr.cpp peripherals/stm32f303r/i2sIsr.cpp
         peripherals/stm32f303r/adcIsr.cpp)

target_include_directories(fibsys_obj_lib PUBLIC components components/shell)

target_link_libraries(
  fibsys_obj_lib
  PUBLIC ln::port::STM32F3_FreeRTOS
         ln::port::FreeRTOS::shell_cmds
         ln::core::shell_cmds
         ln::algo
         ln::shell
         ln::shell::cmds::general
         ln::logger::cmds)

if(LN_LUA)
  target_link_libraries(fibsys_obj_lib INTERFACE ln::lua)
  target_compile_definitions(fibsys_obj_lib INTERFACE LN_LUA)
endif()

# TODO: try to get as many things above below this line (fibsys_obj_lib ->
# fibsys)

add_library(fibsys)
target_link_libraries(fibsys PUBLIC fibsys_obj_lib)
target_include_directories(fibsys PUBLIC streams/interfaces streams)
target_sources(
  fibsys PRIVATE streams/interfaces/asciiStreamIF.cpp streams/i2sStreamer.cpp
                 streams/ioStream.cpp streams/txStream.cpp dsp/oscillator.cpp)
