cmake_minimum_required(VERSION 4.0)

project(
  fibration
  LANGUAGES C CXX ASM
  VERSION 0.0.1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CROSSCOMPILING)
  add_compile_options(-Wall -Wextra -Werror)
  include(system/config/config.cmake) # TODO move config into project folders?
  add_subdirectory(extern/ln)
  add_subdirectory(system)
  add_subdirectory(modules)
else()
  include(CTest)
  add_subdirectory(tests)
endif()
