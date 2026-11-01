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
    "__REV"
    "_IRQHandler"
    "::get_sample"
    "::Param::Arithmetic"
    "Adc2::"
    "AdcIF::"
    "callTaskFunction"
    "centerValueOfBitDepth"
    "Fib::Dsp::Map"
    "Fib::Dsp::Osc::Waveforms"
    "Fib::Dsp::Osc"
    "Fib::Dsp::RangedValue"
    "fmt::"
    "FreeRTOS::"
    "get_instance"
    "getInstance"
    "HAL_ADC_ConvCpltCallback"
    "HAL_ADC_ConvHalfCpltCallback"
    "HAL_I2SEx_TxRxCpltCallback"
    "HAL_I2SEx_TxRxHalfCpltCallback"
    "HAL_TIM_PeriodElapsedCallback"
    "HAL_UART_RxCpltCallback"
    "HAL_UART_TxCpltCallback"
    "I2sIF::txRxCpltIsrCallback"
    "I2sIF::txRxHalfCpltIsrCallback"
    "I2sStreamer::onTxRxCompleteIsrCallback"
    "I2sStreamer::onTxRxHalfCompleteIsrCallback"
    "I2sStreamer::taskFunction"
    "ln::BinarySemaphore"
    "ln::interfaces::"
    "ln::interfaces::Mutex"
    "ln::interfaces::RecursiveMutex"
    "ln::LockGuard"
    "ln::Mutex"
    "ln::port::to_ticks"
    "ln::RecursiveMutex"
    "ln::Timeout"
    "maxValueOfBitDepth"
    "OsResource::"
    "Periph::"
    "Periph::getAdc2"
    "power2"
    "power4"
    "q31ToRawSample"
    "rawSampleToQ31"
    "std::"
    "StdStream::"
    "swap"
    "Tim6::"
    "xPortIsInsideInterrupt"
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
