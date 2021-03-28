
project(blinkTest)

enable_language(CXX C ASM)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

set(ROOT ${CMAKE_SOURCE_DIR})

set(SYSTEM_SRC

# Fibration Framework
# (NOTE: Following source list is ordered from highest to lowest abstraction)
      
    # FibSys (Fibration System) interface level
    ${ROOT}/System/system.cpp
    ${ROOT}/System/system.hpp
    ${ROOT}/System/peripherals.cpp
    ${ROOT}/System/peripherals.hpp
    ${ROOT}/System/shellCommands.cpp
    ${ROOT}/System/shell.cpp
    ${ROOT}/System/shell.hpp
    ${ROOT}/System/log.cpp
    ${ROOT}/System/log.hpp
    ${ROOT}/System/status.hpp
    ${ROOT}/System/config.hpp
    ${ROOT}/System/memory.cpp # this should really find better home...
    ${ROOT}/System/FreeRTOSConfig.h # this too...
    
    # Streams Interfaces shaping interaction of whats above to whats below
    ${ROOT}/System/Streams/Interfaces/textStreamInterface.hpp
    ${ROOT}/System/Streams/Interfaces/stereoAudioStreamInterface.hpp
    ${ROOT}/System/Streams/Interfaces/stereoAudioStreamInterface.cpp

    # Streams Implementations - making use of peripherals efficiently
    ${ROOT}/System/Streams/i2sAudioStream.cpp
    ${ROOT}/System/Streams/i2sAudioStream.hpp
    ${ROOT}/System/Streams/i2sAudioStreamer.cpp
    ${ROOT}/System/Streams/i2sAudioStreamer.hpp
    ${ROOT}/System/Streams/uartTextStream.hpp
    ${ROOT}/System/Streams/uartTextStream.cpp
    ${ROOT}/System/Streams/uartTextStreamer.cpp
    ${ROOT}/System/Streams/uartTextStreamer.hpp

    # Peripheral Interfaces shaping interaction of whats above to whats below
    ${ROOT}/System/Peripherals/Interfaces/uartInterface.hpp
    ${ROOT}/System/Peripherals/Interfaces/i2sInterface.hpp

    # Peripherals Implementations - wrappers for limited resources that are available on the platform
    ${ROOT}/System/Peripherals/gpio.cpp
    ${ROOT}/System/Peripherals/gpio.hpp
    ${ROOT}/System/Peripherals/tim6.cpp
    ${ROOT}/System/Peripherals/tim6.hpp
    ${ROOT}/System/Peripherals/uartIsrHandler.cpp
    ${ROOT}/System/Peripherals/uart1.cpp
    ${ROOT}/System/Peripherals/uart1.hpp
    ${ROOT}/System/Peripherals/uart2.cpp
    ${ROOT}/System/Peripherals/uart2.hpp
    ${ROOT}/System/Peripherals/i2sIsrHandler.cpp
    ${ROOT}/System/Peripherals/i2s2.cpp
    ${ROOT}/System/Peripherals/i2s2.hpp
    # TODO: finish work ! Make all peripherals work over interfaces

# Real-Time Operating System

    # freertos-addons submodule (FreeRTOS C++ wrapper and more)
    ${ROOT}/System/freertos-addons/c++/Source/cmutex.cpp
    ${ROOT}/System/freertos-addons/c++/Source/cqueue.cpp
    ${ROOT}/System/freertos-addons/c++/Source/cread_write_lock.cpp
    ${ROOT}/System/freertos-addons/c++/Source/csemaphore.cpp
    ${ROOT}/System/freertos-addons/c++/Source/ctasklet.cpp
    ${ROOT}/System/freertos-addons/c++/Source/cthread.cpp
    ${ROOT}/System/freertos-addons/c++/Source/ctimer.cpp
    ${ROOT}/System/freertos-addons/c++/Source/ctickhook.cpp
    ${ROOT}/System/freertos-addons/c++/Source/cworkqueue.cpp
    # FreeRTOS
    ${ROOT}/System/FreeRTOS/Source/croutine.c
    ${ROOT}/System/FreeRTOS/Source/event_groups.c
    ${ROOT}/System/FreeRTOS/Source/list.c
    ${ROOT}/System/FreeRTOS/Source/queue.c
    ${ROOT}/System/FreeRTOS/Source/stream_buffer.c
    ${ROOT}/System/FreeRTOS/Source/tasks.c
    ${ROOT}/System/FreeRTOS/Source/timers.c
    ${ROOT}/System/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ${ROOT}/System/FreeRTOS/Source/portable/MemMang/heap_4.c

# MCU Support Package
    # DSP stuff
    ${ROOT}/System/MSP/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.c
    ${ROOT}/System/MSP/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.c
    ${ROOT}/System/MSP/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sin_f32.c
    ${ROOT}/System/MSP/CMSIS/DSP_Lib/Source/FastMathFunctions/arm_sin_f32.c
    # STM32F3xx HAL (Hardware Abstraction Layer)
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_uart_ex.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2s.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2s_ex.c
    # STM32F303 MCU core components
    ${ROOT}/System/MSP/stm32f3xx_hal_msp.c
    ${ROOT}/System/MSP/stm32f3xx_hal_timebase_tim.c
    ${ROOT}/System/MSP/stm32f3xx_it.c
    ${ROOT}/System/MSP/syscalls.c
    ${ROOT}/System/MSP/sysmem.c
    ${ROOT}/System/MSP/system_stm32f3xx.c
    #${ROOT}/System/MSP/startup_stm32f303retx.s
    ${ROOT}/System/MSP/startup_stm32f303cbtx.s
)

add_executable(${PROJECT_NAME} 
    main.cpp
    ${SYSTEM_SRC}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DUSE_HAL_DRIVER
    -DARM_MATH_CM4
    -DSTM32F303xC
    #-DSTM32F303xE
)

target_include_directories(${PROJECT_NAME} PRIVATE 

    # Fibration Framework
    ${ROOT}/System
    ${ROOT}/Stream/Interfaces
    ${ROOT}/Stream
    ${ROOT}/Peripherals/Interfaces
    ${ROOT}/Peripherals

    # Real-Time Operating System
    ${ROOT}/System/FreeRTOS/Source/include
    ${ROOT}/System/freertos-addons/c++/Source/include
    ${ROOT}/System/FreeRTOS/Source/portable/GCC/ARM_CM4F
    
    # MCU Support Package
    ${ROOT}/System/MSP
    ${ROOT}/System/MSP/CMSIS/Include
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Inc
    ${ROOT}/System/MSP/CMSIS/Device/ST/STM32F3xx/Include
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

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
    -T${ROOT}/System/MSP/STM32F303CCTX_FLASH.ld
    #-T${ROOT}/System/MSP/STM32F303RETX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    
    -L${ROOT}/System/MSP/CMSIS/Lib
    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -larm_cortexM4lf_math -Wl,--end-group
    -specs=nosys.specs
    -specs=nano.specs
    -static
    -Wl,--gc-sections

    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--print-memory-usage
)

# Print executable size
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND arm-none-eabi-size ${PROJECT_NAME}
)

# Create hex file
add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
)
