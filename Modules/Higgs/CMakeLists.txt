
project(Higgs)

enable_language(CXX C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

set(ROOT ${CMAKE_SOURCE_DIR})

set(STM32F3xx_CORE_SRC
    ${ROOT}/System/MSP/stm32f3xx_hal_msp.c
    ${ROOT}/System/MSP/stm32f3xx_hal_timebase_tim.c
    ${ROOT}/System/MSP/stm32f3xx_it.c
    ${ROOT}/System/MSP/syscalls.cpp
    ${ROOT}/System/MSP/sysmem.c
    ${ROOT}/System/MSP/system_stm32f3xx.c
)

set(STM32F3xx_HAL_SRC # (Hardware Abstraction Layer)
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2s.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2s_ex.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc.c
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc_ex.c
)

set(STM32F3_MSP_SRC # (MCU Support Package)
    ${STM32F3xx_CORE_SRC}
    ${STM32F3xx_HAL_SRC}
)


set(STM32F303RC_CORE_SRC
    ${ROOT}/System/MSP/startup_stm32f303cbtx.s
)

set(STM32F303CE_MSP_SRC 
    ${STM32F3_MSP_SRC}
    ${STM32F303C_CORE_SRC}
)


set(STM32F303R_CORE_SRC
    ${ROOT}/System/MSP/startup_stm32f303retx.s
)

set(STM32F303RE_MSP_SRC 
    ${STM32F3_MSP_SRC}
    ${STM32F303R_CORE_SRC}
)


set(FREERTOS_BARE_SRC
    ${ROOT}/System/MSP/FreeRTOS-Kernel/croutine.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/event_groups.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/list.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/queue.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/stream_buffer.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/tasks.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/timers.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c
    ${ROOT}/System/MSP/FreeRTOS-Kernel/portable/MemMang/heap_4.c
)

set(FREERTOS_SRC
    # freertos-addons submodule (FreeRTOS C++ wrapper and more)
    ${ROOT}/System/MSP/freertos-addons/c++/Source/cmutex.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/cqueue.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/cread_write_lock.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/csemaphore.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/ctasklet.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/cthread.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/ctimer.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/ctickhook.cpp
    ${ROOT}/System/MSP/freertos-addons/c++/Source/cworkqueue.cpp
    ${FREERTOS_BARE_SRC}
)


set(FIBSYS_CORE_SRC
    ${ROOT}/System/Core/main.cpp
    ${ROOT}/System/Core/memory.cpp
    ${ROOT}/System/Core/system.cpp

    ${ROOT}/System/Core/Commands/clear.cpp
    ${ROOT}/System/Core/Commands/config.cpp
    ${ROOT}/System/Core/Commands/echo.cpp
    ${ROOT}/System/Core/Commands/hexdump.cpp
    ${ROOT}/System/Core/Commands/panic.cpp
    ${ROOT}/System/Core/Commands/reset.cpp
    ${ROOT}/System/Core/Commands/status.cpp
    ${ROOT}/System/Core/Commands/repeat.cpp
    ${ROOT}/System/Core/Commands/donut.cpp
)

set(FIBSYS_DSP_SRC
    ${ROOT}/System/DSP/map.cpp
    ${ROOT}/System/DSP/oscillator.cpp
)

set(FIBSYS_UTILITIES_SRC
    ${ROOT}/System/Utilities/stringContainer.cpp
)

# TODO: Make all peripherals work over interfaces
set(FIBSYS_PERIPHERALS_SRC
    # Peripheral Interfaces shaping interaction of whats above to whats below
    ${ROOT}/System/Peripherals/Interfaces/ioDataIF.hpp
    ${ROOT}/System/Peripherals/Interfaces/i2sIF.cpp
    ${ROOT}/System/Peripherals/Interfaces/adcIF.hpp

    # Peripherals Implementations - wrappers for limited resources that are available on the platform
    ${ROOT}/System/Peripherals/STM32F303R/gpio.cpp
    ${ROOT}/System/Peripherals/STM32F303R/tim6.cpp
    ${ROOT}/System/Peripherals/STM32F303R/uartIsr.cpp
    ${ROOT}/System/Peripherals/STM32F303R/uart2.cpp
    ${ROOT}/System/Peripherals/STM32F303R/uart3.cpp
    ${ROOT}/System/Peripherals/STM32F303R/i2sIsr.cpp
    ${ROOT}/System/Peripherals/STM32F303R/i2s2.cpp
    ${ROOT}/System/Peripherals/STM32F303R/adcIsr.cpp
    ${ROOT}/System/Peripherals/STM32F303R/adc2.cpp

    ${ROOT}/System/Peripherals/resources.cpp
)

set(FIBSYS_STREAMS_SRC
    # Streams Interfaces shaping interaction of whats above to whats below
    ${ROOT}/System/Streams/Interfaces/duplexStereoStreamIF.cpp
    ${ROOT}/System/Streams/Interfaces/asciiStreamIF.cpp
    
    # Streams Implementations - making use of peripherals efficiently
    ${ROOT}/System/Streams/i2sDuplexStream.cpp
    ${ROOT}/System/Streams/ioStream.cpp
    ${ROOT}/System/Streams/txStream.cpp
)

set(FIBSYS_COMPONENTS_SRC
    ${ROOT}/System/Components/Shell/shell.cpp
    ${ROOT}/System/Components/Shell/commands.cpp
    ${ROOT}/System/Components/Shell/input.cpp
    ${ROOT}/System/Components/Shell/argVector.cpp
    ${ROOT}/System/Components/Shell/argBuffer.cpp
    ${ROOT}/System/Components/logger.cpp
)

#TODO: maybe move system libraries to System/ dir. 
# Libs used should be split into two lib categories: fibsys dependant and particular apllication dependant libs.
set(FIBSYS_SRC
    ${FIBSYS_CORE_SRC}
    ${FIBSYS_UTILITIES_SRC}
    ${FIBSYS_DSP_SRC}
    ${FIBSYS_PERIPHERALS_SRC}
    ${FIBSYS_STREAMS_SRC}
    ${FIBSYS_COMPONENTS_SRC}

    ${FREERTOS_SRC}
    ${STM32F303RE_MSP_SRC}
    # ${STM32F303CE_MSP_SRC}
)

# TODO: all above should roughly be distributed to other subdir cmakelists

set(SPIRALE_SRC
    main.cpp
)

add_executable(${PROJECT_NAME} 
    ${FIBSYS_SRC}
    ${SPIRALE_SRC}
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

    ${ROOT}/Modules/Higgs

    # Fibration Framework
    ${ROOT}
    ${ROOT}/System/
    ${ROOT}/System/DSP
    ${ROOT}/System/Config/
    ${ROOT}/System/Core/
    ${ROOT}/System/Utilities/
    ${ROOT}/System/Streams/Interfaces/
    ${ROOT}/System/Streams/
    ${ROOT}/System/Peripherals/Interfaces/
    ${ROOT}/System/Peripherals/STM32F303R/
    ${ROOT}/System/Peripherals/
    ${ROOT}/System/Components/
    ${ROOT}/System/Components/Shell

    # Real-Time Operating System
    ${ROOT}/System/MSP/FreeRTOS-Kernel/include/
    ${ROOT}/System/MSP/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/
    ${ROOT}/System/MSP/freertos-addons/c++/Source/include/
    
    # MCU Support Package
    ${ROOT}/System/MSP

    # STM32F3 support
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver/Inc
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/CMSIS/Include
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/CMSIS/DSP/Include/
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/CMSIS/Device/ST/STM32F3xx/Include
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

    $<$<CONFIG:Debug>:-ggdb -O1 -DDEBUG>

    # -O3 compiler tries to reduce code size and execution time, O3 is highest standard optimization.
    # -Os Optimize for size. -Os enables all -O2 optimizations except those that often increase code size
    $<$<CONFIG:Release>:-Os>

)

target_link_options(${PROJECT_NAME} PRIVATE
    #-T${ROOT}/System/MSP/STM32F303CCTX_FLASH.ld
    -T${ROOT}/System/MSP/STM32F303RETX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -fsingle-precision-constant

    -u _printf_float # enables floating point in printf
    
    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    -specs=nosys.specs
    -specs=nano.specs
    -static
    -Wl,--gc-sections

    -Wl,-Map=${PROJECT_NAME}.map
    -Wl,--cref
    -Wl,--print-memory-usage
)

target_link_libraries(${PROJECT_NAME}
    ${ROOT}/System/MSP/STM32CubeF3/Drivers/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a
)

# Create hex file and print executable size
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-nm ${PROJECT_NAME} -C -n -S -s > ${PROJECT_NAME}.address-sort.nm
    COMMAND arm-none-eabi-nm ${PROJECT_NAME} -C -S -s --size-sort > ${PROJECT_NAME}.size-sort.nm
    COMMAND arm-none-eabi-size ${PROJECT_NAME}
)
