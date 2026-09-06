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

  # set(LN_PROF ON)
  add_subdirectory(extern/ln)
  include(${CMAKE_CURRENT_LIST_DIR}/extern/ln/cmake/ln.cmake)
  ln_configure()

  ln_prof(
    EXCLUDE_FUNCTIONS
    "std::"
    "fmt::"
    "FreeRTOS::"
    "AdcIF::"
    "Fib::Dsp::Map"
    "power2"
    "power4"
    "__REV"
    "swap"
    "maxValueOfBitDepth"
    "centerValueOfBitDepth"
    "rawSampleToQ31"
    "q31ToRawSample")

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
