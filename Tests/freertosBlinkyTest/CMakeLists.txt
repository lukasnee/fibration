
project(freertosBlinkyTest)

enable_language(CXX C ASM)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)

set(ROOT ${CMAKE_SOURCE_DIR})

set(SYSTEM_SRC
	# STM32F3 system source
	${ROOT}/System/MSP/stm32f3xx_hal_msp.c
	${ROOT}/System/MSP/stm32f3xx_hal_timebase_tim.c
	${ROOT}/System/MSP/stm32f3xx_it.c
	${ROOT}/System/MSP/syscalls.c
	${ROOT}/System/MSP/sysmem.c
	${ROOT}/System/MSP/system_stm32f3xx.c
	${ROOT}/System/MSP/startup_stm32f303cctx.s

	# STM32F3 Hardware Abstraction Layer
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c	

	# FreeRTOS source
	${ROOT}/System/FreeRTOS/Source/croutine.c
	${ROOT}/System/FreeRTOS/Source/event_groups.c
	${ROOT}/System/FreeRTOS/Source/list.c
	${ROOT}/System/FreeRTOS/Source/queue.c
	${ROOT}/System/FreeRTOS/Source/stream_buffer.c
	${ROOT}/System/FreeRTOS/Source/tasks.c
	${ROOT}/System/FreeRTOS/Source/timers.c
	${ROOT}/System/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
	${ROOT}/System/FreeRTOS/Source/portable/MemMang/heap_4.c

	# FreeRTOS project specific configuration
	${ROOT}/System/FreeRTOSConfig.h

	# Fibration hardware interface
	${ROOT}/System/gpio.cpp
	${ROOT}/System/gpio.hpp
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

	${ROOT}/System/MSP/CMSIS/Device/ST/STM32F3xx/Include
	${ROOT}/System/MSP/CMSIS/Include
	${ROOT}/System/MSP/STM32F3xx_HAL_Driver/Inc
	${ROOT}/System/MSP
	
	${ROOT}/System/FreeRTOS/Source/include
	${ROOT}/System/FreeRTOS/Source/portable/GCC/ARM_CM4F

	${ROOT}/System
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    # -specs=nosys.specs
    -specs=nano.specs
    -Wl,--gc-sections
    -static

    -fdata-sections
    -ffunction-sections

	-Wall
	
	-fstack-usage
	# -fno-use-cxa-atexit
	# -fno-threadsafe-statics 
	# -fno-rtti 
	-fno-exceptions

	# -c -x assembler-with-cpp 

    $<$<CONFIG:Debug>:-Og>
    $<$<CONFIG:Release>:-O2 g3>
)

target_link_options(${PROJECT_NAME} PRIVATE
    -T${ROOT}/System/MSP/STM32F303CCTX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    # -specs=nosys.specs
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
