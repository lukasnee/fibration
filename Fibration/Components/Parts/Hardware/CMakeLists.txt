cmake_minimum_required(VERSION 3.13)

project(Hardware)

# stm32 familty prepend
set(stm32_hal STM32F3xx_HAL_Driver/Src/stm32f3xx_hal)
set(stm32_ll STM32F3xx_HAL_Driver/Src/stm32f3xx_ll)

add_library(Hardware STATIC 

# FreeRTOS kernel stuff

    FreeRTOS/Source/tasks.c
    FreeRTOS/Source/list.c
    FreeRTOS/Source/queue.c
    FreeRTOS/Source/timers.c
    FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    FreeRTOS/Source/portable/MemMang/heap_4.c

# hardware wrapper for ease (c++)

    adc.cpp
    adc.hpp
    dac.cpp
    dac.hpp
    gpio.cpp
    gpio.hpp

# STM32 hardware abstraction layer stuff

    ${stm32_hal}.c
    ${stm32_hal}_adc.c
    ${stm32_hal}_adc_ex.c
    #${stm32_hal}_can.c
    #${stm32_hal}_cec.c
    #${stm32_hal}_comp.c
    ${stm32_hal}_cortex.c
    #${stm32_hal}_crc.c
    #${stm32_hal}_crc_ex.c
    ${stm32_hal}_dac.c
    ${stm32_hal}_dac_ex.c
    ${stm32_hal}_dma.c
    #${stm32_hal}_exti.c
    ${stm32_hal}_flash.c
    ${stm32_hal}_flash_ex.c
    ${stm32_hal}_gpio.c
    #${stm32_hal}_hrtim.c
    #${stm32_hal}_i2c.c
    #${stm32_hal}_i2c_ex.c
    ${stm32_hal}_i2s.c
    ${stm32_hal}_i2s_ex.c
    #${stm32_hal}_irda.c
    #${stm32_hal}_iwdg.c
    #${stm32_hal}_msp_template.c
    #${stm32_hal}_nand.c
    #${stm32_hal}_nor.c
    #${stm32_hal}_opamp.c
    #${stm32_hal}_opamp_ex.c
    #${stm32_hal}_pccard.c
    #${stm32_hal}_pcd.c
    #${stm32_hal}_pcd_ex.c
    #${stm32_hal}_pwr.c
    #${stm32_hal}_pwr_ex.c
    ${stm32_hal}_rcc.c
    ${stm32_hal}_rcc_ex.c
    #${stm32_hal}_rtc.c
    #${stm32_hal}_rtc_ex.c
    ${stm32_hal}_sdadc.c
    #${stm32_hal}_smartcard.c
    #${stm32_hal}_smartcard_ex.c
    #${stm32_hal}_smbus.c
    #${stm32_hal}_spi.c
    #${stm32_hal}_spi_ex.c
    #${stm32_hal}_sram.c
    ${stm32_hal}_tim.c
    #${stm32_hal}_timebase_rtc_alarm_template.c
    #${stm32_hal}_timebase_rtc_wakeup_template.c
    #${stm32_hal}_timebase_tim_template.c
    ${stm32_hal}_tim_ex.c
    #${stm32_hal}_tsc.c
    ${stm32_hal}_uart.c
    ${stm32_hal}_uart_ex.c
    #${stm32_hal}_usart.c
    #${stm32_hal}_usart_ex.c
    #${stm32_hal}_wwdg.c

# STM32 low level stuff

    #${stm32_ll}_adc.c
    #${stm32_ll}_comp.c
    #${stm32_ll}_crc.c
    #${stm32_ll}_dac.c
    #${stm32_ll}_dma.c
    #${stm32_ll}_exti.c
    #${stm32_ll}_fmc.c
    #${stm32_ll}_gpio.c
    #${stm32_ll}_hrtim.c
    #${stm32_ll}_i2c.c
    #${stm32_ll}_opamp.c
    #${stm32_ll}_pwr.c
    #${stm32_ll}_rcc.c
    #${stm32_ll}_rtc.c
    #${stm32_ll}_spi.c
    #${stm32_ll}_tim.c
    #${stm32_ll}_usart.c
    #${stm32_ll}_usb.c
    #${stm32_ll}_utils.c
)

include_directories(
    FreeRTOS/Source/include
    FreeRTOS/Source/portable/GCC/ARM_CM4F
    CMSIS/Device/ST/STM32F3xx/Include
    CMSIS/Include
    STM32F3xx_HAL_Driver/Inc
)

target_compile_options(Hardware PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -fdata-sections
    -ffunction-sections

    -Wall

    $<$<CONFIG:Debug>:-Og>
)

target_compile_definitions(Hardware PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F303xC
)

#add_subdirectory(STM32)
#target_link_libraries(Hardware PUBLIC STM32)
