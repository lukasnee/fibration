cmake_minimum_required(VERSION 4.0)

project(
  fibration
  LANGUAGES C CXX ASM
  VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

if(CMAKE_CROSSCOMPILING)
  add_compile_options(-Wall -Wextra -Werror)

  include(system/config/config.cmake) # TODO move config into project folders?

  add_subdirectory(extern/ln)

  ln_prof(
    EXCLUDE_FUNCTIONS
    "std::"
    "fmt::"
    "ln::Mutex"
    "ln::interfaces::Mutex"
    "ln::RecursiveMutex"
    "ln::interfaces::RecursiveMutex"
    "ln::BinarySemaphore"
    "ln::interfaces::"
    "ln::LockGuard"
    "ln::Timeout"
    "ln::port::to_ticks"
    "OsResource::"
    "StdStream::"
    "FreeRTOS::"
    "callTaskFunction"
    "I2sStreamer::taskFunction"
    "AdcIF::"
    "Adc2::"
    "Periph::getAdc2"
    "Fib::Dsp::Map"
    "Fib::Dsp::Osc"
    "Fib::Dsp::RangedValue"
    "Fib::Dsp::Osc::Waveforms"
    "power2"
    "power4"
    "::get_sample"
    "::Param::Arithmetic"
    "__REV"
    "swap"
    "maxValueOfBitDepth"
    "centerValueOfBitDepth"
    "xPortIsInsideInterrupt"
    "Periph::"
    "Tim6::"
    "rawSampleToQ31"
    "q31ToRawSample"
    "HAL_ADC_ConvHalfCpltCallback"
    "HAL_ADC_ConvCpltCallback"
    "HAL_I2SEx_TxRxHalfCpltCallback"
    "HAL_I2SEx_TxRxCpltCallback"
    "I2sIF::txRxHalfCpltIsrCallback"
    "I2sIF::txRxCpltIsrCallback"
    "I2sStreamer::onTxRxHalfCompleteIsrCallback"
    "I2sStreamer::onTxRxCompleteIsrCallback"
    "_IRQHandler"
    "getInstance"
    "get_instance"
    "HAL_TIM_PeriodElapsedCallback"
    "HAL_UART_TxCpltCallback"
    "HAL_UART_RxCpltCallback"
    EXCLUDE_FILES
    "gcc-arm-none-eabi/arm-none-eabi/include"
    "system/peripherals/resources.cpp"
    "extern/STM32CubeF3/STM32CubeF3/Drivers/CMSIS"
    "extern/STM32CubeF3/STM32CubeF3/Drivers/STM32F3xx_HAL_Driver")

  add_subdirectory(extern/STM32CubeF3)

  include(${CMAKE_CURRENT_LIST_DIR}/extern/ln/cmake/ln.cmake)
  ln_configure()

  add_subdirectory(system)
  add_subdirectory(modules)
else()
  include(CTest)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.11.0)
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  include(Catch)
endif()
