
project(higgs)

enable_language(CXX C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(ROOT ${CMAKE_SOURCE_DIR})

set(STM32F3xx_CORE_SRC
    ${ROOT}/system/msp/stm32f3xx_hal_msp.c
    ${ROOT}/system/msp/stm32f3xx_hal_timebase_tim.c
    ${ROOT}/system/msp/stm32f3xx_it.c
    ${ROOT}/system/msp/syscalls.cpp
    ${ROOT}/system/msp/sysmem.c
    ${ROOT}/system/msp/system_stm32f3xx.c
)

set(STM32F3xx_HAL_SRC # (Hardware Abstraction Layer)
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2s.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2s_ex.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc.c
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc_ex.c
)

set(STM32F3_MSP_SRC # (MCU Support Package)
    ${STM32F3xx_CORE_SRC}
    ${STM32F3xx_HAL_SRC}
)


set(STM32F303RC_CORE_SRC
    ${ROOT}/system/msp/startup_stm32f303cbtx.s
)

set(STM32F303CE_MSP_SRC 
    ${STM32F3_MSP_SRC}
    ${STM32F303C_CORE_SRC}
)


set(STM32F303R_CORE_SRC
    ${ROOT}/system/msp/startup_stm32f303retx.s
)

set(STM32F303RE_MSP_SRC 
    ${STM32F3_MSP_SRC}
    ${STM32F303R_CORE_SRC}
)


set(FREERTOS_BARE_SRC
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/croutine.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/event_groups.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/list.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/queue.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/stream_buffer.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/tasks.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/timers.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/portable/MemMang/heap_4.c
)

set(FREERTOS_SRC
    # freertos-addons submodule (FreeRTOS C++ wrapper and more)
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/cmutex.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/cqueue.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/cread_write_lock.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/csemaphore.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/ctasklet.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/cthread.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/ctimer.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/ctickhook.cpp
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/cworkqueue.cpp
    ${FREERTOS_BARE_SRC}
)


set(FIBSYS_CORE_SRC
    ${ROOT}/system/core/main.cpp
    ${ROOT}/system/core/memory.cpp
    ${ROOT}/system/core/system.cpp
)

set(FIBSYS_DSP_SRC
    ${ROOT}/system/dsp/sample.cpp
    ${ROOT}/system/dsp/math.cpp
    ${ROOT}/system/dsp/oscillator.cpp
)

set(FIBSYS_UTILITIES_SRC
    ${ROOT}/system/utilities/stringContainer.cpp
)

# TODO: Make all peripherals work over interfaces
set(FIBSYS_PERIPHERALS_SRC
    # Peripheral Interfaces shaping interaction of whats above to whats below
    ${ROOT}/system/peripherals/interfaces/ioDataIF.hpp
    ${ROOT}/system/peripherals/interfaces/i2sIF.cpp
    ${ROOT}/system/peripherals/interfaces/adcIF.hpp

    # Peripherals Implementations - wrappers for limited resources that are available on the platform
    ${ROOT}/system/peripherals/stm32f303r/tim6.cpp
    ${ROOT}/system/peripherals/stm32f303r/uartIsr.cpp
    ${ROOT}/system/peripherals/stm32f303r/uart2.cpp
    ${ROOT}/system/peripherals/stm32f303r/uart3.cpp
    ${ROOT}/system/peripherals/stm32f303r/i2sIsr.cpp
    ${ROOT}/system/peripherals/stm32f303r/i2s2.cpp
    ${ROOT}/system/peripherals/stm32f303r/adcIsr.cpp
    ${ROOT}/system/peripherals/stm32f303r/adc2.cpp

    ${ROOT}/system/peripherals/resources.cpp
)

set(FIBSYS_STREAMS_SRC
    # Streams Interfaces shaping interaction of whats above to whats below
    ${ROOT}/system/streams/interfaces/asciiStreamIF.cpp
    
    # Streams Implementations - making use of peripherals efficiently
    ${ROOT}/system/streams/i2sStream.cpp
    ${ROOT}/system/streams/ioStream.cpp
    ${ROOT}/system/streams/txStream.cpp
)

set(FIBSYS_COMPONENTS_SRC
    ${ROOT}/system/components/shell/shell.cpp
    ${ROOT}/system/components/shell/commands.cpp
    ${ROOT}/system/components/shell/input.cpp
    ${ROOT}/system/components/shell/argVector.cpp
    ${ROOT}/system/components/shell/argBuffer.cpp
    ${ROOT}/system/components/logger.cpp
)

set(FIBSYS_SHELL_COMMANDS_SRC
    ${ROOT}/system/shell/commands/clearShellCommand.cpp
    ${ROOT}/system/shell/commands/configShellCommand.cpp
    ${ROOT}/system/shell/commands/echoShellCommand.cpp
    ${ROOT}/system/shell/commands/hexdumpShellCommand.cpp
    ${ROOT}/system/shell/commands/panicShellCommand.cpp
    ${ROOT}/system/shell/commands/resetShellCommand.cpp
    ${ROOT}/system/shell/commands/statusShellCommand.cpp
    ${ROOT}/system/shell/commands/repeatShellCommand.cpp
    ${ROOT}/system/shell/commands/donutShellCommand.cpp
)

set(CMAKE_AR /usr/bin/arm-none-eabi-ar)
#TODO: maybe move system libraries to system/ dir. 
# Libs used should be split into two lib categories: fibsys dependant and particular apllication dependant libs.
set(FIBSYS_SRC
    ${FIBSYS_CORE_SRC}
    ${FIBSYS_UTILITIES_SRC}
    ${FIBSYS_DSP_SRC}
    ${FIBSYS_PERIPHERALS_SRC}
    ${FIBSYS_STREAMS_SRC}
    ${FIBSYS_COMPONENTS_SRC}
    ${FIBSYS_SHELL_COMMANDS_SRC}

    ${FREERTOS_SRC}
    ${STM32F303RE_MSP_SRC}
    # ${STM32F303CE_MSP_SRC}
)

# TODO: all above should roughly be distributed to other subdir cmakelists

set(HIGGS_SRC
    blinkApp.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} 
    ${FIBSYS_SRC}
    ${HIGGS_SRC}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DUSE_HAL_DRIVER
    -DARM_MATH_CM4
    # -D__FPU_PRESENT=1
    -DSTM32F303xE
    #-DSTM32F303xC
    -DFIBSYS
)

