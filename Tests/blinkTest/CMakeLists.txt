
project(blinkTest)

enable_language(CXX C ASM)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

set(ROOT ${CMAKE_SOURCE_DIR})

set(SYSTEM_SRC

# Hardware Platform

    # STM32F303 MCU core components
    ${ROOT}/System/MSP/stm32f3xx_hal_msp.c
    ${ROOT}/System/MSP/stm32f3xx_hal_timebase_tim.c
    ${ROOT}/System/MSP/stm32f3xx_it.c
    ${ROOT}/System/MSP/syscalls.c
    ${ROOT}/System/MSP/sysmem.c
    ${ROOT}/System/MSP/system_stm32f3xx.c
    ${ROOT}/System/MSP/startup_stm32f303cctx.s

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

# RTOS (Real-Time Operating System)

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
    # ${ROOT}/System/freertos-addons/c++/Source/ccondition_variable.cpp
    # ${ROOT}/System/freertos-addons/c++/Source/cevent_groups.cpp
    # ${ROOT}/System/freertos-addons/c++/Source/cmem_pool.cpp

# Fibration Framework

    # Hardware Peripheral Interface

    ${ROOT}/System/memory.cpp
    ${ROOT}/System/peripherals.cpp
    ${ROOT}/System/peripherals.hpp
    ${ROOT}/System/gpio.cpp
    ${ROOT}/System/gpio.hpp

    ${ROOT}/System/tim6.cpp
    ${ROOT}/System/tim6.hpp

    ${ROOT}/System/uartInterface.hpp
    ${ROOT}/System/uartIsrHandler.cpp
    ${ROOT}/System/uart1.cpp
    ${ROOT}/System/uart1.hpp
    ${ROOT}/System/uart2.cpp
    ${ROOT}/System/uart2.hpp

    ${ROOT}/System/i2sInterface.hpp
    ${ROOT}/System/i2sIsrHandler.cpp
    ${ROOT}/System/i2s2.cpp
    ${ROOT}/System/i2s2.hpp

    # FibSys (Fibration System) interface level

    ${ROOT}/System/FreeRTOSConfig.h # FreeRTOS project specific configuration
    ${ROOT}/System/system.cpp
    ${ROOT}/System/system.hpp

    ${ROOT}/System/textStreamInterface.hpp
    ${ROOT}/System/uartStream.hpp
    ${ROOT}/System/uartStream.cpp
    ${ROOT}/System/uartStreamer.cpp
    ${ROOT}/System/uartStreamer.hpp

    ${ROOT}/System/status.hpp
    ${ROOT}/System/config.hpp
    ${ROOT}/System/log.cpp
    ${ROOT}/System/log.hpp
    ${ROOT}/System/shell.cpp
    ${ROOT}/System/shell.hpp
    ${ROOT}/System/shellCmd.cpp
)

add_executable(${PROJECT_NAME} 
    main.cpp
    ${SYSTEM_SRC}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F303xC
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    
    # Hardware Platform
    ${ROOT}/System/MSP/CMSIS/Device/ST/STM32F3xx/Include
    ${ROOT}/System/MSP/CMSIS/Include
    ${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Inc
    ${ROOT}/System/MSP

    # RTOS (Real-Time Operating System)
    ${ROOT}/System/FreeRTOS/Source/include
    ${ROOT}/System/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ${ROOT}/System/freertos-addons/c++/Source/include

    # Fibration Framework
    ${ROOT}/System
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
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    
    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
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
