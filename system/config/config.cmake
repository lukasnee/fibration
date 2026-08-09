include_guard(GLOBAL)

set(LN_BLOATY ON)
set(LN_FREERTOS ON)
set(LN_LUA OFF)
set(LN_SHELL_CMD_EXCLUDE "repeat")
set(LN_PORT "STM32F3_FreeRTOS")
set(LN_FMT ON)

add_library(freertos_config INTERFACE)
target_include_directories(
  freertos_config INTERFACE ${CMAKE_CURRENT_LIST_DIR} # FreeRTOSConfig.h
)
set(FREERTOS_PORT GCC_ARM_CM4F)
set(FREERTOS_HEAP 4)

add_library(STM32F3xx_HAL_conf INTERFACE)
target_compile_definitions(STM32F3xx_HAL_conf INTERFACE STM32F303xE)
target_include_directories(
  STM32F3xx_HAL_conf
  INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../msp # stm32f3xx_hal_conf.h
)