target_include_directories(${PROJECT_NAME} PRIVATE 

    ${CMAKE_CURRENT_SOURCE_DIR}
    # Fibration Framework
    ${ROOT}
    ${ROOT}/system/
    ${ROOT}/system/config/
    ${ROOT}/system/core/
    ${ROOT}/system/core/Libraries
    ${ROOT}/system/core/libraries/fibstd/include
    ${ROOT}/system/core/libraries/fibstd/libraries/eternal/include
    ${ROOT}/system/core/libraries/magic_enum/include
    ${ROOT}/system/utilities/
    ${ROOT}/system/streams/interfaces/
    ${ROOT}/system/streams/
    ${ROOT}/system/peripherals/interfaces/
    ${ROOT}/system/peripherals/stm32f303r/
    ${ROOT}/system/peripherals/
    ${ROOT}/system/components/
    ${ROOT}/system/components/shell

    # Real-Time Operating System
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/include/
    ${ROOT}/system/core/libraries/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/
    ${ROOT}/system/core/libraries/freertos-addons/c++/Source/include/
    
    # MCU Support Package
    ${ROOT}/system/msp

    # STM32F3 support
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Inc
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/CMSIS/Include
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/CMSIS/DSP/Include/
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/CMSIS/Device/ST/STM32F3xx/Include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -fsingle-precision-constant 

    -specs=nano.specs
    -fstack-usage
    -Wall
    -Wextra
    -fdata-sections
    -ffunction-sections

    # -fno-use-cxa-atexit
    # -fno-threadsafe-statics
    # -fno-rtti
    
    # -fno-exceptions # big change in code size and performace

    # -c -x assembler-with-cpp 

    # For good debug experience, Arm recommends -O1 rather than -O0. When using 
    # -O1, the compiler performs certain optimizations, but the structure of the 
    # generated code is still close to the source code.
    # -g0	no debug information
    # -g1	minimal debug information
    # -g	default debug information
    # -g3	maximal debug information

    $<$<CONFIG:Debug>:-ggdb -Ofast -DDEBUG>

    # -O3 compiler tries to reduce code size and execution time, O3 is highest standard optimization.
    # -Os Optimize for size. -Os enables all -O2 optimizations except those that often increase code size
    $<$<CONFIG:Release>:-Ofast>

)

target_link_options(${PROJECT_NAME} PRIVATE
    #-T${ROOT}/system/msp/STM32F303CCTX_FLASH.ld
    -T${ROOT}/system/msp/STM32F303RETX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -fsingle-precision-constant

    -u _printf_float # enables floating point in printf
    
    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    # --specs=nosys.specs # already defined in .cmake toolchain file
    -specs=nano.specs
    -static
    -Wl,--gc-sections

    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--cref
    -Wl,--print-memory-usage
)

target_link_libraries(${PROJECT_NAME}
    ${ROOT}/system/core/libraries/STM32CubeF3/Drivers/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a
)

# Create hex file and print executable size
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-objdump -S ${PROJECT_NAME} > ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.dump
    COMMAND arm-none-eabi-nm ${PROJECT_NAME} -C -n -S -s > ${PROJECT_NAME}.address-sort.nm
    COMMAND arm-none-eabi-nm ${PROJECT_NAME} -C -S -s --size-sort > ${PROJECT_NAME}.size-sort.nm
    COMMAND arm-none-eabi-size ${PROJECT_NAME}
)
