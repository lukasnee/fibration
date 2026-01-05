add_library(fibsys INTERFACE)

function(fibsys_firmware)
  execute_process(
    COMMAND git describe --always --dirty --match "NOT A TAG"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/include/version.hpp.in
                 ${CMAKE_BINARY_DIR}/include/version.hpp @ONLY)
  target_include_directories(fibsys INTERFACE ${CMAKE_BINARY_DIR}/include)

  generate_firmware_artifacts(${PROJECT_NAME})

  add_custom_target(
    flash_device
    COMMAND
      openocd -f ${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg -c
      "program ${PROJECT_BINARY_DIR}/${PROJECT_NAME} preverify verify reset exit"
    DEPENDS ${PROJECT_NAME}
    VERBATIM USES_TERMINAL # to get live output from openocd
    COMMENT "Flashing ${PROJECT_NAME} firmware...")

  add_custom_target(
    reset_device
    COMMAND openocd -f ${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg -c
            "init;reset;shutdown"
    VERBATIM
    COMMENT "Resetting target...")
endfunction()

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

target_link_options(fibsys INTERFACE -Wl,--wrap=malloc -Wl,--wrap=free
                    -Wl,--wrap=realloc -Wl,--wrap=calloc)

target_sources(fibsys INTERFACE dsp/sample.cpp dsp/math.cpp dsp/oscillator.cpp)

target_include_directories(fibsys INTERFACE utilities)
target_sources(fibsys INTERFACE utilities/stringContainer.cpp)

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
  fibsys INTERFACE streams/interfaces/asciiStreamIF.cpp streams/i2sStream.cpp
                   streams/ioStream.cpp streams/txStream.cpp)

target_include_directories(fibsys INTERFACE components components/shell)

target_link_libraries(fibsys INTERFACE ln::shell ln::shell::cmds::general
                                       ln::logger::cmds)

target_sources(fibsys INTERFACE shell_cmds/panic.cpp shell_cmds/reset.cpp
                                shell_cmds/status.cpp)
