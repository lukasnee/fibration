
project(Spirale)

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
      
    # System Configuration
    ${ROOT}/System/Config/FreeRTOSConfig.h
    ${ROOT}/System/Config/config.hpp

    # System Core stuff
    ${ROOT}/System/Core/main.cpp
    ${ROOT}/System/Core/memory.cpp
    ${ROOT}/System/Core/status.hpp
    ${ROOT}/System/Core/system.cpp
    ${ROOT}/System/Core/system.hpp

    # System Utilities
    ${ROOT}/System/Utilities/stringContainer.hpp
    ${ROOT}/System/Utilities/stringContainer.cpp

    ${ROOT}/Libraries/min/target/min.c
    ${ROOT}/Libraries/min/target/min.h
    ${ROOT}/System/Services/fibcom.hpp
    ${ROOT}/System/Services/fibcom.cpp

    # System Components
    ${ROOT}/System/Components/shellCommands.cpp
    ${ROOT}/System/Components/shell.cpp
    ${ROOT}/System/Components/shell.hpp
    ${ROOT}/System/Components/logger.cpp
    ${ROOT}/System/Components/logger.hpp

    # Streams Interfaces shaping interaction of whats above to whats below
    ${ROOT}/System/Streams/Interfaces/textStreamInterface.hpp
    ${ROOT}/System/Streams/Interfaces/duplexStereoStreamInterface.hpp
    ${ROOT}/System/Streams/Interfaces/duplexStereoStreamInterface.cpp
    ${ROOT}/System/Streams/Interfaces/duplexStereoStreamIF.hpp
    ${ROOT}/System/Streams/Interfaces/duplexStereoStreamIF.cpp

    # Streams Implementations - making use of peripherals efficiently
    ${ROOT}/System/Streams/i2sDuplexStream.cpp
    ${ROOT}/System/Streams/i2sDuplexStream.hpp
    ${ROOT}/System/Streams/uartTextStream.hpp
    ${ROOT}/System/Streams/uartTextStream.cpp
    ${ROOT}/System/Streams/uartTextService.cpp
    ${ROOT}/System/Streams/uartTextService.hpp

    # Peripheral Manager
    ${ROOT}/System/Peripherals/peripherals.cpp
    ${ROOT}/System/Peripherals/peripherals.hpp

    # Peripheral Interfaces shaping interaction of whats above to whats below
    ${ROOT}/System/Peripherals/Interfaces/uartIF.hpp
    ${ROOT}/System/Peripherals/Interfaces/i2sIF.hpp
    ${ROOT}/System/Peripherals/Interfaces/i2sIF.cpp
    ${ROOT}/System/Peripherals/Interfaces/adcIF.hpp

    # Peripherals Implementations - wrappers for limited resources that are available on the platform
    ${ROOT}/System/Peripherals/STM32F303R/gpio.cpp
    ${ROOT}/System/Peripherals/STM32F303R/gpio.hpp
    ${ROOT}/System/Peripherals/STM32F303R/tim6.cpp
    ${ROOT}/System/Peripherals/STM32F303R/tim6.hpp
    ${ROOT}/System/Peripherals/STM32F303R/uartIsrHandler.cpp
    ${ROOT}/System/Peripherals/STM32F303R/uart2.hpp
    ${ROOT}/System/Peripherals/STM32F303R/uart2.cpp
    ${ROOT}/System/Peripherals/STM32F303R/uart3.hpp
    ${ROOT}/System/Peripherals/STM32F303R/uart3.cpp
    ${ROOT}/System/Peripherals/STM32F303R/i2sIsrHandler.cpp
    ${ROOT}/System/Peripherals/STM32F303R/i2s2.cpp
    ${ROOT}/System/Peripherals/STM32F303R/i2s2.hpp
    ${ROOT}/System/Peripherals/STM32F303R/adc2.cpp
    ${ROOT}/System/Peripherals/STM32F303R/adc2.hpp
    # TODO: finish work ! Make all peripherals work over interfaces

# Real-Time Operating System

    # freertos-addons submodule (FreeRTOS C++ wrapper and more)
    ${ROOT}/Libraries/freertos-addons/c++/Source/cmutex.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/cqueue.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/cread_write_lock.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/csemaphore.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/ctasklet.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/cthread.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/ctimer.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/ctickhook.cpp
    ${ROOT}/Libraries/freertos-addons/c++/Source/cworkqueue.cpp
    # FreeRTOS
    ${ROOT}/Libraries/FreeRTOS/Source/croutine.c
    ${ROOT}/Libraries/FreeRTOS/Source/event_groups.c
    ${ROOT}/Libraries/FreeRTOS/Source/list.c
    ${ROOT}/Libraries/FreeRTOS/Source/queue.c
    ${ROOT}/Libraries/FreeRTOS/Source/stream_buffer.c
    ${ROOT}/Libraries/FreeRTOS/Source/tasks.c
    ${ROOT}/Libraries/FreeRTOS/Source/timers.c
    ${ROOT}/Libraries/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ${ROOT}/Libraries/FreeRTOS/Source/portable/MemMang/heap_4.c

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
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc.c
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_adc_ex.c
    # STM32F303 MCU core components
    ${ROOT}/System/MSP/stm32f3xx_hal_msp.c
    ${ROOT}/System/MSP/stm32f3xx_hal_timebase_tim.c
    ${ROOT}/System/MSP/stm32f3xx_it.c
    ${ROOT}/System/MSP/syscalls.c
    ${ROOT}/System/MSP/sysmem.c
    ${ROOT}/System/MSP/system_stm32f3xx.c
    ${ROOT}/System/MSP/startup_stm32f303retx.s
    #${ROOT}/System/MSP/startup_stm32f303cbtx.s
    ${ROOT}/System/MSP/CMSIS/Device/ST/STM32F3xx/Include/stm32f303xe.h
    #${ROOT}/System/MSP/CMSIS/Device/ST/STM32F3xx/Include/stm32f303xc.h
    
)

add_executable(${PROJECT_NAME} 
    main.cpp
    ${SYSTEM_SRC}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DUSE_HAL_DRIVER
    -DARM_MATH_CM4
    #-DSTM32F303xC
    -DSTM32F303xE
)

target_include_directories(${PROJECT_NAME} PRIVATE 

    # Fibration Framework
    ${ROOT}/System/Config/
    ${ROOT}/System/Core/
    ${ROOT}/System/Utilities/
    ${ROOT}/System/Components/
    ${ROOT}/System/
    ${ROOT}/System/Streams/Interfaces/
    ${ROOT}/System/Streams/
    ${ROOT}/System/Peripherals/Interfaces/
    ${ROOT}/System/Peripherals/STM32F303R/
    ${ROOT}/System/Peripherals/

    # Real-Time Operating System
    ${ROOT}/Libraries/FreeRTOS/Source/include/
    ${ROOT}/Libraries/freertos-addons/c++/Source/include/
    ${ROOT}/Libraries/FreeRTOS/Source/portable/GCC/ARM_CM4F/
    
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
    #-T${ROOT}/System/MSP/STM32F303CCTX_FLASH.ld
    -T${ROOT}/System/MSP/STM32F303RETX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -u _printf_float # enables floating point in printf
    
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
