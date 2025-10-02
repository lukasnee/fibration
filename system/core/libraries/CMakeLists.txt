
set(CMSIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeF3/Drivers/CMSIS)
add_library(CMSIS INTERFACE)

target_include_directories(
  CMSIS INTERFACE ${CMSIS_DIR}/Include ${CMSIS_DIR}/DSP/Include
                  ${CMSIS_DIR}/Device/ST/STM32F3xx/Include)

add_library(ARM_MATH_CM4_GCC INTERFACE)
target_link_libraries(ARM_MATH_CM4_GCC
                      INTERFACE ${CMSIS_DIR}/Lib/GCC/libarm_cortexM4lf_math.a)
target_compile_definitions(ARM_MATH_CM4_GCC INTERFACE ARM_MATH_CM4)

set(STM32F3xx_HAL_Driver_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver)
add_library(STM32F3xx_HAL_Driver INTERFACE)

target_sources(
  STM32F3xx_HAL_Driver
  INTERFACE
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_adc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_adc_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_can.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_cec.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_comp.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_cortex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_crc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_crc_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_dac.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_dac_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_dma.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_exti.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_flash.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_flash_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_gpio.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_hrtim.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_i2c.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_i2c_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_i2s.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_i2s_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_irda.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_iwdg.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_nand.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_nor.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_opamp.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_opamp_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_pccard.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_pcd.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_pcd_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_pwr.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_pwr_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_rcc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_rcc_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_rtc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_rtc_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_sdadc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_smartcard.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_smartcard_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_smbus.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_spi.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_spi_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_sram.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_tim.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_tim_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_tsc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_uart.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_uart_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_usart.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_usart_ex.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_hal_wwdg.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_adc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_comp.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_crc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_dac.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_dma.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_exti.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_fmc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_gpio.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_hrtim.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_i2c.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_opamp.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_pwr.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_rcc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_rtc.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_spi.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_tim.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_usart.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_usb.c
    ${STM32F3xx_HAL_Driver_DIR}/Src/stm32f3xx_ll_utils.c)

target_include_directories(
  STM32F3xx_HAL_Driver INTERFACE ${STM32F3xx_HAL_Driver_DIR}/Inc
                              ${STM32F3xx_HAL_Driver_DIR}/Inc/Legacy)

target_compile_definitions(STM32F3xx_HAL_Driver INTERFACE USE_HAL_DRIVER
                                                       STM32F303xE)

target_compile_options(STM32F3xx_HAL_Driver INTERFACE -Wno-unused-parameter)

if(NOT TARGET STM32F3xx_HAL_conf)
  message(FATAL_ERROR "STM32F3xx_HAL_conf not found")
endif()

target_link_libraries(STM32F3xx_HAL_Driver INTERFACE CMSIS STM32F3xx_HAL_conf)
